#include "goldeyeVimba.h"

GoldeyeVimba::GoldeyeVimba() : system ( VimbaSystem::GetInstance() )
{
	//configure Camera class
	myDepth = 16; //actually only 12 or 14 bit, but upscaled to 16
	myWidth = 0;
	myHeight = 0;

	myGain = false;

	connected = false;
	configured = false;

	//startup / initialize Vimba API
//	if ( VmbErrorSuccess != system.Startup() )
//		throw CameraException("Could not initialize Vimba API");
}

GoldeyeVimba::~GoldeyeVimba()
{
	if(connected)
		disconnect();

	//Shutdown Vimba API to free ressources
	//system.Shutdown();
}

void GoldeyeVimba::connect()
{
	try
	{
		connect("");
	}
	catch (...)
	{
		throw;
	}
}

void GoldeyeVimba::connect(QString IPaddress)
{
	VmbErrorType res;

	string model;

	// connect to the first goldeye camera, if the address is empty
	if(IPaddress.length() == 0)
	{
		//get a list of accessible cameras
		CameraPtrVector cameras;
		if ( VmbErrorSuccess != system.GetCameras( cameras ))
			throw CameraException("Error enumerating available cameras");
		if (cameras.size() == 0)
			throw CameraException("No cameras found!");

		//iterate through list to find the first goldeye camera
		bool success = false;
		for (CameraPtrVector::iterator it = cameras.begin();
			 it != cameras.end(); ++it)
		{
			(*it)->GetModel(model);
			QString compModell(model.c_str());
			if( compModell.contains("P-032") )
			{
				pGoldeye = (*it);
				isPmodel = true;
				success = true;
				break;
			}
			else if( compModell.contains("G-032") )
			{
				pGoldeye = (*it);
				isPmodel = false;
				success = true;
				break;
			}
		}

		if(!success)
			throw CameraException("No Goldeye camera found!");
	}
	//otherwise, use the given IP
	else
	{
		if( VmbErrorSuccess == system.GetCameraByID( IPaddress.toStdString().c_str(), pGoldeye) )
		{
			pGoldeye->GetModel(model);
			QString compModell(model.c_str());
			if( compModell.contains("P-032") )
			{
				isPmodel = true;
			}
			else if (compModell.contains("G-032"))
			{
				isPmodel = false;
			}
			else
			{
				throw CameraException("Wrong camera model found on given IP!");
			}
		}
		else
			throw CameraException("Did not find any camera with given IP!");
	}

	FeaturePtr pCmd;

	res = pGoldeye->Open(VmbAccessModeFull);
	if ( res != VmbErrorSuccess )
			throw CameraException(QString("Can not open camera: %1").arg(convErrToMsg(res)));

	//set large packet size
	if(isPmodel)
	{
		res = pGoldeye->GetFeatureByName("GVSPPacketSize", pCmd);

		if( res == VmbErrorSuccess )
		{
			res = pCmd->SetValue(8164);
		}
	}
	else
	{
		res = pGoldeye->GetFeatureByName("GVSPAdjustPacketSize", pCmd);

		if( res == VmbErrorSuccess )
		{
			res = pCmd->RunCommand();

			if( res == VmbErrorSuccess )
			{
				bool bIsCommandDone = false;
				do
				{
					res = pCmd->IsCommandDone( bIsCommandDone );
					if( res != VmbErrorSuccess )
					{
						break;
					}
				} while( false == bIsCommandDone );
			}
		}
	}
	if ( res != VmbErrorSuccess )
	{
		throw CameraException(QString("Can't adjust PacketSize! Code: %1").arg(convErrToMsg(res)));
	}

	//get image size
	VmbInt64_t width, height;
	res = pGoldeye->GetFeatureByName("Width", pCmd);
	if ( VmbErrorSuccess == res )
	{
		res = pCmd->GetValue(width);
		if ( VmbErrorSuccess == res )
		{
			res = pGoldeye->GetFeatureByName("Height", pCmd);
			if ( VmbErrorSuccess == res )
			{
				res = pCmd->GetValue(height);
			}
		}
	}
	if ( res != VmbErrorSuccess )
	{
		throw CameraException(QString("Could not get image width or height: %1").arg(convErrToMsg(res)));
	}
	//check image size for validity
	if((width > 0) && (height > 0))
	{
		myWidth = width;
		myHeight = height;
	}
	else
		throw CameraException(QString("Invalid image size given by camera: %1x%2").arg(width).arg(height));

	//create frame observer for goldeye:
	myFrameObserver.reset( new FrameObserver(pGoldeye) );

	connected = true;

	qDebug() << "Successfully connected to camera " << model.c_str() << endl;
}

void GoldeyeVimba::disconnect()
{
	if(configured)
	{
		pGoldeye->StopContinuousImageAcquisition();
	}

	pGoldeye->Close();

	connected = false;
}

void GoldeyeVimba::configure(double exposureTime, quint8 bufferSize)
{
	VmbErrorType res;
	FeaturePtr pCmd;

	myBufferSize = bufferSize;

	if( !connected )
		throw CameraException("Can't configure: not connected to camera!");

	//if already configured and waiting for images, stop first:
	if(configured)
	{
		pGoldeye->StopContinuousImageAcquisition();
	}

	//configure TriggerMode:
	res = pGoldeye->GetFeatureByName("TriggerMode", pCmd);
	if( res == VmbErrorSuccess )
	{
		res = pCmd->SetValue(0);//1);  // 0 = trigger mode off!!
	}
	if ( res != VmbErrorSuccess )
	{
		throw CameraException(QString("Can't set TriggerMode_On! Code: %1")
							  .arg(convErrToMsg(res)));
	}

	//for Goldeye P:
	if(isPmodel)
	{
		//configure ExposureMode off (trigger controls exposure time):
		res = pGoldeye->GetFeatureByName("ExposureMode", pCmd);
		if( res == VmbErrorSuccess )
		{
			res = pCmd->SetValue(0);
		}
		if ( res != VmbErrorSuccess )
		{
			throw CameraException(QString("Can't set ExposureMode! Code: %1")
								  .arg(convErrToMsg(res)));
		}
	}

	//for Goldeye G:
	if(!isPmodel)
	{
		//configure ExposureTime
		setExposureTime(exposureTime);

		//configure trigger source to line 2
		res = pGoldeye->GetFeatureByName("TriggerSource", pCmd);
		if( res == VmbErrorSuccess )
		{
			res = pCmd->SetValue(2);
		}
		if ( res != VmbErrorSuccess )
		{
			throw CameraException(QString("Can't set TriggerSource! Code: %1")
								  .arg(convErrToMsg(res)));
		}
	}

	//configure Gain to 1:
	setHighgain(false);

	//set default NUC correction:
	setCorrectionDataset(exposureTime);

	//start continuous acquisition (which means: wait for next trigger signal)
	res = pGoldeye->StartContinuousImageAcquisition(myBufferSize, myFrameObserver);

	if ( res != VmbErrorSuccess )
	{
		throw CameraException(QString("Can't start acquisition! Code: %1")
							  .arg(convErrToMsg(res)));
	}

	configured = true;
}

void GoldeyeVimba::setExposureTime(double ms)
{
	VmbErrorType res;
	FeaturePtr pCmd;

	//P-model does gets exposure time via trigger
	if(isPmodel)
		return;

	res = pGoldeye->GetFeatureByName("ExposureTime", pCmd);
	if( res == VmbErrorSuccess )
	{
		res = pCmd->SetValue(ms*1000);
	}
	if ( res != VmbErrorSuccess )
	{
		throw CameraException(QString("Can't set ExposureTime! Code: %1")
							  .arg(convErrToMsg(res)));
	}
}

quint16 GoldeyeVimba::getMaxFPS()
{
	VmbErrorType res;
	FeaturePtr pCmd;

	if(connected)
	{
		if(isPmodel)
		{
			return 30;
		}
		else
		{
			//query maximum frame rate:
			double val;
			res = pGoldeye->GetFeatureByName("AcquisitionFrameRateLimit", pCmd);
			if( res == VmbErrorSuccess )
			{
				res = pCmd->GetValue(val);
			}
			if ( res != VmbErrorSuccess )
			{
				throw CameraException(QString("Can't get AcquisitionFrameRateLimit! Code: %1")
									  .arg(convErrToMsg(res)));
			}
			else
			{
				if ((val <= 0) || (val > 1000))
				{
					throw CameraException("Camera returned invalid maximum frame rate!");
				}
			}
			return (quint16)val;
		}
	}
	else
	{
		return 100;
	}
}


Mat GoldeyeVimba::getCVFrame()
{
	FramePtr frame;
	VmbErrorType res;
	VmbFrameStatusType eReceiveStatus;

	Mat out(myHeight, myWidth, CV_16UC1);

	quint16 count = 0;
	while (!(SP_DYN_CAST(myFrameObserver, FrameObserver)->isFrameAvailable()))
	{
		usleep(200);

		if(count++ > 5000) //after 1 second, throw timeout.
			throw CameraTimeoutException();
	}

	frame = SP_DYN_CAST(myFrameObserver, FrameObserver)->getFrame();

	// If the frame is completely transmitted, extract it's data:
	res = frame->GetReceiveStatus(eReceiveStatus);
	if ( (res == VmbErrorSuccess) && (eReceiveStatus == VmbFrameStatusComplete) )
	{
		VmbUchar_t *pBuffer;
		if ( frame->GetImage( pBuffer ) == VmbErrorSuccess)
		{
			//create new Mat with correct size and use buffer as data:
			Mat newMat(myHeight, myWidth, CV_16UC1, pBuffer);

			//memcpy( newMat.data, pBuffer, myWidth * myHeight );

			//scale the input mat to output
			if(isPmodel)
				out = newMat * 16; //12 to 16 bit
			else
				out = newMat * 4; //14 to 16 bit
		}
		pGoldeye->QueueFrame(frame);
	}
	else
	{
		pGoldeye->QueueFrame(frame);

		throw CameraReadException(QString("Corrupt frame: %1").arg(convErrToMsg(eReceiveStatus)));
	}

	return out;
}

void GoldeyeVimba::reset()
{
	pGoldeye->StopContinuousImageAcquisition();

	SP_DYN_CAST(myFrameObserver, FrameObserver)->clearFrameQueue();

	pGoldeye->StartContinuousImageAcquisition(myBufferSize, myFrameObserver);
}


void GoldeyeVimba::setCorrectionDataset(double intTime)
{
	VmbErrorType result;
	FeaturePtr pCmd;

	quint8 corrData;

	if(!isPmodel)
	{
		if (!myGain)
		{
			if(intTime < 3)
				corrData = 0;
			else if (intTime < 12.5)
				corrData = 1;
			else if (intTime < 35)
				corrData = 2;
			else if (intTime < 65)
				corrData = 3;
			else if (intTime < 90)
				corrData = 4;
			else if (intTime < 125)
				corrData = 5;
			else if (intTime < 175)
				corrData = 6;
			else
				corrData = 7;
		}
		else
		{
			if(intTime < 0.75)
				corrData = 8;
			else if (intTime < 1.5)
				corrData = 9;
			else if (intTime < 3.5)
				corrData = 10;
			else if (intTime < 7.5)
				corrData = 11;
			else if (intTime < 12.5)
				corrData = 12;
			else if (intTime < 17.5)
				corrData = 13;
			else
				corrData = 14;
		}

		//Set correction pattern
		result = pGoldeye->GetFeatureByName("NUCDatasetSelector", pCmd);
		if( result == VmbErrorSuccess )
		{
			result = pCmd->SetValue(corrData);
		}
		if ( result != VmbErrorSuccess )
		{
			throw CameraException(QString("Can't set NUCDatasetSelector! Code: %1")
								  .arg(convErrToMsg(result)));
		}

		result = pGoldeye->GetFeatureByName("NUCDatasetActivate", pCmd);
		if( result == VmbErrorSuccess )
		{
			result = pCmd->RunCommand();
		}
		if ( result != VmbErrorSuccess )
		{
			throw CameraException(QString("Can't activate NUCDataset! Code: %1")
								  .arg(convErrToMsg(result)));
		}
	}
	else
	{
		if (intTime <= 1) // 1ms (or less)
		{
			corrData = 0x1;
		}
		else if (intTime <= 3) // 2-3 ms
		{
			corrData = 0x2;
		}
		else if (intTime <= 7) // 4-7ms
		{
			corrData = 0x3;
		}
		else if (intTime <= 15) // 8-15ms
		{
			corrData = 0x4;
		}
		else if (intTime <= 35) // 16-35ms
		{
			corrData = 0x5;
		}
		else if (intTime <= 62) //36-62ms
		{
			corrData = 0x6;
		}
		else if (intTime <= 87) //63-87ms
		{
			corrData = 0x7;
		}
		else if (intTime <= 125) //88-125ms
		{
			corrData = 0x8;
		}
		else if (intTime <= 175) //126-175ms
		{
			corrData = 0x9;
		}
		else // above 175ms
		{
			corrData = 0xA;
		}

		if(myGain && (corrData <= 0x06)) //if on high gain and low integration time
		{
			corrData = corrData + 0x10;
		}

		//Set correction pattern
		result = pGoldeye->GetFeatureByName("CorrectionDataSet", pCmd);
		if( result == VmbErrorSuccess )
		{
			result = pCmd->SetValue(corrData);
		}
		if ( result != VmbErrorSuccess )
		{
			throw CameraException(QString("Can't set CorrectionDataSet! Code: %1")
								  .arg(convErrToMsg(result)));
		}
	}
}

void GoldeyeVimba::setHighgain(bool setGain)
{
	VmbErrorType res;
	FeaturePtr pCmd;

	if(isPmodel)
		res = pGoldeye->GetFeatureByName("Gain", pCmd);
	else
		res = pGoldeye->GetFeatureByName("SensorGain", pCmd);

	if( res == VmbErrorSuccess )
	{
		if(isPmodel)
		{
			if(setGain)
				res = pCmd->SetValue(20.0);
			else
				res = pCmd->SetValue(1.0);
		}
		else
		{
			if(setGain)
				res = pCmd->SetValue(1);
			else
				res = pCmd->SetValue(0);
		}
	}
	if ( res != VmbErrorSuccess )
	{
		throw CameraException(QString("Can't set Gain! Code: %1").arg(convErrToMsg(res)));
	}

	myGain = setGain;
}

//Methods by Maurice:
void GoldeyeVimba::triggerViaSoftware()
{

}

