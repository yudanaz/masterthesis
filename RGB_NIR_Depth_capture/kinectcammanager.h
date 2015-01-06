#ifndef KINECTCAMMANAGER_H
#define KINECTCAMMANAGER_H

#include"cammanager.h"
#include"libfreenect.hpp"
#include"myfreenectopencvdevice.h"

class KinectCamManager : public CamManager
{
public:
    KinectCamManager();

    void connectCameras();
    void closeCameras();
    void getImages(QMap<RGBDNIR_captureType, Mat> &camImgs);

private:
    Freenect::Freenect freenect;
    MyFreenectOpenCVDevice& freenectDevice;
};

#endif // KINECTCAMMANAGER_H
