#include<unistd.h>
#include "imgacquisitionworker.h"

ImgAcquisitionWorker::ImgAcquisitionWorker(QObject *parent) :
	QThread(parent),
	acquiring(false)
{
	vimbaCamManager.connectCameras();
}

void ImgAcquisitionWorker::setStatus(bool acquiring)
{
	this->acquiring = acquiring;
}

void ImgAcquisitionWorker::startAcquisition()
{
	vimbaCamManager.startFlashlight();
	do
	{
		RGBDNIR_MAP images = vimbaCamManager.getCamImages();
		emit imagesReady(images);
//		usleep(100000);
	}
	while(acquiring);
	vimbaCamManager.stopFlashlight();
}
