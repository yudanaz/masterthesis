#include "vimbacammanager.h"


VimbaCamManager::VimbaCamManager():
	vimbaSystem(VimbaSystem::GetInstance()),
	APIrunning(false),
	detected_prosilica(false),
	detected_goldeye(false)
{
	startVimbaAPI();
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
	if(!detected_goldeye)
	{
		goldeye = new GoldeyeVimba();
		try
		{
			goldeye->connect();
			goldeye->configure();//camConfig.integrationTime, camConfig.bufferSize);
			detected_goldeye = goldeye->isConnected() && goldeye->isConfigured();
		}
		catch(CameraException e)
		{
			QMessageBox::information(NULL, "Goldeye: Camera Exception", e.getMessage(), QMessageBox::Ok);
		}
	}

	if(!detected_prosilica)
	{
		prosilica = new ProsilicaVimba();
		try
		{
			prosilica->connect();
			prosilica->configure();
			detected_prosilica = prosilica->isConnected() && prosilica->isConfigured();
		}
		catch(CameraException e)
		{
			QMessageBox::information(NULL, "Prosilica: Camera Exception", e.getMessage(), QMessageBox::Ok);
		}
	}
}

QList<Mat> VimbaCamManager::getCamImages()
{
	QList<Mat> camImgs;

	bool atLeastOneCam = false;
	Mat img;
	if(detected_goldeye)
	{
		img = goldeye->getCVFrame();
		atLeastOneCam = true;
	}
	if(detected_prosilica)
	{
		img = prosilica->getCVFrame();
		atLeastOneCam = true;
	}

	if(atLeastOneCam){ camImgs.append(img); }
	return camImgs;
}

void VimbaCamManager::closeCameras()
{
	if(goldeye->isConnected()){ goldeye->disconnect(); }
	if(prosilica->isConnected()){ prosilica->disconnect(); }
}
