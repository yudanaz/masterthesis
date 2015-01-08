#include "prosilicaworker.h"

ProsilicaWorker::ProsilicaWorker() :
	ImgAcquisitionWorker(),
	vimbaCamManager(Vimba_Prosilica)
{
	vimbaCamManager.connectCameras();
	kinectCamManager.connectCameras();
}

ProsilicaWorker::~ProsilicaWorker()
{
	vimbaCamManager.closeCameras();
	kinectCamManager.closeCameras();
}

void ProsilicaWorker::startAcquisition()
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
