#ifndef CAFFE_NETRGBDNIR_HPP_
#define CAFFE_NETRGBDNIR_HPP_

#include "../../include/caffe/net.hpp"
#include <opencv2/opencv.hpp>
#include "../../include/caffe/data_layers.hpp"
#include "../../include/caffe/rgbdnir_preproc.hpp"
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/random.hpp>

namespace caffe {

//from: Using boost::random as the RNG for std::random_shuffle
//http://stackoverflow.com/questions/147391/using-boostrandom-as-the-rng-for-stdrandom-shuffle
//answer 13
struct bar : std::unary_function<unsigned, unsigned> {
    boost::mt19937 &_state;
    unsigned operator()(unsigned i) {
        boost::uniform_int<> rng(0, i - 1);
        return rng(_state);
    }
    bar(boost::mt19937 &state) : _state(state) {}
};


template <typename Dtype>
class NetRGBDNIR : public Net<Dtype>
{
public:
    explicit NetRGBDNIR(const NetParameter& param) : Net<Dtype>(param){}
    explicit NetRGBDNIR(const string& param_file, Phase phase) : Net<Dtype>(param_file, phase){}

	void setup(std::string imgsListURL, int patchsize, int batchSize, int batchesPerImage, bool RGB, bool NIR, bool depth, bool skin, bool isMultiscale, std::string imgType, std::string labelImageSuffix);
	void feedNextPatchesToInputLayers();

protected:
	void readAllImages();
	void getNextImage();

	cv::Mat getImgPatch(cv::Mat img, int x, int y, bool isDepth=false);
	void setJitterRandomVars();
	cv::Mat makeJitter(cv::Mat img, bool noInterpolation = false);

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

	//vector that holds random pixel number generators for each image:
	int getNextRandomPixel();
    std::vector< std::vector<long> > randomPixels;
    std::vector<long> randomPixelIndices;

	//random number generator and random vars for jitter
    boost::mt19937 rndmGen;
    boost::uniform_int<> distrib_flip;
    boost::uniform_int<> distrib_rotate;
    boost::uniform_int<> distrib_scale;
    boost::uniform_int<> distrib_imageList;

	int jitter_flipping;
	double jitter_rotAngle;
	double jitter_scale_fac;

    int batchNr;
	int batchesPerImg;
	vector<int> sparsePatches;

	bool hasRGB;
	bool hasNIR;
	bool hasDepth;
	bool hasSkin;
	bool multiscale;
	std::string imgType;
	std::string labelImgSuffix;

	cv::Mat img_labels;
	std::vector<cv::Mat> imgs_labels;

	cv::Mat img_rgb0;
	cv::Mat img_rgb1;
	cv::Mat img_rgb2;
	cv::Mat img_nir0;
	cv::Mat img_nir1;
	cv::Mat img_nir2;
	cv::Mat img_depth0;
	cv::Mat img_depth1;
	cv::Mat img_depth2;
	cv::Mat img_skin0;
	cv::Mat img_skin1;
	cv::Mat img_skin2;

	std::vector<cv::Mat> imgs_rgb0;
	std::vector<cv::Mat> imgs_rgb1;
	std::vector<cv::Mat> imgs_rgb2;
	std::vector<cv::Mat> imgs_nir0;
	std::vector<cv::Mat> imgs_nir1;
	std::vector<cv::Mat> imgs_nir2;
	std::vector<cv::Mat> imgs_depth0;
	std::vector<cv::Mat> imgs_depth1;
	std::vector<cv::Mat> imgs_depth2;
	std::vector<cv::Mat> imgs_skin0;
	std::vector<cv::Mat> imgs_skin1;
	std::vector<cv::Mat> imgs_skin2;

	//image preprocessor:
	RGBDNIR_preproc preproc;

	//debug
	long iteration;
};

} //namespace caffe
#endif // CAFFE_NETRGBDNIR_HPP_
