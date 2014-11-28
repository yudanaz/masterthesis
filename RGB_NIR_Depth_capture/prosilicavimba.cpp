#include "prosilicavimba.h"

ProsilicaVimba::ProsilicaVimba() : system ( VimbaSystem::GetInstance() )
{
	//configure Camera class
	myDepth = 16; //actually only 8 or 12 bit, but upscaled to 16
	myWidth = 0;
	myHeight = 0;

//	myGain = false;

	connected = false;
	configured = false;
}

ProsilicaVimba::~ProsilicaVimba()
{
	if(connected) { disconnect(); }
}

void ProsilicaVimba::connect()
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

void ProsilicaVimba::connect(QString IPaddress)
{
	VmbErrorType res;

	string model;

	// connect to the first prosilica camera, if the address is empty
	if(IPaddress.length() == 0)
	{
		//get a list of accessible cameras
		CameraPtrVector cameras;
		if ( VmbErrorSuccess != system.GetCameras( cameras ))
			throw CameraException("Error enumerating available cameras");
		if (cameras.size() == 0)
			throw CameraException("No cameras found!");

//		//iterate through list to find the first prosilica camera
//		bool success = false;
		for (CameraPtrVector::iterator it = cameras.begin();
			 it != cameras.end(); ++it)
		{
			(*it)->GetModel(model);
			QString compModell(model.c_str());
			qDebug() << compModell;
//			if( compModell.contains("P-032") ) //GT2050C (02-2627A) das ist die Prosilica!!!
//			{
//				pGoldeye = (*it);
//				isPmodel = true;
//				success = true;
//				break;
//			}
//			else if( compModell.contains("G-032") )
//			{
//				pGoldeye = (*it);
//				isPmodel = false;
//				success = true;
//				break;
//			}
		}

//		if(!success)
//			throw CameraException("No Prosilica camera found!");
	}
//	//otherwise, use the given IP
//	else
//	{
//		if( VmbErrorSuccess == system.GetCameraByID( IPaddress.toStdString().c_str(), pGoldeye) )
//		{
//			pGoldeye->GetModel(model);
//			QString compModell(model.c_str());
//			if( compModell.contains("P-032") )
//			{
//				isPmodel = true;
//			}
//			else if (compModell.contains("G-032"))
//			{
//				isPmodel = false;
//			}
//			else
//			{
//				throw CameraException("Wrong camera model found on given IP!");
//			}
//		}
//		else
//			throw CameraException("Did not find any camera with given IP!");
//	}

//	FeaturePtr pCmd;

//	res = pGoldeye->Open(VmbAccessModeFull);
//	if ( res != VmbErrorSuccess )
//			throw CameraException(QString("Can not open camera: %1").arg(convErrToMsg(res)));

//	//set large packet size
//	if(isPmodel)
//	{
//		res = pGoldeye->GetFeatureByName("GVSPPacketSize", pCmd);

//		if( res == VmbErrorSuccess )
//		{
//			res = pCmd->SetValue(8164);
//		}
//	}
//	else
//	{
//		res = pGoldeye->GetFeatureByName("GVSPAdjustPacketSize", pCmd);

//		if( res == VmbErrorSuccess )
//		{
//			res = pCmd->RunCommand();

//			if( res == VmbErrorSuccess )
//			{
//				bool bIsCommandDone = false;
//				do
//				{
//					res = pCmd->IsCommandDone( bIsCommandDone );
//					if( res != VmbErrorSuccess )
//					{
//						break;
//					}
//				} while( false == bIsCommandDone );
//			}
//		}
//	}
//	if ( res != VmbErrorSuccess )
//	{
//		throw CameraException(QString("Can't adjust PacketSize! Code: %1").arg(convErrToMsg(res)));
//	}

//	//get image size
//	VmbInt64_t width, height;
//	res = pGoldeye->GetFeatureByName("Width", pCmd);
//	if ( VmbErrorSuccess == res )
//	{
//		res = pCmd->GetValue(width);
//		if ( VmbErrorSuccess == res )
//		{
//			res = pGoldeye->GetFeatureByName("Height", pCmd);
//			if ( VmbErrorSuccess == res )
//			{
//				res = pCmd->GetValue(height);
//			}
//		}
//	}
//	if ( res != VmbErrorSuccess )
//	{
//		throw CameraException(QString("Could not get image width or height: %1").arg(convErrToMsg(res)));
//	}
//	//check image size for validity
//	if((width > 0) && (height > 0))
//	{
//		myWidth = width;
//		myHeight = height;
//	}
//	else
//		throw CameraException(QString("Invalid image size given by camera: %1x%2").arg(width).arg(height));

//	//create frame observer for goldeye:
//	myFrameObserver.reset( new FrameObserver(pGoldeye) );

//	connected = true;

//	qDebug() << "Successfully connected to camera " << model.c_str() << endl;
}

void ProsilicaVimba::disconnect()
{
	if(configured) { pProsilica->StopContinuousImageAcquisition(); }
	pProsilica->Close();
	connected = false;
}


void ProsilicaVimba::configure(double exposureTime, quint8 bufferSize)
{
//	VmbErrorType res;
//	FeaturePtr pCmd;

//	myBufferSize = bufferSize;

//	if( !connected )
//		throw CameraException("Can't configure: not connected to camera!");

//	//if already configured and waiting for images, stop first:
//	if(configured)
//	{
//		pGoldeye->StopContinuousImageAcquisition();
//	}

//	//configure TriggerMode:
//	res = pGoldeye->GetFeatureByName("TriggerMode", pCmd);
//	if( res == VmbErrorSuccess )
//	{
//		res = pCmd->SetValue(0);//1);
//	}
//	if ( res != VmbErrorSuccess )
//	{
//		throw CameraException(QString("Can't set TriggerMode_On! Code: %1")
//							  .arg(convErrToMsg(res)));
//	}

//	//for Goldeye P:
//	if(isPmodel)
//	{
//		//configure ExposureMode off (trigger controls exposure time):
//		res = pGoldeye->GetFeatureByName("ExposureMode", pCmd);
//		if( res == VmbErrorSuccess )
//		{
//			res = pCmd->SetValue(0);
//		}
//		if ( res != VmbErrorSuccess )
//		{
//			throw CameraException(QString("Can't set ExposureMode! Code: %1")
//								  .arg(convErrToMsg(res)));
//		}
//	}

//	//for Goldeye G:
//	if(!isPmodel)
//	{
//		//configure ExposureTime
//		setExposureTime(exposureTime);

//		//configure trigger source to line 2
//		res = pGoldeye->GetFeatureByName("TriggerSource", pCmd);
//		if( res == VmbErrorSuccess )
//		{
//			res = pCmd->SetValue(2);
//		}
//		if ( res != VmbErrorSuccess )
//		{
//			throw CameraException(QString("Can't set TriggerSource! Code: %1")
//								  .arg(convErrToMsg(res)));
//		}
//	}

//	//configure Gain to 1:
//	setHighgain(false);

//	//set default NUC correction:
//	setCorrectionDataset(exposureTime);

//	//start continuous acquisition (which means: wait for next trigger signal)
//	res = pGoldeye->StartContinuousImageAcquisition(myBufferSize, myFrameObserver);

//	if ( res != VmbErrorSuccess )
//	{
//		throw CameraException(QString("Can't start acquisition! Code: %1")
//							  .arg(convErrToMsg(res)));
//	}

//	configured = true;
}

void ProsilicaVimba::setExposureTime(double ms)
{
//	VmbErrorType res;
//	FeaturePtr pCmd;

//	//P-model does gets exposure time via trigger
//	if(isPmodel)
//		return;

//	res = pGoldeye->GetFeatureByName("ExposureTime", pCmd);
//	if( res == VmbErrorSuccess )
//	{
//		res = pCmd->SetValue(ms*1000);
//	}
//	if ( res != VmbErrorSuccess )
//	{
//		throw CameraException(QString("Can't set ExposureTime! Code: %1")
//							  .arg(convErrToMsg(res)));
//	}
}

quint16 ProsilicaVimba::getMaxFPS()
{
//	VmbErrorType res;
//	FeaturePtr pCmd;

//	if(connected)
//	{
//		if(isPmodel)
//		{
//			return 30;
//		}
//		else
//		{
//			//query maximum frame rate:
//			double val;
//			res = pGoldeye->GetFeatureByName("AcquisitionFrameRateLimit", pCmd);
//			if( res == VmbErrorSuccess )
//			{
//				res = pCmd->GetValue(val);
//			}
//			if ( res != VmbErrorSuccess )
//			{
//				throw CameraException(QString("Can't get AcquisitionFrameRateLimit! Code: %1")
//									  .arg(convErrToMsg(res)));
//			}
//			else
//			{
//				if ((val <= 0) || (val > 1000))
//				{
//					throw CameraException("Camera returned invalid maximum frame rate!");
//				}
//			}
//			return (quint16)val;
//		}
//	}
//	else
//	{
//		return 100;
//	}

	return 0;
}


Mat ProsilicaVimba::getCVFrame()
{
//	FramePtr frame;
//	VmbErrorType res;
//	VmbFrameStatusType eReceiveStatus;

//	Mat out(myHeight, myWidth, CV_16UC1);

//	quint16 count = 0;
//	while (!(SP_DYN_CAST(myFrameObserver, FrameObserver)->isFrameAvailable()))
//	{
//		usleep(200);

//		if(count++ > 5000) //after 1 second, throw timeout.
//			throw CameraTimeoutException();
//	}

//	frame = SP_DYN_CAST(myFrameObserver, FrameObserver)->getFrame();

//	// If the frame is completely transmitted, extract it's data:
//	res = frame->GetReceiveStatus(eReceiveStatus);
//	if ( (res == VmbErrorSuccess) && (eReceiveStatus == VmbFrameStatusComplete) )
//	{
//		VmbUchar_t *pBuffer;
//		if ( frame->GetImage( pBuffer ) == VmbErrorSuccess)
//		{
//			//create new Mat with correct size and use buffer as data:
//			Mat newMat(myHeight, myWidth, CV_16UC1, pBuffer);

//			//memcpy( newMat.data, pBuffer, myWidth * myHeight );

//			//scale the input mat to output
//			if(isPmodel)
//				out = newMat * 16; //12 to 16 bit
//			else
//				out = newMat * 4; //14 to 16 bit
//		}
//		pGoldeye->QueueFrame(frame);
//	}
//	else
//	{
//		pGoldeye->QueueFrame(frame);

//		throw CameraReadException(QString("Corrupt frame: %1").arg(convErrToMsg(eReceiveStatus)));
//	}

//	return out;
	return Mat();
}

void ProsilicaVimba::reset()
{
//	pGoldeye->StopContinuousImageAcquisition();

//	SP_DYN_CAST(myFrameObserver, FrameObserver)->clearFrameQueue();

//	pGoldeye->StartContinuousImageAcquisition(myBufferSize, myFrameObserver);
}
