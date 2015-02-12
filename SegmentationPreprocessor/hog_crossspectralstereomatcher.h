#ifndef HOG_CROSSSPECTRALSTEREOMATCHER_H
#define HOG_CROSSSPECTRALSTEREOMATCHER_H

#include "crossspectralstereomatcher.h"
#include "opencv2/opencv.hpp"

using namespace cv;

class HOG_crossSpectralStereoMatcher : public CrossSpectralStereoMatcher
{
public:
    HOG_crossSpectralStereoMatcher();

    virtual void process(Mat imgRGB_L, Mat imgNIR_R, Mat &out_disp);

    Mat makeDisparity_WTA(vector<float>& values_L, vector<float>& values_R, Size imgSize, int blockSize, int nCells, int nBins);

    static void setParams(int disparityRange);


private:

    void getDescriptor(vector<float> &allDescriptors, vector<float> &descriptor, int blockSize, int currentPixel);

    float getL1Distance(vector<float> a, vector<float> b);

    static int dispRange;
};

#endif // HOG_CROSSSPECTRALSTEREOMATCHER_H
