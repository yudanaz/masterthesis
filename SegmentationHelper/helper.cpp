#include "helper.h"

Helper::Helper()
{
}


Mat Helper::NormalizeLocally(Mat image1Channel, int neighborhoodSize)
{
    Mat floatImg, mean, stdDev, out;

    //convert to float image
    image1Channel.convertTo(floatImg, CV_32F, 0.00390625); // 1/256 = 0.00390625

    //estimate image mean with gaussian blur
    GaussianBlur(floatImg, mean, Size(neighborhoodSize, neighborhoodSize), 0);
    out = floatImg - mean;

    //estimate standard deviation with gaussian blur by doing sqrt( gauss_blur(img²) )
    GaussianBlur(out.mul(out), mean, Size(neighborhoodSize, neighborhoodSize), 0); //re-use mean matrix
    cv::pow(mean, 0.5, stdDev);
    out = out / stdDev;

    return out;
}


void Helper::Print1ChMatrixToConsole(Mat mat)
{
    QString s;
    int type = mat.type();
    for (int y = 0; y < mat.rows; ++y)
    {
        s = "";
        for (int x = 0; x < mat.cols; ++x)
        {
            switch(type)
            {
            case CV_8UC1:
                s += QString::number(mat.at<uchar>(y,x)) + "\t";
                break;
            case CV_16UC1:
                s += QString::number(mat.at<ushort>(y,x)) + "\t";
                break;
            case CV_32FC1:
                s += QString::number(mat.at<float>(y,x)) + "\t";
                break;
            case CV_64FC1:
                s += QString::number(mat.at<double>(y,x)) + "\t";
                break;
            default:
                qDebug() << "matrix format not supported";
                return;
            }
        }
        qDebug() << s;
    }
    qDebug() << "===";
}
