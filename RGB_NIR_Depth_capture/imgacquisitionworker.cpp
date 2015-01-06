#include<unistd.h>
#include "imgacquisitionworker.h"

ImgAcquisitionWorker::ImgAcquisitionWorker(QObject *parent) :
	QThread(parent),
    freenectDevice(freenect.createDevice<MyFreenectOpenCVDevice>(0)),
	acquiring(false)
{
	vimbaCamManager.connectCameras();
}

void ImgAcquisitionWorker::setStatus(bool acquiring)
{
	this->acquiring = acquiring;
}

void ImgAcquisitionWorker::startAcquisition()
{
	vimbaCamManager.startFlashlight();

    freenectDevice.startDepth();
    freenectDevice.startVideo();

    do
	{
        //get RGB and NIR images from vimba cameras
        RGBDNIR_MAP images;
        vimbaCamManager.getCamImages(images);

        //add Kinect Images
        Mat depthMat(Size(640,480),CV_16UC1);
        Mat depthMap8bit (Size(640,480),CV_8UC1);
        Mat rgbMat(Size(640,480),CV_8UC3,Scalar(0));
        //freenectDevice.getDepth(depthMat);
        freenectDevice.getVideo(rgbMat);
        //depthMat.convertTo(depthMap8bit, CV_8UC1, 255.0/2048.0);
        images[Kinect_RGB] = rgbMat;
        //images[Kinect_Depth] = depthMap8bit;

        //forward images to main thread
        emit imagesReady(images);
//		usleep(100000);
	}
	while(acquiring);
	vimbaCamManager.stopFlashlight();
    freenectDevice.stopDepth();
    freenectDevice.stopVideo();
}
