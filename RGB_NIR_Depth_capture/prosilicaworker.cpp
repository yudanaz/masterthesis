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
		lock.lockForRead();

		stopped = false;
		do
		{
			RGBDNIR_MAP images;
			//get RGB image from prosilica
			vimbaCamManager.getImages(images);

			//forward images to main thread
			emit imagesReady(images);

			usleep(211011);
		}
		while(acquiring);
		stopped = true;

		lock.unlock();
	}
}
