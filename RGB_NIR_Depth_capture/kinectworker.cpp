#include "kinectworker.h"

KinectWorker::KinectWorker() :
	ImgAcquisitionWorker()
{
	kinectCamManager.connectCameras();
}

KinectWorker::~KinectWorker()
{
	kinectCamManager.closeCameras();
}


void KinectWorker::startAcquisition()
{
	if(kinectCamManager.isConnected())
	{
		lock.lockForRead();

		stopped = false;
		do
		{
			//get RGB image and depth map from Kinect
			RGBDNIR_MAP images;
			kinectCamManager.getImages(images);

			//forward images to main thread
			emit imagesReady(images);
		}
		while(acquiring);
		stopped = true;

		lock.unlock();
	}
}


void KinectWorker::triggerIRcapture()
{
	kinectCamManager.triggerIRcapture();
}

void KinectWorker::switch_RGB_IR(bool captureRGB)
{
	kinectCamManager.switch_RGB_IR(captureRGB);
}

bool KinectWorker::isCapturingRGB()
{
	return kinectCamManager.isCapturingRGB();
}
