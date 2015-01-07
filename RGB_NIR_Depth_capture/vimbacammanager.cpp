#include "vimbacammanager.h"

void MyImageSource::doWhiteCalib(Mat m, int index)
{
	Mat temp;
	switch(index)
	{
		case 0: return; //dark image
		case 1: m.convertTo(temp, -1, 3.0);
		case 2: m.convertTo(temp, -1, 1.0);
		case 3: m.convertTo(temp, -1, 1.105);
		case 4: m.convertTo(temp, -1, 1.13);
	}
	m = temp.clone();
}

VimbaCamManager::VimbaCamManager():
	vimbaSystem(VimbaSystem::GetInstance()),
	APIrunning(false),
	connected_prosilica(false),
	connected_goldeye(false),
	connected_flashlight(false),
	flashLightRunning(false),
	maxFPS(30), FramePause(33333)
{
	startVimbaAPI();
	flashlight = new FlashlightControl();

	myImageSource.loadFPNRef();
}

VimbaCamManager::~VimbaCamManager()
{
	//closeCameras();
	vimbaSystem.Shutdown();
}

void VimbaCamManager::startVimbaAPI()
{
	APIrunning = (VmbErrorSuccess == vimbaSystem.Startup());
}

void VimbaCamManager::connectCameras()
{
	//connect to goldeye
	if(!connected_goldeye)
	{
		goldeye = new GoldeyeVimba();
		try
		{
			goldeye->connect();
			goldeye->configure();//camConfig.integrationTime, camConfig.bufferSize);
			connected_goldeye = goldeye->isConnected() && goldeye->isConfigured();
		}
		catch(CameraException e)
		{
			QMessageBox::information(NULL, "Goldeye: Camera Exception", e.getMessage(), QMessageBox::Ok);
		}
	}

	//if goldeye was detected, try opening and configuring the flashlight
	if(connected_goldeye && !connected_flashlight)
	{
		try
		{
			flashlight->connect("/dev/ttyUSB1");
			connected_flashlight = true;


			/****************************************************************************************************
			 *  The next section is an altered version of the setConfig() / configureFlashlight() and calcWavebandsValue()
			 *  methods in the skincam cameraSystem class. Normally the values are read out of a config file,
			 *  but for the sake of simplicity they're hard-coded here.
			 * **************************************************************************************************/
			//warning: the ringlight needs integration time in 1/10ths of a ms as Integer!
			quint16 integrationTime = 10;
			quint16 intVal = (quint16)(integrationTime * 10);
			flashlight->sendCommand(QString("P%1;").arg(intVal), true); //integration time
			goldeye->setExposureTime(integrationTime);

			maxFPS = 25;//goldeye->getMaxFPS();
			FramePause = 1000000 / maxFPS; //1Mio us / fps
			flashlight->sendCommand(QString("F%1;").arg(maxFPS), true); //frequency
			qDebug() << "fps for Goldeye: " << maxFPS << "overall fps: " << (maxFPS/4);

			quint8 value = 0;
			//calculate waveband value: binary representation, 1 = enabled, from left
			QList<bool> bands;// = myConfig.wavebands.config.values();
			bands << true << true << true << true;
			for (quint8 i = 0; i < 4 /*myConfig.wavebands.totalNumber*/; i++)
			{
				if(bands.at(i))
					value |= 1<<(4 /*myConfig.wavebands.totalNumber*/ - (i+1));
			}
			flashlight->sendCommand(QString("W%1;").arg(value), true);
			/*** endof hardcoded section ************************************************************************/

			startFlashlight();
		}
		catch(FlashlightException e)
		{
			QMessageBox::information(NULL, "Goldeye: Flashlight Exception", e.getMessage(), QMessageBox::Ok);
		}
	}

	//connect to prosilica
	if(!connected_prosilica)
	{
		prosilica = new ProsilicaVimba();
		try
		{
			prosilica->connect();
			prosilica->configure();
			connected_prosilica = prosilica->isConnected() && prosilica->isConfigured();
		}
		catch(CameraException e)
		{
			QMessageBox::information(NULL, "Prosilica: Camera Exception", e.getMessage(), QMessageBox::Ok);
		}
	}
}

void VimbaCamManager::startFlashlight()
{
	if(!connected_flashlight){ return; }
	if(!flashLightRunning)
	{
		flashlight->run();
		flashLightRunning = true;
		goldeye->reset();
	}
}

void VimbaCamManager::stopFlashlight()
{
	if(!connected_flashlight){ return; }
	if(flashLightRunning)
	{
		flashlight->halt();
		flashLightRunning = false;
		goldeye->reset();
	}
}

void VimbaCamManager::getImages(QMap<RGBDNIR_captureType, Mat> &camImgs)
{
	Mat img;
	quint8 i;
	quint8 errorCnt = 0;

	if(connected_prosilica)
	{
		try
		{
			prosilica->triggerViaSoftware();
			img = prosilica->getCVFrame();
			camImgs[RGB] = img;
		}
		catch(CameraException e)
		{
			qDebug() << "Prosilica: Camera Exception:  "<< e.getMessage();
			//QMessageBox::information(NULL, "Prosilica: Camera Exception", e.getMessage(), QMessageBox::Ok);
		}

	}

	if(connected_goldeye & connected_flashlight)
	{
		//get lock on flashlight usage:
		QMutexLocker locker(&flashlightLock);

		try
		{
			//trigger flashlight
//			flashlight->triggerSeries();

			bool darkImageSaved = false;

			//get waveband images from camera (plus dark)
			for(i = 0; i < 5; i++)
			{
				//get current channel - this waits for the flashlight!
//				flashlight->triggerBand(i);
				quint8 key = flashlight->getFrameKey();

//				qDebug() << "index: " << i << "frameKey: " << key;

				//acknowledge flashlight when last waveband is reached
				if(i == 4)
				{
					flashlight->sendAck();
				}

				//get next frame from camera
				img = goldeye->getCVFrame();

				//return if image empty to avoid runtime errors
				if(img.rows == 0 || img.cols == 0){ continue; }

				//apply fixed pattern noise calibration
				myImageSource.doFPNCalib(img);

				//if waveband, subtract dark image to get "pure" waveband image
				//and do white calibration

				if(key == 0){ darkImageSaved = true; }
				else
				{
					if(darkImageSaved)
					{
						cv::subtract(img, camImgs[NIR_Dark], img);
					}
					myImageSource.doWhiteCalib(img, key);
				}

				//create correct entry for image list depending on channel returned by goldeye
				switch(key)
				{
					case 0: camImgs[NIR_Dark] = img; break;
					case 1: camImgs[NIR_935] = img;  break;
					case 2: camImgs[NIR_1060] = img; break;
					case 3: camImgs[NIR_1300] = img; break;
					case 4: camImgs[NIR_1550] = img; break;
					default: break;
				}
			}
		}
		catch(CameraException e)
		{
			//handle problem
			if(flashLightRunning)
			{
				flashlight->halt();
			}

			qDebug() << "Goldeye acquisition (Frame " << i << "):" << e.getMessage();

			goldeye->reset();

			if(flashLightRunning)
			{
				flashlight->run();
			}

			//only show error if too many errors occur
			if(errorCnt++ > 3)
			{
				QMessageBox::information(NULL, "Goldeye: Camera Exception ("
										 + QString::number(i) + "):", e.getMessage(), QMessageBox::Ok);
			}
		}
		catch (SkinCamException e)
		{
			if(flashLightRunning)
			{
				flashlight->halt();
			}
			QMessageBox::information(NULL, "Goldeye: Error during frame acquisition ("
									 + QString::number(i) + "):", e.getMessage(), QMessageBox::Ok);
		}
		catch (...)
		{
			if(flashLightRunning)
			{
				flashlight->halt();
			}
			QMessageBox::information(NULL, "Goldeye: Unknown error",
									 "Unknown error during frame acquisition", QMessageBox::Ok);
		}
	}
}

void VimbaCamManager::closeCameras()
{
	if(goldeye->isConnected()){ goldeye->disconnect(); }
	if(flashLightRunning){ stopFlashlight(); }
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
