#ifndef CROSSBILATERALFILTERWRAPPER_H
#define CROSSBILATERALFILTERWRAPPER_H

#define CHRONO
#include "crossBilateralFilter/geom.h"
#include "crossBilateralFilter/fast_lbf.h"

#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

typedef Array_2D<double> image_type;


/*!
 * \brief The CrossBilateralFilterWRAPPER class is a wrapper for the code of
 * Copyright (c) 2006, Sylvain Paris and Frédo Durand
 * as described in their paper
 *      Paris, Sylvain, and Frédo Durand. "A fast approximation of the bilateral filter
 *      using a signal processing approach." International Journal of Computer Vision
 *      81.1 (2009): 24-52.
 *
 */
class CrossBilateralFilterWRAPPER
{
public:
    CrossBilateralFilterWRAPPER();

    Mat filter(Mat srcGray, Mat edgesGray, float sigma_s, float sigma_r);

private:
    image_type getInternalImageType(Mat imgGray);
    Mat getMat(image_type img);
};

#endif // CROSSBILATERALFILTERWRAPPER_H
