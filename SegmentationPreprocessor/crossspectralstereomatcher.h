#ifndef CROSSSPECTRALSTEREOMATCHER_H
#define CROSSSPECTRALSTEREOMATCHER_H

#include <QObject>
#include "opencv2/opencv.hpp"

using namespace cv;

class CrossSpectralStereoMatcher : public QObject
{
public:
    virtual void process(Mat imgRGB_L, Mat imgNIR_R, Mat& out_disp) = 0;
};

#endif // CROSSSPECTRALSTEREOMATCHER_H
