#include "../../include/caffe/rgbdnir_preproc.hpp"

RGBDNIR_preproc::RGBDNIR_preproc()
{
}



void RGBDNIR_preproc::normalizeZeroMeanUnitVariance(Mat &img)
{
    //calculate mean and standard deviation
    Scalar mean, stdDev;
    meanStdDev(img, mean, stdDev);

    //get variance
    float variance = cv::pow(stdDev[0], 2);

    //make zero mean and unit variance
     img = ((img - mean[0]) / variance);
}


void RGBDNIR_preproc::normalizeLocally(Mat &img, int localNbrhd)
{
    Mat img32F;
    img.convertTo(img32F, CV_32F);//, 0.003921569);
    int w = img32F.cols;
    int h = img32F.rows;

    //check from when on a smaller roi has to be used
    int x_rest = w % localNbrhd;
    int y_rest = h % localNbrhd;

    int x_limit = w - x_rest;
    int y_limit = h - y_rest;

    for (int y = 0; y < h; y += localNbrhd)
    {
        for (int x = 0; x < w; x += localNbrhd)
        {
            Rect roiRect(x, y, localNbrhd, localNbrhd);

            //padding with smaller roi
            if(x == x_limit){ roiRect.width = x_rest; }
            if(y == y_limit){ roiRect.height = y_rest; }

            Mat roi = img32F(roiRect);
            normalizeZeroMeanUnitVariance(roi);
        }
    }

//    for (int y = 0; y <= h - localNbrhd; y += localNbrhd)
//    {
//        for (int x = 0; x <= w - localNbrhd; x += localNbrhd)
//        {
//            Rect roiRect(x, y, localNbrhd, localNbrhd);
//            Mat roi = img32F(roiRect);
//            normalizeZeroMeanUnitVariance(roi);
//        }
//    }

    //convert back to 8 bit
    cv::normalize(img32F, img32F, 0.0, 1.0, NORM_MINMAX, -1);
    img32F.convertTo(img, CV_8U, 255);
}


void RGBDNIR_preproc::normalizeEachChannelLocally(Mat &img, int kernel)
{
    std::vector<Mat> chs(3);
    split(img, chs);
    for(int i = 0; i < 3; ++i)
    {
        normalizeLocally(chs[i], kernel);
//        normalizeLocally2(chs[i], kernel);
    }
    Mat img_;
    merge(chs, img_);
    img = img_;
}


void RGBDNIR_preproc::normalizeLocally2(Mat &img, int kernel)
{
    Mat img32F, mean, stdDev;

    //convert to float image
    img.convertTo(img32F, CV_32F);//, 0.003921569); // 1/255 = 0.003921569

    //estimate image mean with gaussian blur
    GaussianBlur(img32F, mean, Size(kernel, kernel), 0);
    img32F = img32F - mean;

    //estimate standard deviation with gaussian blur by doing sqrt( gauss_blur(img²) )
    GaussianBlur(img32F.mul(img32F), mean, Size(kernel*81, kernel*81), 0); //re-use mean matrix
    cv::pow(mean, 0.5, stdDev);
    img32F = img32F / stdDev;

    //convert back to 8 bit
    cv::normalize(img32F, img32F, 0, 1, NORM_MINMAX, -1);
    img32F.convertTo(img, CV_8U, 255);
}


vector<Mat> RGBDNIR_preproc::makePyramid(Mat img, int leveln, int interpolMethod)
{
    vector<Mat> levels;
    Mat procImg;

    levels.push_back(img);
    for(int i = 1; i < leveln; ++i)
    {
        resize(img, procImg, Size((img.cols+1)/2, (img.rows+1)/2), 0, 0, interpolMethod);
        levels.push_back(procImg.clone());
        img = procImg.clone();
    }
    return levels;
}


vector<Mat> RGBDNIR_preproc::makeGaussianPyramid(Mat img, int leveln)
{
    vector<Mat> levels;
    Mat procImg;

    levels.push_back(img);
    for(int i = 1; i < leveln; ++i)
    {
        pyrDown(img, procImg);
        levels.push_back(procImg.clone());
        img = procImg.clone();
    }
    return levels;
}



vector<Mat> RGBDNIR_preproc::makeLaplacianPyramid(Mat img, int leveln)
{
    vector<Mat> levels;
    Mat next_img, img1;

    for(int i = 0; i < leveln-1; ++i)
    {
        pyrDown(img, next_img);
        pyrUp(next_img, img1, img.size());
        levels.push_back(img - img1);
        img = next_img;
    }
    levels.push_back(img);
    return levels;
}
