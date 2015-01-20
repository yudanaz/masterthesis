#include "../../include/caffe/netrgbdnir.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include "../../include/caffe/data_layers.hpp"
#include "../../include/caffe/util/io.hpp"

namespace caffe {

template<typename Dtype>
void NetRGBDNIR<Dtype>::setup(std::string imgsListURL, int patchsize, int imgHeight, int imgWidth)
{
    //read image names from file to list
    std::ifstream inFile;
    inFile.open((char*)imgsListURL.c_str());
    std::string line;
    imgCnt = 0;
    imgMax = 0;
    while(std::getline(inFile, line))
    {
        imgs.push_back(line);
        imgMax++;
    }
    inFile.close();

    //set the rest
    patchSz = patchsize;
    borderSz = patchsize / 2;
    heigth = imgHeight;
    width = imgWidth;
    patchCnt = 0;
    patchMax = heigth * width;

    //read first image
    readNextImage();
}

template<typename Dtype>
void NetRGBDNIR<Dtype>::feedNextPatchesToInputLayers(int batchSize)
{
    vector<Datum> datums_rgb0;
    vector<Datum> datums_rgb1;
    vector<Datum> datums_rgb2;

    vector<Datum> datums_nir0;
    vector<Datum> datums_nir1;
    vector<Datum> datums_nir2;

    vector<Datum> datums_depth0;
    vector<Datum> datums_depth1;
    vector<Datum> datums_depth2;

    for(int batchCnt = 0; batchCnt < batchSize; ++batchCnt)
    {
        //if all patches in current image have been read, load next image
        if(++patchCnt == patchMax)
        {
            readNextImage();
            patchCnt = 0;
        }

        //get the correct pixel indices
        int w = img_labels.cols;
        int h = img_labels.rows;
        int x = patchCnt % w;
        int y = patchCnt / w;

        //get label for this patch
        int currentLabel = img_labels.at<uchar>(y,x);

        //read the current patch from the image (orig. scale 0)
        patch_rgb0 = getImgPatch(img_rgb0, x, y);
        patch_nir0 = getImgPatch(img_nir0, x, y);
        patch_depth0 = getImgPatch(img_depth0, x, y);

        //only read scale 1 every 2 steps
        if(patchCnt % 2 == 0)
        {
            patch_rgb1 = getImgPatch(img_rgb1, x, y);
            patch_nir1 = getImgPatch(img_nir1, x, y);
            patch_depth1 = getImgPatch(img_depth1, x, y);
        }

        //only read scale 2 every 4 steps
        if(patchCnt % 4 == 0)
        {
            patch_rgb2 = getImgPatch(img_rgb2, x, y);
            patch_nir2 = getImgPatch(img_nir2, x, y);
            patch_depth2 = getImgPatch(img_depth2, x, y);
        }

        //add patches to datum vectors
        Datum* datum_rgb0;
        Datum* datum_rgb1;
        Datum* datum_rgb2;
        Datum* datum_nir0;
        Datum* datum_nir1;
        Datum* datum_nir2;
        Datum* datum_depth0;
        Datum* datum_depth1;
        Datum* datum_depth2;

        ReadOpenCVMatToDatum(patch_rgb0, currentLabel, true, datum_rgb0);
        ReadOpenCVMatToDatum(patch_rgb1, currentLabel, true, datum_rgb1);
        ReadOpenCVMatToDatum(patch_rgb2, currentLabel, true, datum_rgb2);
        ReadOpenCVMatToDatum(patch_nir0, currentLabel, true, datum_nir0);
        ReadOpenCVMatToDatum(patch_nir1, currentLabel, true, datum_nir1);
        ReadOpenCVMatToDatum(patch_nir2, currentLabel, true, datum_nir2);
        ReadOpenCVMatToDatum(patch_depth0, currentLabel, true, datum_depth0);
        ReadOpenCVMatToDatum(patch_depth1, currentLabel, true, datum_depth1);
        ReadOpenCVMatToDatum(patch_depth2, currentLabel, true, datum_depth2);

        datums_rgb0.push_back(*datum_rgb0);
        datums_rgb1.push_back(*datum_rgb1);
        datums_rgb2.push_back(*datum_rgb2);
        datums_nir0.push_back(*datum_nir0);
        datums_nir1.push_back(*datum_nir1);
        datums_nir2.push_back(*datum_nir2);
        datums_depth0.push_back(*datum_depth0);
        datums_depth1.push_back(*datum_depth1);
        datums_depth2.push_back(*datum_depth2);
    }

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
            if(nm == "rgb0"){ ((MemoryDataLayer<Dtype>)layer).AddDatumVector(datums_rgb0); }
            else if(nm == "rgb1"){ ((MemoryDataLayer<Dtype>)layer).AddDatumVector(datums_rgb1); }
            else if(nm == "rgb2"){ ((MemoryDataLayer<Dtype>)layer).AddDatumVector(datums_rgb2); }

            else if(nm == "nir0"){ ((MemoryDataLayer<Dtype>)layer).AddDatumVector(datums_rgb0); }
            else if(nm == "nir1"){ ((MemoryDataLayer<Dtype>)layer).AddDatumVector(datums_rgb1); }
            else if(nm == "nir2"){ ((MemoryDataLayer<Dtype>)layer).AddDatumVector(datums_rgb2); }

            else if(nm == "depth0"){ ((MemoryDataLayer<Dtype>)layer).AddDatumVector(datums_depth0); }
            else if(nm == "depth1"){ ((MemoryDataLayer<Dtype>)layer).AddDatumVector(datums_depth1); }
            else if(nm == "depth2"){ ((MemoryDataLayer<Dtype>)layer).AddDatumVector(datums_depth2); }
        }
    }
}

template<typename Dtype>
cv::Mat NetRGBDNIR<Dtype>::getImgPatch(cv::Mat img, int x, int y)
{
    cv::Mat patch;
    cv::Rect srcROI(x-borderSz, y-borderSz, patchSz, patchSz);
    img(srcROI).copyTo(patch);
    return patch;
}

template<typename Dtype>
void NetRGBDNIR<Dtype>::readNextImage()
{
    //get next image URL, also circle through images (if iterations are > all available patches)
    std::string imageURL = imgs[ (imgCnt++) % imgMax ];

    //load all image types (RGB, NIR and Depth) if available, create scales (image pyramid) pad images (make borders)
    std::string labelsNm = imageURL + std::string("_label.jpg");
    std::string rgbNm = imageURL + std::string(".jpg");
    std::string nirNm = imageURL + std::string("_nir.jpg");
    std::string depthNm = imageURL + std::string("_depth.jpg");

    img_labels = cv::imread(labelsNm, cv::IMREAD_GRAYSCALE); //label img isn't downsampled nor padded

    cv::Mat temp1, temp2;

    temp1 = cv::imread(rgbNm, cv::IMREAD_COLOR);
    cv::copyMakeBorder(temp1, img_rgb0, borderSz, borderSz-1, borderSz, borderSz-1, cv::BORDER_CONSTANT, cv::Scalar(0)); //scale 0
    cv::pyrDown(temp1, temp2);
    cv::copyMakeBorder(temp2, img_rgb1, borderSz, borderSz-1, borderSz, borderSz-1, cv::BORDER_CONSTANT, cv::Scalar(0)); //scale 1 (half the size)
    cv::pyrDown(temp1, temp2);
    cv::pyrDown(temp2, temp1);
    cv::copyMakeBorder(temp1, img_rgb2, borderSz, borderSz-1, borderSz, borderSz-1, cv::BORDER_CONSTANT, cv::Scalar(0)); //scale 2 (1/4 the size)

    temp1 = cv::imread(nirNm, cv::IMREAD_COLOR);
    cv::copyMakeBorder(temp1, img_nir0, borderSz, borderSz-1, borderSz, borderSz-1, cv::BORDER_CONSTANT, cv::Scalar(0)); //scale 0
    cv::pyrDown(temp1, temp2);
    cv::copyMakeBorder(temp2, img_nir1, borderSz, borderSz-1, borderSz, borderSz-1, cv::BORDER_CONSTANT, cv::Scalar(0)); //scale 1 (half the size)
    cv::pyrDown(temp1, temp2);
    cv::pyrDown(temp2, temp1);
    cv::copyMakeBorder(temp1, img_nir2, borderSz, borderSz-1, borderSz, borderSz-1, cv::BORDER_CONSTANT, cv::Scalar(0)); //scale 2 (1/4 the size)

    temp1 = cv::imread(depthNm, cv::IMREAD_GRAYSCALE);
    cv::copyMakeBorder(temp1, img_depth0, borderSz, borderSz-1, borderSz, borderSz-1, cv::BORDER_CONSTANT, cv::Scalar(0)); //scale 0
    cv::pyrDown(temp1, temp2);
    cv::copyMakeBorder(temp2, img_depth1, borderSz, borderSz-1, borderSz, borderSz-1, cv::BORDER_CONSTANT, cv::Scalar(0)); //scale 1 (half the size)
    cv::pyrDown(temp1, temp2);
    cv::pyrDown(temp2, temp1);
    cv::copyMakeBorder(temp1, img_depth2, borderSz, borderSz-1, borderSz, borderSz-1, cv::BORDER_CONSTANT, cv::Scalar(0)); //scale 2 (1/4 the size)
}


} //namespace caffe
