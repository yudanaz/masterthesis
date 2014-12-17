#include "imgacquisitionworker.h"

ImgAcquisitionWorker::ImgAcquisitionWorker(QObject *parent) :
	QThread(parent),
	acquiring(false)
{
	vimbaCamManager.detectCameras();
}

void ImgAcquisitionWorker::setStatus(bool acquiring)
{
	this->acquiring = acquiring;
}

void ImgAcquisitionWorker::startAcquisition()
{
	bool running = true;
	do
	{
		RGBDNIR_MAP images = vimbaCamManager.getCamImages();
		emit imagesReady(images);

		running = acquiring;
		//if any key is pressed, stop
//		if(waitKey(1) != -1){ return; }
	}
	while(running);
}

void ImgAcquisitionWorker::stopAcquisition()
{
	acquiring = false;
}
