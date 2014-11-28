#include "camerasystem.h"

CameraSystem::CameraSystem()
{
	#ifdef AVT_PLEORA
	myCamera = new GoldeyePleora();
	#endif
	#ifdef AVT_VIMBA
	myCamera = new GoldeyeVimba();
	#endif

	myFlashlight = new FlashlightControl();
	myConfig = CameraSystemConfig(myFlashlight->getBands());
	isOpen = false;
	isRunning = false;

	//set default white calibration
	QList<qint16> bands =  myFlashlight->getBands();
	for(quint8 i = 0; i < bands.size(); i++)
	{
		myCalSettings.whiteCal.insert( bands.at(i), 1.0 );
	}
}

CameraSystem::~CameraSystem()
{
	delete myCamera;
	delete myFlashlight;
}

void CameraSystem::openSource(QString source)
{
	//get lock on flashlight usage:
	QMutexLocker locker(&flashlightLock);

	if (source.isEmpty())
		source = myConfig.ringlightInterface;

	try
	{
		//connect flashlight on given com-port
		myFlashlight->connect(source);

		//connect camera using auto-search or given IP
		if(myConfig.autoCameraSearch)
		{
			myCamera->connect();
		}
		else
		{
			myCamera->connect(myConfig.IPadress);
		}
		isOpen = true;

		//configure camera parameters
		myCamera->configure(myConfig.integrationTime, myConfig.bufferSize);

		//check if pre-configured framerate is valid and cut off if necessary:
		quint8 maxFPS = myCamera->getMaxFPS();
		if(myConfig.frequency > maxFPS)
		{
			myConfig.frequency = maxFPS;
		}

		//configure flashlight with these settings
		configureFlashlight();

		myFPS = (myConfig.frequency / myConfig.wavebands.totalNumber);

		initializeFrame();
	}
	catch (FlashlightException e)
	{
		throw ImageSourceException(e.getMessage());
	}
	catch (CameraException e)
	{
		throw ImageSourceException(e.getMessage());
	}

	mySourceName = "SkinCam";
}

MultispectralImage CameraSystem::acquireNewFrame()
{
	quint8 i;
	quint8 curCh;
	Mat newImg;

	//get lock on flashlight usage:
	QMutexLocker locker(&flashlightLock);

	try
	{
		//if the camera system is not autonomously running right now,
		//trigger the flashlight to start one round of images
		if (!isRunning)
		{
			myFlashlight->triggerSeries();
		}

		//acquire the required number of channel images (plus dark!)
		for(i = 0; i < myConfig.wavebands.usedNumber + 1; i++)
		{
			//get current channel - this waits for the flashlight!
			curCh = myFlashlight->getFrameKey();

			//acknowledge if this is the last channel for this image
			//#### IF there are sync-problems, it might be a good idea
			// to put this after "get next frame"
			if(isRunning && (i == myConfig.wavebands.usedNumber))
			{
				myFlashlight->sendAck();
			}

			//get next image from camera
			newImg = myCamera->getCVFrame();

			//apply fixed pattern noise calibration if enabled
			if(myCalSettings.applyFPNCal)
			{
				applyFPNCalibration(newImg);
			}

			if(curCh == 0)
			{
				//set image as dark channel (data is not copied!)
				myFrame.setChannelImage(0, newImg);
			}
			else
			{
				//subtract dark channel if enabled
				if (myCalSettings.subtractDark)
				{
					cv::subtract(newImg, myFrame.getDarkImage(), newImg);
				}

				//set image as one channel image in the frame (data is not copied!)
				myFrame.setChannelImage(myFlashlight->getBands().at(curCh-1), newImg);
			}
		}

		//reset camera, clearing all buffers to avoid sync problems - DO NOT DO WITH VIMBA
		#ifdef AVT_PLEORA
		myCamera->reset();
		#endif

		//note if frame is FPN corrected
		myFrame.setFPNCorrected(myCalSettings.applyFPNCal);

		//note if frame is dark subtracted
		myFrame.setDarkSubtracted(myCalSettings.subtractDark);

		//apply white calibration if enabled
		if(myCalSettings.applyWhiteCal)
		{
			applyWhiteCalibration();
		}
		else
		{
			myFrame.setWhiteCalibrated(false);
		}
	}
	catch (CameraException e)
	{
		if(isRunning)
		{
			myFlashlight->halt();
		}

		qDebug() << "Acquisition:" << e.getMessage();

		myCamera->reset();

		if(isRunning)
		{
			myFlashlight->run();
		}

		throw ImageSourceException(QString("Error during frame acquisition: %1")
								   .arg(e.getMessage()));
	}
	catch (SkinCamException e)
	{
		if(isRunning)
		{
			myFlashlight->halt();
		}
		throw ImageSourceException(QString("Error during frame acquisition: %1")
								   .arg(e.getMessage()));
	}
	catch (...)
	{
		if(isRunning)
		{
			myFlashlight->halt();
		}
		throw ImageSourceException("Unknown error during frame acquisition");
	}

	return myFrame;
}

void CameraSystem::initializeFrame()
{
	//set up multispectral image object
	myFrame.setSize(myCamera->getImgWidth(), myCamera->getImgHeight());
	myFrame.setDepth(myCamera->getImgDepth());
	myFrame.setDarkSubtracted(false);
	myFrame.setFPNCorrected(false);
	myFrame.setWhiteCalibrated(false);

	//add a dark image for every enabled channel
	Mat dark = Mat::zeros(myCamera->getImgHeight(), myCamera->getImgWidth(), CV_16U);
	QList<qint16> channels = myConfig.wavebands.config.keys();
	myFrame.setChannelImage(0, dark);
	for(quint8 i = 0; i < myConfig.wavebands.totalNumber; i++)
	{
		if(myConfig.wavebands.config.value(channels.at(i)))
			myFrame.setChannelImage(channels.at(i), dark);
	}
}

void CameraSystem::run(bool doRun)
{
	//get lock on flashlight usage:
	QMutexLocker locker(&flashlightLock);

	if(doRun && !isRunning)
	{
		//reset camera to clean buffers:
		//myCamera->reset();

		try
		{
			myFlashlight->run();
		}
		catch (FlashlightException e)
		{
			qDebug() << "Flashlight Exception on Start: " << e.getMessage();
		}
	}
	else if (!doRun && isRunning)
	{
		try
		{
			myFlashlight->halt();
		}
		catch (FlashlightException e)
		{
			qDebug() << "Flashlight Exception on Stop: " << e.getMessage();
		}

		//reset camera to clean buffers:
		myCamera->reset();
	}

	isRunning = doRun;
}

CameraSystemConfig CameraSystem::getConfig()
{
	return myConfig;
}

void CameraSystem::setConfig(CameraSystemConfig newConfig)
{
	//get lock on flashlight usage:
	QMutexLocker locker(&flashlightLock);

	//check if settings are valid
	if((newConfig.integrationTime < 0.1)
			|| (newConfig.integrationTime > 50.0))
	{
		throw ImageSourceException("Invalid integration time!");
	}
	if((newConfig.frequency > 100))
	{
		throw ImageSourceException("Invalid frequency!");
	}

	//if the camera/flashlight is not currently connected, simply apply new config
	if(!isOpen)
	{
		myConfig = newConfig;
	}
	//otherwise, just update changed values
	else
	{
		if (myConfig.integrationTime != newConfig.integrationTime)
		{
			try
			{
				//warning: the ringlight needs integration time in 1/10ths of a ms as Integer!
				quint16 intVal = (quint16)(myConfig.integrationTime*10);
				myFlashlight->sendCommand(QString("P%1;").arg(intVal), true);

				myCamera->setExposureTime(newConfig.integrationTime);

				myConfig.integrationTime = newConfig.integrationTime;
			}
			catch (FlashlightException e)
			{
				throw ImageSourceException(QString("Error configuring flashlight: %1")
										   .arg(e.getMessage()));
			}
			catch (CameraException e)
			{
				throw ImageSourceException(QString("Error configuring camera: %1")
										   .arg(e.getMessage()));
			}
		}

		if (myConfig.frequency != newConfig.frequency)
		{
			//check for maximum framerate given current integration time
			// and limit configured framerate to maximum:
			myConfig.maxFPS = myCamera->getMaxFPS();
			if(myConfig.maxFPS < newConfig.frequency)
			{
				newConfig.frequency = myConfig.maxFPS;
			}

			try
			{
				myFlashlight->sendCommand(QString("F%1;").arg(newConfig.frequency), true);

				myConfig.frequency = newConfig.frequency;

				myFPS = (myConfig.frequency / myConfig.wavebands.totalNumber);
			}
			catch (FlashlightException e)
			{
				throw ImageSourceException(QString("Error configuring flashlight: %1")
										   .arg(e.getMessage()));
			}
		}

		//if not running, waveband configuration might have changed also, so update it
		if(!isRunning)
		{
			myConfig.wavebands = newConfig.wavebands;

			myFlashlight->sendCommand(QString("W%1;").arg(calcWavebandsValue(myConfig.wavebands)), true);

			initializeFrame();
		}
	}
}

void CameraSystem::configureFlashlight()
{
	if(!isOpen)
	{
		return;
	}

	try
	{
		//warning: the ringlight needs integration time in 1/10ths of a ms as Integer!
		quint16 intVal = (quint16)(myConfig.integrationTime*10);
		myFlashlight->sendCommand(QString("P%1;").arg(intVal), true);
		myFlashlight->sendCommand(QString("F%1;").arg(myConfig.frequency), true);
		myFlashlight->sendCommand(QString("W%1;").arg(calcWavebandsValue(myConfig.wavebands)), true);
	}
	catch (FlashlightException e)
	{
		throw ImageSourceException(QString("Error configuring flashlight: %1")
								   .arg(e.getMessage()));
	}
}

quint8 CameraSystem::calcWavebandsValue(wavebands)
{
	quint8 value = 0;

	//calculate waveband value: binary representation, 1 = enabled, from left
	QList<bool> bands = myConfig.wavebands.config.values();
	for (quint8 i = 0; i < myConfig.wavebands.totalNumber; i++)
	{
		if(bands.at(i))
			value |= 1<<(myConfig.wavebands.totalNumber - (i+1));
	}

	return value;
}
