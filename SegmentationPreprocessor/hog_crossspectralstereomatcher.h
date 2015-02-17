#ifndef HOG_CROSSSPECTRALSTEREOMATCHER_H
#define HOG_CROSSSPECTRALSTEREOMATCHER_H

#include "crossspectralstereomatcher.h"
#include "opencv2/opencv.hpp"

typedef uchar PixType;
typedef short CostType;
typedef short DispType;

enum { NR = 16, NR2 = NR/2 };

enum { dispOpt_WTA, dispOpt_SGBM };

using namespace std;
using namespace cv;

class HOG_crossSpectralStereoMatcher : public CrossSpectralStereoMatcher
{
public:
    HOG_crossSpectralStereoMatcher();

    virtual void process(Mat imgRGB_L, Mat imgNIR_R, Mat &out_disp);

    void set_disparity_optimization_strategy(int strategy);

    static void setParams(int disparityRange);

private:
    Mat makeDisparity_WTA(vector<float>& values_L, vector<float>& values_R, Size imgSize, int blockSize, int nCells, int nBins);

    float getL1Distance(vector<float>& a, int index_a, vector<float>& b, int index_b, int binsInDescriptor);

    int disparityOptimimizationStragey;

    static int dispRange;

    //modified SGBM:
    void compute_SGBM_HOG(InputArray leftarr, InputArray rightarr, OutputArray disparr, StereoSGBM params, vector<float> &HOGdesriptors_L, vector<float> &HOGdescriptors_R, int blockSize, int nCells, int nBins);

    void computeDisparitySGBM(const Mat& img1, const Mat& img2,
                               Mat& disp1, const StereoSGBM& params,
                               Mat& buffer , vector<float> &HOGdescr_L, vector<float> &HOGdescr_R, int blockSize, int binsInDescriptor);

    void calcPixelCostHOG(vector<float>& descr_L, vector<float>& descr_R, int blockSize, int y,
                          int imgWidth, int imgHeight, int disparityRange, int binsInDescriptor, CostType* cost);

    void calcPixelCostBT( const Mat& img1, const Mat& img2, int y,
                          int minD, int maxD, CostType* cost,
                          PixType* buffer, const PixType* tab,
                          int tabOfs, int );

    Mat SGBM_buffer;

};

#endif // HOG_CROSSSPECTRALSTEREOMATCHER_H
