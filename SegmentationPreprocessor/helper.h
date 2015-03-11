#ifndef HELPER_H
#define HELPER_H

#include "opencv2/opencv.hpp"
using namespace cv;

class Helper
{
public:
	Helper();

	//stolen from http://www.juergenwiki.de/work/wiki/doku.php?id=public:hog_descriptor_computation_and_visualization#computing_the_hog_descriptor_using_opencv
	static Mat get_hogdescriptor_visual_image(Mat& origImg,
									   vector<float>& descriptorValues,
									   Size winSize,
									   Size cellSize,
									   int scaleFactor,
									   double viz_factor);

	//thinning operation on binary image using the Zhang-Suen algorithm
	static void thinning(cv::Mat& im);

private:
	static void thinningIteration(cv::Mat& im, int iter);
};

#endif // HELPER_H
