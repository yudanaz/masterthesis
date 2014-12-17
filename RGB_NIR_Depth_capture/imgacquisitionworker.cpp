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
	do
	{
		RGBDNIR_MAP images = vimbaCamManager.getCamImages();
		emit imagesReady(images);
	}
	while(acquiring);
}

void ImgAcquisitionWorker::stopAcquisition()
{
	acquiring = false;
}
