#ifndef CAFFE_NETRGBDNIR_HPP_
#define CAFFE_NETRGBDNIR_HPP_

#include "../../include/caffe/net.hpp"
#include <opencv2/opencv.hpp>
#include "../../include/caffe/data_layers.hpp"
#include <boost/random/mersenne_twister.hpp>

namespace caffe {

template <typename Dtype>
class NetRGBDNIR : public Net<Dtype>
{
public:
    explicit NetRGBDNIR(const NetParameter& param) : Net<Dtype>(param){}
    explicit NetRGBDNIR(const string& param_file) : Net<Dtype>(param_file){}

    void setup(std::string imgsListURL, int patchsize, int batchSize, bool RGB, bool NIR, bool depth, bool isMultiscale);
    void feedNextPatchesToInputLayers();

protected:
    void readNextImage();
    void setRandomPatches();
    cv::Mat getImgPatch(cv::Mat img, int x, int y);

    int patchSz;
    int borderSz;
    std::vector<std::string> imgs;
    int imgCnt;
    int imgMax;
    int patchCnt;
    int patchMax;
    int batchSz;

    boost::mt19937 gen; //random number generator
    int randomPatchesCntMax;
    vector<int> randomPatches;

    bool hasRGB;
    bool hasNIR;
    bool hasDepth;
    bool multiscale;

    cv::Mat img_labels;

    cv::Mat img_rgb0;
    cv::Mat img_rgb1;
    cv::Mat img_rgb2;
    cv::Mat img_nir0;
    cv::Mat img_nir1;
    cv::Mat img_nir2;
    cv::Mat img_depth0;
    cv::Mat img_depth1;
    cv::Mat img_depth2;

    cv::Mat patch_rgb0;
    cv::Mat patch_rgb1;
    cv::Mat patch_rgb2;
    cv::Mat patch_nir0;
    cv::Mat patch_nir1;
    cv::Mat patch_nir2;
    cv::Mat patch_depth0;
    cv::Mat patch_depth1;
    cv::Mat patch_depth2;
};

} //namespace caffe
#endif // CAFFE_NETRGBDNIR_HPP_
