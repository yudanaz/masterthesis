#ifndef CAMMANAGER_H
#define CAMMANAGER_H

#include<QMap>
#include<opencv2/opencv.hpp>

using namespace cv;

enum RGBDNIR_captureType
{
    RGB, Kinect_Depth, Kinect_RGB, NIR_Dark, NIR_935, NIR_1060, NIR_1300, NIR_1550
};
typedef QMap<RGBDNIR_captureType, Mat> RGBDNIR_MAP;



class CamManager
{
public:
    virtual void connectCameras() = 0;
    virtual void closeCameras() = 0;
    virtual void getImages(QMap<RGBDNIR_captureType, Mat> &camImgs) = 0;
};

#endif // CAMMANAGER_H
