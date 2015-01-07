#include "imgacquisitionworker.h"

ImgAcquisitionWorker::ImgAcquisitionWorker() :
	acquiring(false),
	stopped(true)
{
	vimbaCamManager.connectCameras();
	kinectCamManager.connectCameras();
}

ImgAcquisitionWorker::~ImgAcquisitionWorker()
{

	vimbaCamManager.closeCameras();
	kinectCamManager.closeCameras();
}

void ImgAcquisitionWorker::setAcquiring(bool acquiring)
{
	this->acquiring = acquiring;
}

bool ImgAcquisitionWorker::isAcquiring()
{
	return this->acquiring;
}

void ImgAcquisitionWorker::stop()
{

}

bool ImgAcquisitionWorker::isStopped()
{
	return stopped;
}

void ImgAcquisitionWorker::startAcquisition()
{
	RGBDNIR_MAP images;

	lock.lockForRead();

	stopped = false;
	do
	{
		//get RGB and NIR images from vimba cameras
		vimbaCamManager.getImages(images);

		//get RGB image and depth map from Kinect
		kinectCamManager.getImages(images);

		//forward images to main thread
		emit imagesReady(images);
	}
	while(acquiring);
	stopped = true;

	lock.unlock();
}
