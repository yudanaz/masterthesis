#include "imgacquisitionworker.h"

ImgAcquisitionWorker::ImgAcquisitionWorker(QObject *parent) :
	QObject(parent),
	acquiring(false)
{
	vimbaCamManager.detectCameras();
}

void ImgAcquisitionWorker::startAcquisition()
{
	acquiring = true;
	while(acquiring)
	{
		RGBDNIR_MAP images = vimbaCamManager.getCamImages();

		emit imagesReady(images);

		//if any key is pressed, stop
//		if(waitKey(1) != -1){ return; }
	}
}

void ImgAcquisitionWorker::stopAcquisition()
{
	acquiring = false;
}
