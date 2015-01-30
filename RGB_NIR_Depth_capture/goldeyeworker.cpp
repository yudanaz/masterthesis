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
	if(vimbaCamManager.goldeyeIsConnected())
	{
		lock.lockForRead();

		stopped = false;
		do
		{
			RGBDNIR_MAP images;
			//get NIR image from goldeye
			vimbaCamManager.getImages(images);

			//forward images to main thread
			emit imagesReady(images);

			usleep(100000);
		}
		while(acquiring);
		stopped = true;

		lock.unlock();
	}
}
