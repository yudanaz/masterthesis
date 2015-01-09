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
		RGBDNIR_MAP images;

		lock.lockForRead();

		stopped = false;
		do
		{
			//get RGB image and depth map from Kinect
			kinectCamManager.getImages(images);

			//forward images to main thread
			emit imagesReady(images);
		}
		while(acquiring);
		stopped = true;

		lock.unlock();
	}
}
