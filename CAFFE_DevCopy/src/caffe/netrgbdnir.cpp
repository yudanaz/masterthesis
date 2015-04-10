#include "../../include/caffe/netrgbdnir.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include "../../include/caffe/data_layers.hpp"
#include "../../include/caffe/util/io.hpp"

using namespace cv;

namespace caffe {

template<typename Dtype>
void NetRGBDNIR<Dtype>::setup(std::string imgsListURL, int patchsize, int batchSize, int batchesPerImage,
                              bool RGB, bool NIR, bool depth, bool isMultiscale, std::string imageType, std::string labelImageSuffix)
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

        vector<int> randomImgPixels;
        randomPixels.push_back(randomImgPixels);
        randomPixelIndices.push_back(0);
//        imgs_uniformSubpatchSize.push_back(0); //just init here, set correct value in readNextImage()
//        imgs_uniformSubpatchIndex.push_back(0); //init here, incremented in setUniformPatches()
//        imgs_uniformSubpatchIndex_inits.push_back(false); //set all to "not initialized with random start index"

        imgMax++;
    }
//    LOG(INFO) << "max imgs in list: " << imgMax;
    inFile.close();

    //set the rest
    patchSz = patchsize;
    borderSz = patchsize / 2;
    patchCnt = 0;
    patchMax = 0; //is set correctly in readNextImage() each time an image is read, in case image sizes differ
    batchSz = batchSize;
    batchNr = 0;
    hasRGB = RGB;
    hasNIR = NIR;
    hasDepth = depth;
    multiscale = isMultiscale;
    imgType = imageType;
    labelImgSuffix = labelImageSuffix;
    batchesPerImg = batchesPerImage;

    //read first image
    readNextImage();

    //define random patches that should be read from this image
//    setRandomPatches();
//    setUniformPatches();

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

    std::vector<Mat> mats_nir0_Y;
    std::vector<Mat> mats_nir0_UV;
    std::vector<Mat> mats_nir1_Y;
    std::vector<Mat> mats_nir1_UV;
    std::vector<Mat> mats_nir2_Y;
    std::vector<Mat> mats_nir2_UV;

    std::vector<Mat> mats_depth0;
    std::vector<Mat> mats_depth1;
    std::vector<Mat> mats_depth2;
    std::vector<int> labels;

    int batchCnt = 0;
    for(batchCnt = 0; batchCnt < batchSz; )
    {
        //LOG(INFO) << "Starting nr " << batchCnt << " / " << batchSz << " in current batch";
        //if all patches in current image have been read, load next image
//        if(patchCnt >= batchesPerImg)//patchMax)
//        {
////            LOG(INFO) << "Next image";
//            readNextImage();
////            setRandomPatches();
//            setUniformPatches();
//            patchCnt = 0;
//        }

        //load next image if enough batches have been read from the current image
        if(batchNr == batchesPerImg)
        {
            readNextImage();
            batchNr = 0;
        }
        else{ batchNr++; }

        //get the correct pixel indices
        int w = img_labels.cols;

        //get the next random pixel. if the label is "unknown", go to next pixel
        int currentLabel, x, y;
        do
        {
            int randomPixel = getNextRandomPixel();
            x = randomPixel % w;
            y = randomPixel / w;
    //        int x = sparsePatches[patchCnt] % w;
    //        int y = sparsePatches[patchCnt] / w;
    //        LOG(INFO) << "patch count: " << patchCnt << "  patch: " << sparsePatches[patchCnt] << " w: " << w << " x: " << x << " y: " << y;

            //get label for this patch
            currentLabel = img_labels.at<uchar>(y,x);
//            LOG(INFO) << "label: " << currentLabel;
        }
        while(currentLabel == 255);

        labels.push_back(currentLabel);

        //jump the "unknown" label, training with this label is useless
        batchCnt++;

        //LOG(INFO) << "Get Patch for Scale 0";
        //read the current patch from the image (orig. scale 0)
        if(hasRGB)
        {
//            LOG(INFO) << "has RGB \n";
            Mat patch_rgb0 = getImgPatch(img_rgb0, x, y);
            std::vector<Mat> patch_rgb0_vect;
            split(patch_rgb0, patch_rgb0_vect);
            Mat rgb0y = patch_rgb0_vect[0];
            Mat rgb0uv;
            std::vector<Mat> rgb0uv_vec;
            rgb0uv_vec.push_back(patch_rgb0_vect[1]);
            rgb0uv_vec.push_back(patch_rgb0_vect[2]);
            merge(rgb0uv_vec, rgb0uv);
            mats_rgb0_Y.push_back(rgb0y);
            mats_rgb0_UV.push_back(rgb0uv);
//            imshow("rgb0y", rgb0y); cvWaitKey();
//            imwrite("/home/maurice/rgb0y.png", rgb0y);

            if(multiscale)
            {
//                LOG(INFO) << "is multiscale\n";
                //level 1
                Mat patch_rgb1 = getImgPatch(img_rgb1, x/2, y/2);
                std::vector<Mat> patch_rgb1_vect;
                split(patch_rgb1, patch_rgb1_vect);
                Mat rgb1y = patch_rgb1_vect[0];
                Mat rgb1uv;
                std::vector<Mat> rgb1uv_vec;
                rgb1uv_vec.push_back(patch_rgb1_vect[1]);
                rgb1uv_vec.push_back(patch_rgb1_vect[2]);
                merge(rgb1uv_vec, rgb1uv);
                mats_rgb1_Y.push_back(rgb1y);
                mats_rgb1_UV.push_back(rgb1uv);
//                imshow("rgb1y", rgb1y); cvWaitKey();
//                imwrite("/home/maurice/rgb1y.png", rgb1y);

                //level 2
                Mat patch_rgb2 = getImgPatch(img_rgb2, x/4, y/4);
                std::vector<Mat> patch_rgb2_vect;
                split(patch_rgb2, patch_rgb2_vect);
                Mat rgb2y = patch_rgb2_vect[0];
                Mat rgb2uv;
                std::vector<Mat> rgb2uv_vec;
                rgb2uv_vec.push_back(patch_rgb2_vect[1]);
                rgb2uv_vec.push_back(patch_rgb2_vect[2]);
                merge(rgb2uv_vec, rgb2uv);
                mats_rgb2_Y.push_back(rgb2y);
                mats_rgb2_UV.push_back(rgb2uv);
//                imshow("rgb2y", rgb2y); cvWaitKey();
//                imwrite("/home/maurice/rgb2y.png", rgb2y);
            }
        }

        if(hasNIR)
        {
//            LOG(INFO) << "has NIR \n";
            Mat patch_nir0 = getImgPatch(img_nir0, x, y);
            std::vector<Mat> patch_nir0_vect;
            split(patch_nir0, patch_nir0_vect);
            Mat nir0y = patch_nir0_vect[0];
            Mat nir0uv;
            std::vector<Mat> nir0uv_vec;
            nir0uv_vec.push_back(patch_nir0_vect[1]);
            nir0uv_vec.push_back(patch_nir0_vect[2]);
            merge(nir0uv_vec, nir0uv);
            mats_nir0_Y.push_back(nir0y);
            mats_nir0_UV.push_back(nir0uv);
//            imshow("nir0y", nir0y); cvWaitKey();
//            imwrite("/home/maurice/nir0y.png", nir0y);

            if(multiscale)
            {
//                LOG(INFO) << "is multiscale\n";
                //level 1
                Mat patch_nir1 = getImgPatch(img_nir1, x/2, y/2);
                std::vector<Mat> patch_nir1_vect;
                split(patch_nir1, patch_nir1_vect);
                Mat nir1y = patch_nir1_vect[0];
                Mat nir1uv;
                std::vector<Mat> nir1uv_vec;
                nir1uv_vec.push_back(patch_nir1_vect[1]);
                nir1uv_vec.push_back(patch_nir1_vect[2]);
                merge(nir1uv_vec, nir1uv);
                mats_nir1_Y.push_back(nir1y);
                mats_nir1_UV.push_back(nir1uv);
//                imshow("nir1y", nir1y); cvWaitKey();
//                imwrite("/home/maurice/nir1y.png", nir1y);

                //level 2
                Mat patch_nir2 = getImgPatch(img_nir2, x/4, y/4);
                std::vector<Mat> patch_nir2_vect;
                split(patch_nir2, patch_nir2_vect);
                Mat nir2y = patch_nir2_vect[0];
                Mat nir2uv;
                std::vector<Mat> nir2uv_vec;
                nir2uv_vec.push_back(patch_nir2_vect[1]);
                nir2uv_vec.push_back(patch_nir2_vect[2]);
                merge(nir2uv_vec, nir2uv);
                mats_nir2_Y.push_back(nir2y);
                mats_nir2_UV.push_back(nir2uv);
//                imshow("nir2y", nir2y); cvWaitKey();
//                imwrite("/home/maurice/nir2y.png", nir2y);
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
            if(nm == "nir0_Y"){ if(hasNIR)
                { ((MemoryDataLayer<Dtype>*)layer)->AddMatVector(mats_nir0_Y, labels); } }
            else if(nm == "nir0_UV"){ if(hasNIR)
                { ((MemoryDataLayer<Dtype>*)layer)->AddMatVector(mats_nir0_UV, labels); } }
            else if(nm == "nir1_Y"){ if(hasNIR && multiscale)
                { ((MemoryDataLayer<Dtype>*)layer)->AddMatVector(mats_nir1_Y, labels); } }
            else if(nm == "nir1_UV"){ if(hasNIR && multiscale)
                { ((MemoryDataLayer<Dtype>*)layer)->AddMatVector(mats_nir1_UV, labels); } }
            else if(nm == "nir2_Y"){ if(hasNIR && multiscale)
                { ((MemoryDataLayer<Dtype>*)layer)->AddMatVector(mats_nir2_Y, labels); } }
            else if(nm == "nir2_UV"){ if(hasNIR && multiscale)
                { ((MemoryDataLayer<Dtype>*)layer)->AddMatVector(mats_nir2_UV, labels); } }

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
void NetRGBDNIR<Dtype>::readNextImage()
{
    //increment image counter
    imgCnt  = (imgCnt + 1) % imgMax;

    //get next image URL, also circle through images (if iterations are > all available patches)
    std::string imageURL = imgs[imgCnt];
//    LOG(INFO) << "Read next image: " << imageURL;

    //load all image types (RGB, NIR and Depth) if available, create scales (image pyramid) pad images (make borders)
    std::string labelsNm = imageURL + labelImgSuffix + std::string(".png"); //labels lossless, always png
//    std::string rgbNm = imageURL + std::string(".") + imgType; //for stanford
    std::string rgbNm = imageURL + std::string("_rgb.") + imgType;
    std::string nirNm = imageURL + std::string("_nir.") + imgType;
    std::string depthNm = imageURL + std::string("_depth.jpg"); //depth lossless, always png

    //LOG(INFO) << "Reading image " << imageURL;
    img_labels = cv::imread(labelsNm, cv::IMREAD_GRAYSCALE); //label img isn't downsampled nor padded

    //make a shuffled list of pixel indices for each image
    if(randomPixels.at(imgCnt).size() == 0) //vector for current image not initialized yet
    {
        //fill vector holding all image pixels for current image, then shuffle it
        int totalNrOfPixels = img_labels.cols * img_labels.rows;
        srand(time(0)); //set seed for random generator using current time
        for (int i = 0; i < totalNrOfPixels; ++i)
        {
            randomPixels.at(imgCnt).push_back(i);
        }
        std::random_shuffle(randomPixels.at(imgCnt).begin(), randomPixels.at(imgCnt).end());
    }

    cv::Mat temp1;//, temp2, temp3;

    //Make the downsized images for the image pyramid and add the padding according to patch size
    //LOG(INFO) << "Making scales for RGB";
    if(hasRGB)
    {
        //read image and convert ot YUV color space
        temp1 = cv::imread(rgbNm, cv::IMREAD_COLOR);
        cvtColor(temp1, temp1, CV_BGR2YCrCb);

        if(!multiscale)
        {
            normalizeEachChannelLocally(temp1, 15);
            cv::copyMakeBorder(temp1, img_rgb0, borderSz, borderSz-1, borderSz, borderSz-1, cv::BORDER_CONSTANT, cv::Scalar(0)); //scale 0
        }
        else //make Laplacian pyramid
        {
            std::vector<Mat> pyramid = makePyramid(temp1, 3);

            //compute zero mean and unit variance for each channel
            for(int i = 0; i < 3; ++i)
            {
                Mat pyrLevel = pyramid[i];
                normalizeEachChannelLocally(pyrLevel, 15);
            }

            //make border padding around image for each pyramid level, border is whole patchsize to allow
            //for artificial jitter (rotation & scale), then the patches are cutout after applying jitter
            cv::copyMakeBorder(pyramid[0], img_rgb0, patchSz, patchSz-1, patchSz, patchSz-1, cv::BORDER_CONSTANT, cv::Scalar(0)); //scale 0
            cv::copyMakeBorder(pyramid[1], img_rgb1, patchSz, patchSz-1, patchSz, patchSz-1, cv::BORDER_CONSTANT, cv::Scalar(0)); //scale 1 (half the size)
            cv::copyMakeBorder(pyramid[2], img_rgb2, patchSz, patchSz-1, patchSz, patchSz-1, cv::BORDER_CONSTANT, cv::Scalar(0)); //scale 2 (1/4 the size)
        }
    }

    //LOG(INFO) << "Making scales for NIR";
    if(hasNIR)
    {
        temp1 = cv::imread(nirNm, cv::IMREAD_COLOR);
        cvtColor(temp1, temp1, CV_BGR2YCrCb);

        if(!multiscale)
        {
            normalizeEachChannelLocally(temp1, 15);
            cv::copyMakeBorder(temp1, img_nir0, borderSz, borderSz-1, borderSz, borderSz-1, cv::BORDER_CONSTANT, cv::Scalar(0)); //scale 0
        }
        else //make Laplacian pyramid
        {
            std::vector<Mat> pyramid = makePyramid(temp1, 3);

            //compute zero mean and unit variance for each channel
            for(int i = 0; i < 3; ++i)
            {
                Mat pyrLevel = pyramid[i];
                normalizeEachChannelLocally(pyrLevel, 15);
            }

            //make border padding around image for each pyramid level, border is whole patchsize to allow
            //for artificial jitter (rotation & scale), then the patches are cutout after applying jitter
            cv::copyMakeBorder(pyramid[0], img_nir0, patchSz, patchSz-1, patchSz, patchSz-1, cv::BORDER_CONSTANT, cv::Scalar(0)); //scale 0
            cv::copyMakeBorder(pyramid[1], img_nir1, patchSz, patchSz-1, patchSz, patchSz-1, cv::BORDER_CONSTANT, cv::Scalar(0)); //scale 1 (half the size)
            cv::copyMakeBorder(pyramid[2], img_nir2, patchSz, patchSz-1, patchSz, patchSz-1, cv::BORDER_CONSTANT, cv::Scalar(0)); //scale 2 (1/4 the size)
        }
    }

//    LOG(INFO) << "Making scales for Depth";
    if(hasDepth)
    {
        temp1 = cv::imread(depthNm, cv::IMREAD_GRAYSCALE);

        if(!multiscale)
        {
            normalizeEachChannelLocally(temp1, 15);
            cv::copyMakeBorder(temp1, img_depth0, borderSz, borderSz-1, borderSz, borderSz-1, cv::BORDER_CONSTANT, cv::Scalar(0)); //scale 0
        }
        else //make Laplacian pyramid
        {
            std::vector<Mat> pyramid = makePyramid(temp1, 3);

            //compute zero mean and unit variance for each channel
            for(int i = 0; i < 3; ++i)
            {
                Mat pyrLevel = pyramid[i];
                normalizeLocally(pyrLevel, 15);
            }

            //make border padding around image for each pyramid level, border is whole patchsize to allow
            //for artificial jitter (rotation & scale), then the patches are cutout after applying jitter
            cv::copyMakeBorder(pyramid[0], img_depth0, patchSz, patchSz-1, patchSz, patchSz-1, cv::BORDER_CONSTANT, cv::Scalar(0)); //scale 0
            cv::copyMakeBorder(pyramid[1], img_depth1, patchSz, patchSz-1, patchSz, patchSz-1, cv::BORDER_CONSTANT, cv::Scalar(0)); //scale 1 (half the size)
            cv::copyMakeBorder(pyramid[2], img_depth2, patchSz, patchSz-1, patchSz, patchSz-1, cv::BORDER_CONSTANT, cv::Scalar(0)); //scale 2 (1/4 the size)
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
vector<Mat> NetRGBDNIR<Dtype>::makePyramid(Mat img, int leveln)
{
    vector<Mat> levels;
    Mat procImg;

    levels.push_back(img);
    for(int i = 1; i < leveln; ++i)
    {
        resize(img, procImg, Size((img.cols+1)/2, (img.rows+1)/2), 0, 0, INTER_AREA);
        levels.push_back(procImg.clone());
        img = procImg.clone();
    }
    return levels;
}

template<typename Dtype>
vector<Mat> NetRGBDNIR<Dtype>::makeGaussianPyramid(Mat img, int leveln)
{
    vector<Mat> levels;
    Mat procImg;

    levels.push_back(img);
    for(int i = 1; i < leveln; ++i)
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


template<typename Dtype>
cv::Mat NetRGBDNIR<Dtype>::getImgPatch(cv::Mat img, int x, int y)
{
    //cut out patch twice the size
    cv::Mat patch2x, patchJitter, patch;
    int patchSz2x = patchSz*2;
    cv::Rect roi(x, y, patchSz2x, patchSz2x);
    img(roi).copyTo(patch2x);
//    imshow("patch", patch2x); cvWaitKey();
//    imwrite("/home/maurice/"+s+"orig.png", img);
//    imwrite("/home/maurice/"+s+"patch.png", patch2x);

    //apply jitter and crop to actual patch size
    patchJitter = makeJitter(patch2x);
//    imshow("patch with jitter", patchJitter); cvWaitKey();
//    imwrite("/home/maurice/"+s+"patch_jitter.png", patchJitter);

    int offset = borderSz - (patchSz2x - patchJitter.cols) / 2; //cols and rows should be equal, it's a square after all
    cv::Rect roi2(offset, offset, patchSz, patchSz);
    patchJitter(roi2).copyTo(patch);
//    imshow("patch with jitter resized", patch); cvWaitKey();
//    imwrite("/home/maurice/"+s+"patch_jitter_resized.png", patch);
    return patch;
}

template<typename Dtype>
Mat NetRGBDNIR<Dtype>::makeJitter(Mat img)
{
    int sz = std::max(img.cols, img.rows);
    Mat imgFlipped, imgRotated, imgScaled;

    //make random numbers for flip, rescale and rotation
    boost::uniform_int<> dist_flip(0, 1);
    boost::uniform_int<> dist_rotate(-8, 8);
    boost::uniform_int<> dist_scale(90, 110);
    boost::variate_generator<boost::mt19937&, boost::uniform_int<> > die_flip(gen, dist_flip);
    boost::variate_generator<boost::mt19937&, boost::uniform_int<> > die_rotate(gen, dist_rotate);
    boost::variate_generator<boost::mt19937&, boost::uniform_int<> > die_scale(gen, dist_scale);

    int flipping = die_flip();
    double rotAngle = die_rotate();
    double scale_fac = (float)die_scale() / 100.0;

    //flip horizontally
    if(flipping == 1){ cv::flip(img, imgFlipped, 1); }
    else{ imgFlipped = img; }

    //rotate
    if(rotAngle != 0)
    {
        Point2f centerPt(sz/2, sz/2); //the image is assumed to have size patchSz * 2
        Mat rotMat = getRotationMatrix2D(centerPt, rotAngle, 1.0);
        warpAffine(imgFlipped, imgRotated, rotMat, Size(sz, sz));
    }
    else{ imgRotated = imgFlipped; }

    //rescale
    if(scale_fac != 1.0)
    {
        int newSize = sz * scale_fac + 0.5;
        if(scale_fac > 1.0) //upscaling
        {
            cv::resize(imgRotated, imgScaled, Size(newSize, newSize), 0, 0, INTER_CUBIC);
        }
        else //downscaling
        {
            cv::resize(imgRotated, imgScaled, Size(newSize, newSize), 0, 0, INTER_AREA);;
        }
    }
    else{ imgScaled = imgRotated; }

    return imgScaled;
}

template<typename Dtype>
int NetRGBDNIR<Dtype>::getNextRandomPixel()
{
    int rpx = randomPixels.at(imgCnt).at( randomPixelIndices.at(imgCnt) );
    randomPixelIndices.at(imgCnt) = (randomPixelIndices.at(imgCnt) + 1) % randomPixels.at(imgCnt).size();
//    LOG(INFO) << imgCnt << " : " <<  randomPixelIndices.at(imgCnt);
    return rpx;
}


INSTANTIATE_CLASS(NetRGBDNIR);

} //namespace caffe
