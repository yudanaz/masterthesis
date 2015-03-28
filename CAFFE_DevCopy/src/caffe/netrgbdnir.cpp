#include "../../include/caffe/netrgbdnir.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <boost/random/uniform_int.hpp>
#include <boost/random/variate_generator.hpp>
#include "../../include/caffe/data_layers.hpp"
#include "../../include/caffe/util/io.hpp"

using namespace cv;

namespace caffe {

template<typename Dtype>
void NetRGBDNIR<Dtype>::setup(std::string imgsListURL, int patchsize, int batchSize, int imgsPerBatch,
                              bool RGB, bool NIR, bool depth, bool isMultiscale, std::string imageType)
{
    //LOG(INFO) << "Doing Setup of NetRGBDNIR";

    //read image names from file to list
    std::ifstream inFile;
    inFile.open((char*)imgsListURL.c_str());
    std::string line;
    imgCnt = -1; //init like this so it would jump to zero in first call of readNextImage()
    imgMax = 0;
    while(std::getline(inFile, line))
    {
        imgs.push_back(line);
        imgs_uniformSubpatchSize.push_back(0); //just init here, set correct value in readNextImage()
        imgs_uniformSubpatchIndex.push_back(0); //init here, incremented in setUniformPatches()
        imgs_uniformSubpatchIndex_inits.push_back(false); //set all to "not initialized with random start index"

        imgMax++;
    }
//    LOG(INFO) << "max imgs in list: " << imgMax;
    inFile.close();

    //init the vector for image channels
    for(int i = 0; i < 3; ++i)
    {
        img_rgb0.push_back(Mat());
        img_rgb1.push_back(Mat());
        img_rgb2.push_back(Mat());
    }

    //set the rest
    patchSz = patchsize;
    borderSz = patchsize / 2;
    patchCnt = 0;
    patchMax = 0; //is set correctly in readNextImage() each time an image is read, in case image sizes differ
    batchSz = batchSize;
    hasRGB = RGB;
    hasNIR = NIR;
    hasDepth = depth;
    multiscale = isMultiscale;
    imgType = imageType;

    //define how many pixels should be selected randomly out of every image, before loading a new image
    //leave this value small to avoid overfitting to the first images in training list
    sparsePatchesCntMax = batchSize / imgsPerBatch;//batchSize * 4;

    //read first image
    readNextImage();

    //define random patches that should be read from this image
//    setRandomPatches();
    setUniformPatches();

    //debug
    iteration = 0;
}

template<typename Dtype>
void NetRGBDNIR<Dtype>::feedNextPatchesToInputLayers()
{
//    LOG(INFO) << "Feeding new patches to memory data layers";
    std::vector<Mat> mats_rgb0_Y;
    std::vector<Mat> mats_rgb0_UV;
    std::vector<Mat> mats_rgb1_Y;
    std::vector<Mat> mats_rgb1_UV;
    std::vector<Mat> mats_rgb2_Y;
    std::vector<Mat> mats_rgb2_UV;

    std::vector<Mat> mats_nir0;
    std::vector<Mat> mats_nir1;
    std::vector<Mat> mats_nir2;

    std::vector<Mat> mats_depth0;
    std::vector<Mat> mats_depth1;
    std::vector<Mat> mats_depth2;
    std::vector<int> labels;

    int batchCnt = 0;
    for(batchCnt = 0; batchCnt < batchSz; ++batchCnt)
    {
        //LOG(INFO) << "Starting nr " << batchCnt << " / " << batchSz << " in current batch";

        //if all patches in current image have been read, load next image
        if(patchCnt >= sparsePatchesCntMax)//patchMax)
        {
//            LOG(INFO) << "Next image";
            readNextImage();
//            setRandomPatches();
            setUniformPatches();
            patchCnt = 0;
        }


        //get the correct pixel indices
        int w = img_labels.cols;
        int x = sparsePatches[patchCnt] % w;
        int y = sparsePatches[patchCnt] / w;
//        LOG(INFO) << "patch count: " << patchCnt << "  patch: " << sparsePatches[patchCnt] << " w: " << w << " x: " << x << " y: " << y;

        //get label for this patch
        int currentLabel = img_labels.at<uchar>(y,x);
        labels.push_back(currentLabel);
//        LOG(INFO) << "label: " << currentLabel;

        //jump the "unknown" label, training with this label is useless
        if(currentLabel == 255) //255 = unknown
        {
            batchCnt--; //go back one step so that in the end we have the correct number of patches in the batch
            patchCnt++; //but go to the next patch (this is normally done at the end of this for loop, which is now skipped)
//            LOG(INFO)<< "zero label";
            continue;
        }

        //LOG(INFO) << "Get Patch for Scale 0";
        //read the current patch from the image (orig. scale 0)
        if(hasRGB)
        {
//            LOG(INFO) << "has RGB \n";
            Mat rgb0y = img_rgb0[0];
            Mat rgb0uv;
            std::vector<Mat> rgb0uv_vec;
            rgb0uv_vec.push_back(img_rgb0[1]);
            rgb0uv_vec.push_back(img_rgb0[2]);
            merge(rgb0uv_vec, rgb0uv);

            mats_rgb0_Y.push_back(getImgPatch(rgb0y, x, y));
            mats_rgb0_UV.push_back(getImgPatch(rgb0uv, x, y));
            if(multiscale)
            {
//                LOG(INFO) << "is multiscale\n";
                //level 1
                Mat rgb1y = img_rgb1[0];
                Mat rgb1uv;
                std::vector<Mat> rgb1uv_vec;
                rgb1uv_vec.push_back(img_rgb1[1]);
                rgb1uv_vec.push_back(img_rgb1[2]);
                merge(rgb1uv_vec, rgb1uv);
                mats_rgb1_Y.push_back(getImgPatch(rgb1y, x/2, y/2));
                mats_rgb1_UV.push_back(getImgPatch(rgb1uv, x/2, y/2));

                //level 2
                Mat rgb2y = img_rgb2[0];
                Mat rgb2uv;
                std::vector<Mat> rgb2uv_vec;
                rgb2uv_vec.push_back(img_rgb2[1]);
                rgb2uv_vec.push_back(img_rgb2[2]);
                merge(rgb2uv_vec, rgb2uv);
                mats_rgb2_Y.push_back(getImgPatch(rgb2y, x/4, y/4));
                mats_rgb2_UV.push_back(getImgPatch(rgb2uv, x/4, y/4));
            }
        }

        if(hasNIR)
        {
//            LOG(INFO) << "has NIR \n";
            mats_nir0.push_back(getImgPatch(img_nir0, x, y));
            if(multiscale)
            {
                mats_nir1.push_back(getImgPatch(img_nir1, x/2, y/2));
                mats_nir2.push_back(getImgPatch(img_nir2, x/4, y/4));
            }
        }

        if(hasDepth)
        {
//            LOG(INFO) << "has Depth \n";
            mats_depth0.push_back(getImgPatch(img_depth0, x, y));
            if(multiscale)
            {
                mats_depth1.push_back(getImgPatch(img_depth1, x/2, y/2));
                mats_depth2.push_back(getImgPatch(img_depth2, x/4, y/4));
            }
        }

        patchCnt++;
    }

    //LOG(INFO) << "Feed Datum vectors to Memory Data Layers";
    //feed images to corresponding memory_data_layers. We have different input layers for
    //RGB, NIR and Depth, so convolution won't mix those channels, plus 3 different scales
    //I.e. in total there are 3 * 3 = 9 input layers
    for(int layer_id = 0; layer_id < this->layers_.size(); ++layer_id)
    {
        Layer<Dtype>* layer = this->layers_[layer_id].get(); //get stored pointer from shared pointer

        //if memory data layer, add correct datum vector to layer
        if( strcmp( layer->type(), "MemoryData") == 0 )
        {
            std::string nm = layer->layer_param().name();
            //RGB//////////////////////////////////////////////////////////////
            if(nm == "rgb0_Y"){ if(hasRGB)
                { ((MemoryDataLayer<Dtype>*)layer)->AddMatVector(mats_rgb0_Y, labels); } }
            else if(nm == "rgb0_UV"){ if(hasRGB)
                { ((MemoryDataLayer<Dtype>*)layer)->AddMatVector(mats_rgb0_UV, labels); } }
            else if(nm == "rgb1_Y"){ if(hasRGB && multiscale)
                { ((MemoryDataLayer<Dtype>*)layer)->AddMatVector(mats_rgb1_Y, labels); } }
            else if(nm == "rgb1_UV"){ if(hasRGB && multiscale)
                { ((MemoryDataLayer<Dtype>*)layer)->AddMatVector(mats_rgb1_UV, labels); } }
            else if(nm == "rgb2_Y"){ if(hasRGB && multiscale)
                { ((MemoryDataLayer<Dtype>*)layer)->AddMatVector(mats_rgb2_Y, labels); } }
            else if(nm == "rgb2_UV"){ if(hasRGB && multiscale)
                { ((MemoryDataLayer<Dtype>*)layer)->AddMatVector(mats_rgb2_UV, labels); } }


            //NIR//////////////////////////////////////////////////////////////
            else if(nm == "nir0"){ if(hasNIR)
                { ((MemoryDataLayer<Dtype>*)layer)->AddMatVector(mats_nir0, labels); } }
            else if(nm == "nir1"){ if(hasNIR && multiscale)
                { ((MemoryDataLayer<Dtype>*)layer)->AddMatVector(mats_nir1, labels); } }
            else if(nm == "nir2"){ if(hasNIR && multiscale)
                { ((MemoryDataLayer<Dtype>*)layer)->AddMatVector(mats_nir2, labels); } }

            //DEPTH////////////////////////////////////////////////////////////
            else if(nm == "depth0"){ if(hasDepth)
                {((MemoryDataLayer<Dtype>*)layer)->AddMatVector(mats_depth0, labels); } }
            else if(nm == "depth1"){ if(hasDepth && multiscale)
                { ((MemoryDataLayer<Dtype>*)layer)->AddMatVector(mats_depth1, labels); } }
            else if(nm == "depth2"){ if(hasDepth && multiscale)
                { ((MemoryDataLayer<Dtype>*)layer)->AddMatVector(mats_depth2, labels); } }
        }
    }

    //debug
    iteration++;
}

template<typename Dtype>
void NetRGBDNIR<Dtype>::setRandomPatches()
{
    boost::uniform_int<> distribution(0, patchMax - 1);
    boost::variate_generator<boost::mt19937&, boost::uniform_int<> > die(gen, distribution);

    sparsePatches.clear();

    for(int i = 0; i < sparsePatchesCntMax; ++i)
    {
        int rp = die();
//        LOG(INFO) << rp;
        sparsePatches.push_back(rp);
    }
}

template<typename Dtype>
void NetRGBDNIR<Dtype>::setUniformPatches()
{
    //get uniformly distributed indices into image
    int sp_sz = imgs_uniformSubpatchSize[imgCnt];
    int sp_i = imgs_uniformSubpatchIndex[imgCnt];

    //save indices
    sparsePatches.clear();
    for(int i = 0; i < sparsePatchesCntMax; ++i)
    {
        sparsePatches.push_back(i * sp_sz + sp_i);
    }

    //update subpatch index
    imgs_uniformSubpatchIndex[imgCnt] = (sp_i + 1) % sp_sz;
}

template<typename Dtype>
cv::Mat NetRGBDNIR<Dtype>::getImgPatch(cv::Mat img, int x, int y)
{
    cv::Mat patch;
    cv::Rect srcROI(x, y, patchSz, patchSz);
    img(srcROI).copyTo(patch);
//    cv::imshow("", patch);
//    cv::waitKey(700);
    return patch;
}

template<typename Dtype>
void NetRGBDNIR<Dtype>::readNextImage()
{
    //increment image counter
    imgCnt  = (imgCnt + 1) % imgMax;

    //get next image URL, also circle through images (if iterations are > all available patches)
    std::string imageURL = imgs[imgCnt];
//    LOG(INFO) << "Read next image: " << imageURL;

    //load all image types (RGB, NIR and Depth) if available, create scales (image pyramid) pad images (make borders)
    std::string labelsNm = imageURL + std::string("_labels.png"); //labels lossless, always png
    std::string rgbNm = imageURL + std::string(".") + imgType;
    std::string nirNm = imageURL + std::string("_nir.") + imgType;
    std::string depthNm = imageURL + std::string("_depth.jpg"); //depth lossless, always png

    //LOG(INFO) << "Reading image " << imageURL;
    img_labels = cv::imread(labelsNm, cv::IMREAD_GRAYSCALE); //label img isn't downsampled nor padded

    //set amount of patches for each image and set size of suppatches for uniform patch selection
    patchMax = img_labels.cols * img_labels.rows;
    int patchSize = patchMax / sparsePatchesCntMax;
    imgs_uniformSubpatchSize[imgCnt] = patchSize;

    //init indices for sub patches randomly ONCE
    if(!imgs_uniformSubpatchIndex_inits[imgCnt])
    {
        boost::uniform_int<> distribution(0, patchSize - 1);
        boost::variate_generator<boost::mt19937&, boost::uniform_int<> > die(gen, distribution);
        int randomIndex = die();
        imgs_uniformSubpatchIndex[imgCnt] = randomIndex;
        imgs_uniformSubpatchIndex_inits[imgCnt] = true;
//        LOG(INFO) << "random index: " << randomIndex;
    }

    cv::Mat temp1, temp2, temp3;

    //Make the downsized images for the image pyramid and add the padding according to patch size
    //LOG(INFO) << "Making scales for RGB";
    if(hasRGB)
    {
        //read image and convert ot YUV color space
        temp1 = cv::imread(rgbNm, cv::IMREAD_COLOR);
//        imshow("RGB", temp1); cvWaitKey();

        cvtColor(temp1, temp1, CV_BGR2YCrCb);
//        imshow("YUV", temp1); cvWaitKey();


        if(!multiscale)
        {
            normalizeEachChannelLocally(temp1, 15);
            cv::copyMakeBorder(temp1, img_rgb0, borderSz, borderSz-1, borderSz, borderSz-1, cv::BORDER_CONSTANT, cv::Scalar(0)); //scale 0
        }
        else //make Laplacian pyramid
        {
//            std::vector<Mat> pyramid = makeLaplacianPyramid(temp1, 3);
            std::vector<Mat> pyramid = makeGaussianPyramid(temp1, 3);

            //compute zero mean and unit variance for each channel
            for(int i = 0; i < 3; ++i)
            {
                Mat pyrLevel = pyramid[i];
                normalizeEachChannelLocally(pyrLevel, 15);
            }

            //make border padding around image for each pyramid level
            std::vector<Mat> pyramid2(3);
            cv::copyMakeBorder(pyramid[0], pyramid2[0], borderSz, borderSz-1, borderSz, borderSz-1, cv::BORDER_CONSTANT, cv::Scalar(0)); //scale 0
            cv::copyMakeBorder(pyramid[1], pyramid2[1], borderSz, borderSz-1, borderSz, borderSz-1, cv::BORDER_CONSTANT, cv::Scalar(0)); //scale 1 (half the size)
            cv::copyMakeBorder(pyramid[2], pyramid2[2], borderSz, borderSz-1, borderSz, borderSz-1, cv::BORDER_CONSTANT, cv::Scalar(0)); //scale 2 (1/4 the size)

            //split each pyramid level in Y,U,V channels
            split(pyramid2[0], img_rgb0);
            split(pyramid2[1], img_rgb1);
            split(pyramid2[2], img_rgb2);

            //show for debug
//            imshow("rgb 0 Y", img_rgb0[0]); cvWaitKey();
//            imshow("rgb 0 U", img_rgb0[1]); cvWaitKey();
//            imshow("rgb 0 V", img_rgb0[2]); cvWaitKey();
//            imshow("rgb 1 Y", img_rgb1[0]); cvWaitKey();
//            imshow("rgb 1 U", img_rgb1[1]); cvWaitKey();
//            imshow("rgb 1 V", img_rgb1[2]); cvWaitKey();
//            imshow("rgb 2 Y", img_rgb2[0]); cvWaitKey();
//            imshow("rgb 2 U", img_rgb2[1]); cvWaitKey();
//            imshow("rgb 2 V", img_rgb2[2]); cvWaitKey();
        }
    }

    //LOG(INFO) << "Making scales for NIR";
    if(hasNIR)
    {
        temp1 = cv::imread(nirNm, cv::IMREAD_COLOR);
        cv::copyMakeBorder(temp1, img_nir0, borderSz, borderSz-1, borderSz, borderSz-1, cv::BORDER_CONSTANT, cv::Scalar(0)); //scale 0

        if(multiscale)
        {
            cv::pyrDown(temp1, temp2);
            cv::copyMakeBorder(temp2, img_nir1, borderSz, borderSz-1, borderSz, borderSz-1, cv::BORDER_CONSTANT, cv::Scalar(0)); //scale 1 (half the size)

            cv::pyrDown(temp2, temp3);
            cv::copyMakeBorder(temp3, img_nir2, borderSz, borderSz-1, borderSz, borderSz-1, cv::BORDER_CONSTANT, cv::Scalar(0)); //scale 2 (1/4 the size)
        }
    }

//    LOG(INFO) << "Making scales for Depth";
    if(hasDepth)
    {
        temp1 = cv::imread(depthNm, cv::IMREAD_GRAYSCALE);
        cv::copyMakeBorder(temp1, img_depth0, borderSz, borderSz-1, borderSz, borderSz-1, cv::BORDER_CONSTANT, cv::Scalar(0)); //scale 0

        if(multiscale)
        {
            cv::pyrDown(temp1, temp2);
            cv::copyMakeBorder(temp2, img_depth1, borderSz, borderSz-1, borderSz, borderSz-1, cv::BORDER_CONSTANT, cv::Scalar(0)); //scale 1 (half the size)

            cv::pyrDown(temp2, temp3);
            cv::copyMakeBorder(temp3, img_depth2, borderSz, borderSz-1, borderSz, borderSz-1, cv::BORDER_CONSTANT, cv::Scalar(0)); //scale 2 (1/4 the size)
        }
    }
}

template<typename Dtype>
void NetRGBDNIR<Dtype>::normalizeZeroMeanUnitVariance(Mat &img)
{
    //calculate mean and standard deviation
    Scalar mean, stdDev;
    meanStdDev(img, mean, stdDev);

    //get variance
    float variance = cv::pow(stdDev[0], 2);

    //make zero mean and unit variance
     img = ((img - mean[0]) / variance);
}

template<typename Dtype>
void NetRGBDNIR<Dtype>::normalizeLocally(Mat &img, int localNbrhd)
{
    Mat img32F;
    img.convertTo(img32F, CV_32F);//, 0.003921569);
    int w = img32F.cols;
    int h = img32F.rows;

    //check from when on a smaller roi has to be used
    int x_rest = w % localNbrhd;
    int y_rest = h % localNbrhd;

    int x_limit = w - x_rest;
    int y_limit = h - y_rest;

    for (int y = 0; y < h; y += localNbrhd)
    {
        for (int x = 0; x < w; x += localNbrhd)
        {
            Rect roiRect(x, y, localNbrhd, localNbrhd);

            //padding with smaller roi
            if(x == x_limit){ roiRect.width = x_rest; }
            if(y == y_limit){ roiRect.height = y_rest; }

            Mat roi = img32F(roiRect);
            normalizeZeroMeanUnitVariance(roi);
        }
    }

//    for (int y = 0; y <= h - localNbrhd; y += localNbrhd)
//    {
//        for (int x = 0; x <= w - localNbrhd; x += localNbrhd)
//        {
//            Rect roiRect(x, y, localNbrhd, localNbrhd);
//            Mat roi = img32F(roiRect);
//            normalizeZeroMeanUnitVariance(roi);
//        }
//    }

    //convert back to 8 bit
    cv::normalize(img32F, img32F, 0.0, 1.0, NORM_MINMAX, -1);
    img32F.convertTo(img, CV_8U, 255);
}

template<typename Dtype>
void NetRGBDNIR<Dtype>::normalizeEachChannelLocally(Mat &img, int kernel)
{
    std::vector<Mat> chs(3);
    split(img, chs);
    for(int i = 0; i < 3; ++i)
    {
        normalizeLocally(chs[i], kernel);
//        normalizeLocally2(chs[i], kernel);
    }
    Mat img_;
    merge(chs, img_);
    img = img_;
}

template<typename Dtype>
void NetRGBDNIR<Dtype>::normalizeLocally2(Mat &img, int kernel)
{
    Mat img32F, mean, stdDev;

    //convert to float image
    img.convertTo(img32F, CV_32F);//, 0.003921569); // 1/255 = 0.003921569

    //estimate image mean with gaussian blur
    GaussianBlur(img32F, mean, Size(kernel, kernel), 0);
    img32F = img32F - mean;

    //estimate standard deviation with gaussian blur by doing sqrt( gauss_blur(img²) )
    GaussianBlur(img32F.mul(img32F), mean, Size(kernel*81, kernel*81), 0); //re-use mean matrix
    cv::pow(mean, 0.5, stdDev);
    img32F = img32F / stdDev;

    //convert back to 8 bit
    cv::normalize(img32F, img32F, 0, 1, NORM_MINMAX, -1);
    img32F.convertTo(img, CV_8U, 255);
}

template<typename Dtype>
vector<Mat> NetRGBDNIR<Dtype>::makeGaussianPyramid(Mat img, int leveln)
{
    vector<Mat> levels;
    Mat procImg;

    levels.push_back(img);
    for(int i = 0; i < leveln; ++i)
    {
        pyrDown(img, procImg);
        levels.push_back(procImg.clone());
        img = procImg.clone();
    }
    return levels;
}


template<typename Dtype>
vector<Mat> NetRGBDNIR<Dtype>::makeLaplacianPyramid(Mat img, int leveln)
{
    vector<Mat> levels;
    Mat next_img, img1;

    for(int i = 0; i < leveln-1; ++i)
    {
        pyrDown(img, next_img);
        pyrUp(next_img, img1, img.size());
        levels.push_back(img - img1);
        img = next_img;
    }
    levels.push_back(img);
    return levels;
}

INSTANTIATE_CLASS(NetRGBDNIR);

} //namespace caffe
