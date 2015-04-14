#ifndef SKINDETECTOR_H
#define SKINDETECTOR_H

#include "opencv2/opencv.hpp"

using namespace cv;

class SkinDetector
{
public:
	SkinDetector();

	Mat detect(Mat nirImg);
	Mat filter(Mat img1, Mat img2, int min1, int min2, double lowThresh, double highThresh);

private:
	int min970;
	int min1300;
	int min1550;
	double lowThresh1;
	double lowThresh2;
	double lowThresh3;
	double highThresh1;
	double highThresh2;
	double highThresh3;
};

#endif // SKINDETECTOR_H
