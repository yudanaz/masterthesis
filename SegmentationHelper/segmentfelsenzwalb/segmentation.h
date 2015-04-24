#ifndef SEGMENTATION_H
#define SEGMENTATION_H

#include <cstdio>
#include <cstdlib>
#include "image.h"
#include "misc.h"

#include "opencv2/opencv.hpp"

using namespace cv;

class Segmentation
{
public:
	Segmentation();
	Mat makeSuperPixelSegmenation(Mat img, float sigma, float k, int min_size, int *num_ccs, bool get16BitGrayscaleInsteadOfColorImg);

private:
	Mat getMat(image<rgb> *rgbimg);
	image<rgb>* getRGBimg(Mat img);
};



#endif // SEGMENTATION_H
