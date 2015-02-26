#include "../../include/caffe/netrgbdnir.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <boost/random/uniform_int.hpp>
#include <boost/random/variate_generator.hpp>
#include "../../include/caffe/data_layers.hpp"
#include "../../include/caffe/util/io.hpp"

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
    imgCnt = 999999; //init high so it would jump to zero in first call of readNextImage()
    imgMax = 0;
    while(std::getline(inFile, line))
    {
        imgs.push_back(line);
        imgs_uniformSubpatchSize.push_back(0); //just init here, set correct value in readNextImage()
        imgs_uniformSubpatchIndex.push_back(0); //init here, incremented in setUniformPatches()
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
}

template<typename Dtype>
void NetRGBDNIR<Dtype>::feedNextPatchesToInputLayers()
{
//    LOG(INFO) << "Feeding new patches to memory data layers";
    vector<Datum> datums_rgb0;
    vector<Datum> datums_rgb1;
    vector<Datum> datums_rgb2;
    vector<Datum> datums_nir0;
    vector<Datum> datums_nir1;
    vector<Datum> datums_nir2;
    vector<Datum> datums_depth0;
    vector<Datum> datums_depth1;
    vector<Datum> datums_depth2;

    for(int batchCnt = 0; batchCnt < batchSz; ++batchCnt)
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

        //LOG(INFO) << "Get Patch for Scale 0";
        //read the current patch from the image (orig. scale 0)
        if(hasRGB)
        {
            patch_rgb0 = getImgPatch(img_rgb0, x, y);
            if(multiscale)
            {
                /*if(patchCnt % 2 == 0)*/{ patch_rgb1 = getImgPatch(img_rgb1, x/2, y/2); }
                /*if(patchCnt % 4 == 0)*/{ patch_rgb2 = getImgPatch(img_rgb2, x/4, y/4); }
            }
        }

        if(hasNIR)
        {
            patch_nir0 = getImgPatch(img_nir0, x, y);
            if(multiscale)
            {
                /*if(patchCnt % 2 == 0)*/{ patch_nir1 = getImgPatch(img_nir1, x/2, y/2); }
                /*if(patchCnt % 4 == 0)*/{ patch_nir2 = getImgPatch(img_nir2, x/4, y/4); }
            }
        }

        if(hasDepth)
        {
            patch_depth0 = getImgPatch(img_depth0, x, y);
            if(multiscale)
            {
                /*if(patchCnt % 2 == 0)*/{ patch_depth1 = getImgPatch(img_depth1, x/2, y/2); }
                /*if(patchCnt % 4 == 0)*/{ patch_depth2 = getImgPatch(img_depth2, x/4, y/4); }
            }
        }



        //add patches to datum vectors
        Datum datum_rgb0;
        Datum datum_rgb1;
        Datum datum_rgb2;
        Datum datum_nir0;
        Datum datum_nir1;
        Datum datum_nir2;
        Datum datum_depth0;
        Datum datum_depth1;
        Datum datum_depth2;

        //LOG(INFO) << "Read opencv Mats to Datums";
        if(hasRGB)
        {
            ReadOpenCVMatToDatum(patch_rgb0, currentLabel, true, &datum_rgb0);
            if(multiscale)
            {
                ReadOpenCVMatToDatum(patch_rgb1, currentLabel, true, &datum_rgb1);
                ReadOpenCVMatToDatum(patch_rgb2, currentLabel, true, &datum_rgb2);
            }
        }

        if(hasNIR)
        {
            ReadOpenCVMatToDatum(patch_nir0, currentLabel, true, &datum_nir0);
            if(multiscale)
            {
                ReadOpenCVMatToDatum(patch_nir1, currentLabel, true, &datum_nir1);
                ReadOpenCVMatToDatum(patch_nir2, currentLabel, true, &datum_nir2);
            }
        }

        if(hasDepth)
        {
            ReadOpenCVMatToDatum(patch_depth0, currentLabel, false, &datum_depth0);
            if(multiscale)
            {
                ReadOpenCVMatToDatum(patch_depth1, currentLabel, false, &datum_depth1);
                ReadOpenCVMatToDatum(patch_depth2, currentLabel, false, &datum_depth2);
            }
        }

        //LOG(INFO) << "Push Datums into vectors";
        if(hasRGB)
        {
            datums_rgb0.push_back(datum_rgb0);
            if(multiscale)
            {
                datums_rgb1.push_back(datum_rgb1);
                datums_rgb2.push_back(datum_rgb2);
            }
        }

        if(hasNIR)
        {
            datums_nir0.push_back(datum_nir0);
            if(multiscale)
            {
                datums_nir1.push_back(datum_nir1);
                datums_nir2.push_back(datum_nir2);

            }
        }

        if(hasDepth)
        {
            datums_depth0.push_back(datum_depth0);
            if(multiscale)
            {
                datums_depth1.push_back(datum_depth1);
                datums_depth2.push_back(datum_depth2);
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
        if(layer->type() == LayerParameter_LayerType_MEMORY_DATA)
        {
            std::string nm = layer->layer_param().name();
            if(nm == "rgb0"){ if(hasRGB)
                { ((MemoryDataLayer<Dtype>*)layer)->AddDatumVector(datums_rgb0); } }
            else if(nm == "rgb1"){ if(hasRGB && multiscale)
                { ((MemoryDataLayer<Dtype>*)layer)->AddDatumVector(datums_rgb1); } }
            else if(nm == "rgb2"){ if(hasRGB && multiscale)
                { ((MemoryDataLayer<Dtype>*)layer)->AddDatumVector(datums_rgb2); } }

            else if(nm == "nir0"){ if(hasNIR)
                { ((MemoryDataLayer<Dtype>*)layer)->AddDatumVector(datums_rgb0); } }
            else if(nm == "nir1"){ if(hasNIR && multiscale)
                { ((MemoryDataLayer<Dtype>*)layer)->AddDatumVector(datums_rgb1); } }
            else if(nm == "nir2"){ if(hasNIR && multiscale)
                { ((MemoryDataLayer<Dtype>*)layer)->AddDatumVector(datums_rgb2); } }

            else if(nm == "depth0"){ if(hasDepth)
                {((MemoryDataLayer<Dtype>*)layer)->AddDatumVector(datums_depth0); } }
            else if(nm == "depth1"){ if(hasDepth && multiscale)
                { ((MemoryDataLayer<Dtype>*)layer)->AddDatumVector(datums_depth1); } }
            else if(nm == "depth2"){ if(hasDepth && multiscale)
                { ((MemoryDataLayer<Dtype>*)layer)->AddDatumVector(datums_depth2); } }
        }
    }
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
    LOG(INFO) << "Read next image: " << imageURL;

    //load all image types (RGB, NIR and Depth) if available, create scales (image pyramid) pad images (make borders)
    std::string labelsNm = imageURL + std::string("_labels.png"); //labels lossless, always png
    std::string rgbNm = imageURL + std::string(".") + imgType;
    std::string nirNm = imageURL + std::string("_nir.") + imgType;
    std::string depthNm = imageURL + std::string("_depth.jpg"); //depth lossless, always png

    //LOG(INFO) << "Reading image " << imageURL;
    img_labels = cv::imread(labelsNm, cv::IMREAD_GRAYSCALE); //label img isn't downsampled nor padded

    //set amount of patches for each image and set size of suppatches for uniform patch selection
    patchMax = img_labels.cols * img_labels.rows;
    imgs_uniformSubpatchSize[imgCnt] = patchMax / sparsePatchesCntMax;

    cv::Mat temp1, temp2, temp3;

    //LOG(INFO) << "Making scales for RGB";
    if(hasRGB)
    {
        temp1 = cv::imread(rgbNm, cv::IMREAD_COLOR);
        cv::copyMakeBorder(temp1, img_rgb0, borderSz, borderSz-1, borderSz, borderSz-1, cv::BORDER_CONSTANT, cv::Scalar(0)); //scale 0

        if(multiscale)
        {
            cv::pyrDown(temp1, temp2);
            cv::copyMakeBorder(temp2, img_rgb1, borderSz, borderSz-1, borderSz, borderSz-1, cv::BORDER_CONSTANT, cv::Scalar(0)); //scale 1 (half the size)

            cv::pyrDown(temp2, temp3);
            cv::copyMakeBorder(temp3, img_rgb2, borderSz, borderSz-1, borderSz, borderSz-1, cv::BORDER_CONSTANT, cv::Scalar(0)); //scale 2 (1/4 the size)
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

    //LOG(INFO) << "Making scales for Depth";
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

INSTANTIATE_CLASS(NetRGBDNIR);

} //namespace caffe
