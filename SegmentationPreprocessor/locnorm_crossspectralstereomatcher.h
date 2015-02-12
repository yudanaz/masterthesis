#ifndef LOCNORM_CROSSSPECTRALSTEREOMATCHER_H
#define LOCNORM_CROSSSPECTRALSTEREOMATCHER_H

#include "crossspectralstereomatcher.h"
#include "opencv2/opencv.hpp"

using namespace cv;

class LocNorm_crossSpectralStereoMatcher : public CrossSpectralStereoMatcher
{
public:
    LocNorm_crossSpectralStereoMatcher();

    virtual void process(Mat imgRGB_L, Mat imgNIR_R, Mat &out_disp);

private:
    /*!
     * \brief Pre-processes an image to zero mean and unit variance
     * (standard deviation) for local neighborhoods in order to make
     * it more machine-learning-friendly.
     * inspired by
     * http://bigwww.epfl.ch/demo/jlocalnormalization/
     */
    Mat NormalizeLocally(Mat img, int meanKernel, int stdDevKernel, bool outputAs8bit = true);
};

#endif // LOCNORM_CROSSSPECTRALSTEREOMATCHER_H
