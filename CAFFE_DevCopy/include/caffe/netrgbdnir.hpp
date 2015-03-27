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
    explicit NetRGBDNIR(const string& param_file, Phase phase) : Net<Dtype>(param_file, phase){}

    void setup(std::string imgsListURL, int patchsize, int batchSize, int imgsPerBatch, bool RGB, bool NIR, bool depth, bool isMultiscale, std::string imgType);
    void feedNextPatchesToInputLayers();

protected:
    void readNextImage();
    void setRandomPatches();
    void setUniformPatches();
    cv::Mat getImgPatch(cv::Mat img, int x, int y);

    /*!
     * \brief Pre-processes an image to zero mean and unit variance
     * (standard deviation) for local neighborhoods in order to make
     * it more machine-learning-friendly.
     * inspired by
     * http://bigwww.epfl.ch/demo/jlocalnormalization/
     * and
     * http://bigwww.epfl.ch/sage/soft/localnormalization/]
     *
     * Local normalization is done using ESTIMATIONS for mean and
     * standard deviation achieved with a gaussian smoothing filter.
     * \param img is the original image. NOTE THAT IF IT'S A MULTICHANNEL IMAGE,
     * NORMALIZATION IS DONE FOR EACH CHANNEL SEPARATELY, THIS HAS BEEN TESTED
     * AND PROVED IN GIMP, SEE TEST IN MAINWINDOW!
     * \param meanKernel: The Gaussian kernel size used for mean estimation.
     * \param stdDevKernel: The Gaussian kernel size used for standard deviation estimation.
     * \return the standardized image as a float matrix.
     */
    cv::Mat NormalizeLocally(cv::Mat img, int meanKernel, int stdDevKernel, bool outputAs8bit = true);


    int patchSz;
    int borderSz;
    std::vector<std::string> imgs;
    int imgCnt;
    int imgMax;
    int patchCnt;
    int patchMax;
    int batchSz;
    std::vector<int> imgs_uniformSubpatchSize; //stores the size of the uniform subpatches for each image
    std::vector<int> imgs_uniformSubpatchIndex; //stores the index in the uniform subpatches for each image
    std::vector<bool> imgs_uniformSubpatchIndex_inits; //stores whether this subpatch index has been initialized with a random index

    boost::mt19937 gen; //random number generator
    int sparsePatchesCntMax;
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
