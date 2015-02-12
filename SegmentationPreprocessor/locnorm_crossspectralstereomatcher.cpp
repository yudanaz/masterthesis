#include "locnorm_crossspectralstereomatcher.h"

LocNorm_crossSpectralStereoMatcher::LocNorm_crossSpectralStereoMatcher()
{
}

void LocNorm_crossSpectralStereoMatcher::process(Mat imgRGB_L, Mat imgNIR_R, Mat& out_disp)
{
    Mat rgb_norm = NormalizeLocally(imgRGB_L, 11, 171);
    Mat nir_norm = NormalizeLocally(imgNIR_R, 11, 171);
//    Mat rgb_norm = rgb_gray;
//    Mat nir_norm = imgNIR_R;

    //try to make both image the same brightness
    double rgb_mean = mean(rgb_norm)[0];
    double nir_mean = mean(nir_norm)[0];
    double multi = rgb_mean > nir_mean ? rgb_mean / nir_mean : nir_mean / rgb_mean;
    if(rgb_mean > nir_mean)
        { nir_norm *= multi; }
    else
        { rgb_norm *= multi; }

    imshow("rgb norm", rgb_norm);
    imshow("nir norm", nir_norm);
    imwrite("rgb norm.png", rgb_norm);
    imwrite("nir norm.png", nir_norm);

    Mat rgb_sobel, nir_sobel;
    Sobel(rgb_norm, rgb_sobel, CV_16U, 1, 1, 7);
    Sobel(nir_norm, nir_sobel, CV_16U, 1, 1, 7);

    //normalize
    Mat rgb_sobelNorm, nir_sobelNorm;
    double min, max;
    minMaxLoc(rgb_sobel, &min, &max);
    rgb_sobel -= min;
    rgb_sobel.convertTo(rgb_sobelNorm, CV_8U, 255/(max-min));
    minMaxLoc(nir_sobel, &min, &max);
    nir_sobel -= min;
    nir_sobel.convertTo(nir_sobelNorm, CV_8U, 255/(max-min));

    imshow("rgb sobel", rgb_sobelNorm);
    imshow("nir sobel", nir_sobelNorm);
    imwrite("rgb sobel.png", rgb_sobelNorm);
    imwrite("nir sobel.png", nir_sobelNorm);
}

Mat LocNorm_crossSpectralStereoMatcher::NormalizeLocally(Mat img, int meanKernel, int stdDevKernel, bool outputAs8bit)
{
    Mat floatImg, mean, stdDev, out;

    //convert to float image
    img.convertTo(floatImg, CV_32F);//, 0.003921569); // 1/255 = 0.003921569

    //estimate image mean with gaussian blur
    GaussianBlur(floatImg, mean, Size(meanKernel, meanKernel), 0);
    floatImg = floatImg - mean;

    //estimate standard deviation with gaussian blur by doing sqrt( gauss_blur(img²) )
    GaussianBlur(floatImg.mul(floatImg), mean, Size(stdDevKernel, stdDevKernel), 0); //re-use mean matrix
    cv::pow(mean, 0.5, stdDev);
    floatImg = floatImg / stdDev;

    if(outputAs8bit)
    {
        //cast back to [0, 255] interval, so it can be saved as a JPG image (or other lossy compression)
        cv::normalize(floatImg, floatImg, 0, 1, NORM_MINMAX, -1);
        floatImg.convertTo(out, CV_8U, 255);
        return out;
    }
    else
    {
        return floatImg;
    }
}
