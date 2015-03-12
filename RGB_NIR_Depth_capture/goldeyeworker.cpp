#include "goldeyeworker.h"

GoldeyeWorker::GoldeyeWorker(int USBPortNr) :
	ImgAcquisitionWorker(),
	vimbaCamManager(Vimba_Goldeye)
{
	vimbaCamManager.setUSB(USBPortNr);
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

//			usleep(100000); //quick +/- fix for network packet collision -> alternates camera queries by simple timeout
		}
		while(acquiring);
		stopped = true;

		lock.unlock();
	}
}


void GoldeyeWorker::toggleMultiChannelCapture(bool captureMulti)
{
	vimbaCamManager.toggleMultichannelCapture(captureMulti);
}

