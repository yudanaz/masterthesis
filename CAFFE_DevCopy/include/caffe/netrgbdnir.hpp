#ifndef CAFFE_NETRGBDNIR_HPP_
#define CAFFE_NETRGBDNIR_HPP_

#include "../../include/caffe/net.hpp"
#include <opencv2/opencv.hpp>
#include "../../include/caffe/data_layers.hpp"
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int.hpp>
#include <boost/random/variate_generator.hpp>

namespace caffe {

template <typename Dtype>
class NetRGBDNIR : public Net<Dtype>
{
public:
    explicit NetRGBDNIR(const NetParameter& param) : Net<Dtype>(param){}
    explicit NetRGBDNIR(const string& param_file, Phase phase) : Net<Dtype>(param_file, phase){}

    void setup(std::string imgsListURL, int patchsize, int batchSize, int batchesPerImage, bool RGB, bool NIR, bool depth, bool isMultiscale, std::string imgType);
    void feedNextPatchesToInputLayers();

protected:
    void readNextImage();
//    void setRandomPatches();
//    void setUniformPatches();

    void normalizeZeroMeanUnitVariance(cv::Mat &img);
    void normalizeEachChannelLocally(cv::Mat &img, int localNbrhd);
    void normalizeLocally(cv::Mat &img, int localNbrhd);
    void normalizeLocally2(cv::Mat &img, int kernel);
    vector<cv::Mat> makePyramid(cv::Mat img, int leveln=3);
    vector<cv::Mat> makeGaussianPyramid(cv::Mat img, int leveln=3);
    vector<cv::Mat> makeLaplacianPyramid(cv::Mat img, int leveln=3);
    cv::Mat getImgPatch(cv::Mat img, int x, int y, string s="");
    cv::Mat makeJitter(cv::Mat img);


    int patchSz;
    int borderSz;
    std::vector<std::string> imgs;
    int imgCnt;
    int imgMax;
    int patchCnt;
    int patchMax;
    int batchSz;
    int batchNr;
    std::vector<int> imgs_uniformSubpatchSize; //stores the size of the uniform subpatches for each image
    std::vector<int> imgs_uniformSubpatchIndex; //stores the index in the uniform subpatches for each image
    std::vector<bool> imgs_uniformSubpatchIndex_inits; //stores whether this subpatch index has been initialized with a random index

    //vector that holds random pixel number generators for each image:
    int getNextRandomPixel();
    std::vector< std::vector<int> > randomPixels;
    std::vector<int> randomPixelIndices;

    boost::mt19937 gen; //random number generator
    int batchesPerImg;
    vector<int> sparsePatches;

    bool hasRGB;
    bool hasNIR;
    bool hasDepth;
    bool multiscale;
    string imgType;

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

    //debug
    long iteration;
};

} //namespace caffe
#endif // CAFFE_NETRGBDNIR_HPP_
