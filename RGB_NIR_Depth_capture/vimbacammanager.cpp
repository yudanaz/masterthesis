#include "vimbacammanager.h"


VimbaCamManager::VimbaCamManager():
	vimbaSystem(VimbaSystem::GetInstance()),
	APIrunning(false),
	detected_prosilica(false),
	detected_goldeye(false)
{
	startVimbaAPI();
	flashlight = new FlashlightControl();
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
		flashlight->connect("/dev/ttyUSB1");

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
	Mat img;
	int i;

	if(detected_goldeye)
	{
		try
		{
			//trigger flashlight
			flashlight->triggerSeries();

			//get waveband images from camera (plus dark)
			for(i = 0; i < 5; ++i)
			{
				flashlight->getFrameKey();

				//acknowledge flashlight when last waveband is reached
				if(i == 4)
				{
					flashlight->sendAck();
				}

				img = goldeye->getCVFrame();
				camImgs.append(img);
			}
		}
		catch(CameraException e)
		{
			QMessageBox::information(NULL, "Prosilica: Camera Exception", e.getMessage(), QMessageBox::Ok);
		}
	}
	if(detected_prosilica)
	{
		try
		{
			prosilica->triggerViaSoftware();
			img = prosilica->getCVFrame();
			camImgs.append(img);
		}
		catch(CameraException e)
		{
			QMessageBox::information(NULL, "Prosilica: Camera Exception", e.getMessage(), QMessageBox::Ok);
		}
	}
	return camImgs;
}

void VimbaCamManager::closeCameras()
{
	if(goldeye->isConnected()){ goldeye->disconnect(); }
	if(prosilica->isConnected()){ prosilica->disconnect(); }
}
