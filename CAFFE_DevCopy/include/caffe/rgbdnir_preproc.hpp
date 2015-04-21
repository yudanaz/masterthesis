#ifndef RGBDNIR_PREPROC_HPP
#define RGBDNIR_PREPROC_HPP

#include "../../include/caffe/net.hpp"
#include <opencv2/opencv.hpp>
using namespace cv;

class RGBDNIR_preproc
{
public:
	RGBDNIR_preproc();

	void normalizeEachChannelLocally(Mat &img, int localNbrhd);
	std::vector<Mat> makePyramid(Mat img, int leveln=3, int interpolMethod = cv::INTER_AREA);
	void normalizeZeroMeanUnitVariance(Mat &img);
	void normalizeLocallyWithPatches(Mat &img, int patchSize);
	void normalize(Mat &img);
	void normalizeLocally2(Mat &img, int kernel);
	std::vector<Mat> makeGaussianPyramid(Mat img, int leveln=3);
	std::vector<Mat> makeLaplacianPyramid(Mat img, int leveln=3);
};

#endif // RGBDNIR_PREPROC_HPP
