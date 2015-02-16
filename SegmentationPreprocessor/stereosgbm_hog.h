#ifndef STEREOSGBM_HOG_H
#define STEREOSGBM_HOG_H

#include "opencv2/opencv.hpp"

using namespace cv;

typedef uchar PixType;
typedef short CostType;
typedef short DispType;

enum { NR = 16, NR2 = NR/2 };

class StereoSGBM_HOG
{
public:
    StereoSGBM_HOG();

    void calcPixelCostBT( const Mat& img1, const Mat& img2, int y,
                          int minD, int maxD, CostType* cost,
                          PixType* buffer, const PixType* tab,
                          int tabOfs, int );
    void operator()(InputArray leftarr, InputArray rightarr, OutputArray disparr, StereoSGBM params);


private:
    void computeDisparitySGBM( const Mat& img1, const Mat& img2,
                               Mat& disp1, const StereoSGBM& params,
                               Mat& buffer );

    Mat buffer;
};

#endif // STEREOSGBM_HOG_H
