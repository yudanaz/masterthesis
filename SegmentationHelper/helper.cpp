#include "helper.h"

Helper::Helper()
{
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
