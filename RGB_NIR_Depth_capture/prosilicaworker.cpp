#include "prosilicaworker.h"

ProsilicaWorker::ProsilicaWorker() :
	ImgAcquisitionWorker(),
	vimbaCamManager(Vimba_Prosilica)
{
	vimbaCamManager.connectCameras();
}

ProsilicaWorker::~ProsilicaWorker()
{
	vimbaCamManager.closeCameras();
}

void ProsilicaWorker::startAcquisition()
{
	if(vimbaCamManager.prosilicaIsConnected())
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
}
