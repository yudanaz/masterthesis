#include "vimbacammanager.h"


VimbaCamManager::VimbaCamManager():
	vimbaSystem(VimbaSystem::GetInstance()),
	APIrunning(false),
	camsDetected(false)
{
	startVimbaAPI();
	detectCameras();
}

VimbaCamManager::~VimbaCamManager()
{
	closeCameras();
	vimbaSystem.Shutdown();
}

void VimbaCamManager::startVimbaAPI()
{
	APIrunning = (VmbErrorSuccess == vimbaSystem.Startup());
}

void VimbaCamManager::detectCameras()
{
	if(camsDetected){ return; }

//	goldeye = new GoldeyeVimba();
//	goldeye->connect();
//	goldeye->configure();//camConfig.integrationTime, camConfig.bufferSize);

	prosilica = new ProsilicaVimba();
	try
	{
		prosilica->connect();
		prosilica->configure();
	}
	catch(CameraException e)
	{
		QMessageBox::information(NULL, "Camera Exception", e.getMessage(), QMessageBox::Ok);
	}

	camsDetected = true;
}

QList<Mat> VimbaCamManager::getCamImages()
{
	QList<Mat> camImgs;

	Mat img;
//	img = goldeye->getCVFrame();
	img = prosilica->getCVFrame();
	camImgs.append(img);



	return camImgs;
}

void VimbaCamManager::closeCameras()
{
//	goldeye->disconnect();
}
