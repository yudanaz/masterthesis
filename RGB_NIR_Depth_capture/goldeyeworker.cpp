#include "goldeyeworker.h"

GoldeyeWorker::GoldeyeWorker() :
	ImgAcquisitionWorker(),
	vimbaCamManager(Vimba_Goldeye)
{
	vimbaCamManager.connectCameras();
}

GoldeyeWorker::~GoldeyeWorker()
{
	vimbaCamManager.closeCameras();
}

void GoldeyeWorker::startAcquisition()
{
	RGBDNIR_MAP images;

	lock.lockForRead();

	stopped = false;
	do
	{
		//get RGB and NIR images from vimba cameras
		vimbaCamManager.getImages(images);

		//forward images to main thread
		emit imagesReady(images);
	}
	while(acquiring);
	stopped = true;

	lock.unlock();
}
