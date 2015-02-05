#include "crossbilateralfilterwrapper.h"

CrossBilateralFilterWRAPPER::CrossBilateralFilterWRAPPER()
{
}


Mat CrossBilateralFilterWRAPPER::filter(Mat srcGray, Mat edgesGray, float sigma_s, float sigma_r)
{
    //read images [!] have to be grayscale!!
    image_type src = getInternalImageType(srcGray);
    image_type edg = getInternalImageType(edgesGray);
    image_type flt(srcGray.cols, srcGray.rows);
    Image_filter::fast_LBF(src, edg, sigma_s, sigma_r, false, &flt, &flt);
    return getMat(flt);
}


image_type CrossBilateralFilterWRAPPER::getInternalImageType(Mat imgGray)
{
    uint h = imgGray.rows;
    uint w = imgGray.cols;
    image_type outImg(w, h);

    for (int y = 0; y < h; ++y)
    {
        for (int x = 0; x < w; ++x)
        {
            outImg(x,y) = 1.0 * imgGray.at<uchar>(y,x) / 255.0;
        }
    }
    return outImg;
}

Mat CrossBilateralFilterWRAPPER::getMat(image_type img)
{
    uint w = img.width();
    uint h = img.height();
    Mat outImg(h, w, CV_8UC1);

    for (int y = 0; y < h; ++y)
    {
        for (int x = 0; x < w; ++x)
        {
            outImg.at<uchar>(y,x) = img(x,y) * 255.0;
        }
    }
    return outImg;
}
