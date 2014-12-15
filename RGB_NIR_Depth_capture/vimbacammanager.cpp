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

QMap<RGBDNIR_captureType, Mat> VimbaCamManager::getCamImages()
{
	QMap<RGBDNIR_captureType, Mat> camImgs;
	Mat img;
	int i;

	if(detected_goldeye)
	{
		//get lock on flashlight usage:
		QMutexLocker locker(&flashlightLock);

		try
		{
			//trigger flashlight
			flashlight->triggerSeries();

			//get waveband images from camera (plus dark)
			for(i = 0; i < 5; ++i)
			{
				quint8 key = flashlight->getFrameKey();

				//acknowledge flashlight when last waveband is reached
				if(i == 4)
				{
					flashlight->sendAck();
				}

				img = goldeye->getCVFrame();


				//create correct entry for image list depending on channel returned by goldeye
				switch(key)
				{
					case 0: camImgs[NIR_Dark] = img;
					case 1: camImgs[NIR_935] = img;
					case 2: camImgs[NIR_1060] = img;
					case 3: camImgs[NIR_1300] = img;
					case 4: camImgs[NIR_1550] = img;
					default: break;
				}
			}
		}
		catch(CameraException e)
		{
			QMessageBox::information(NULL, "Goldeye: Camera Exception", e.getMessage(), QMessageBox::Ok);
		}
	}
	if(detected_prosilica)
	{
		try
		{
			prosilica->triggerViaSoftware();
			img = prosilica->getCVFrame();
			camImgs[RGB] = img;
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

QString VimbaCamManager::getRGBDNIR_captureTypeString(RGBDNIR_captureType i)
{
	switch (i) {
		case RGB: return "RGB";
		case Kinect_Depth: return "Kinect Depth Map";
		case Kinect_RGB: return "Kinect RGB Image";
		case NIR_Dark: return "NIR Dark image";
		case NIR_935: return "NIR 935 nm Waveband";
		case NIR_1060: return "NIR 1060 nm Waveband";
		case NIR_1300: return "NIR 1300 nm Waveband";
		case NIR_1550: return "NIR 1550 nm Waveband";
		default:
			break;
	}
}
