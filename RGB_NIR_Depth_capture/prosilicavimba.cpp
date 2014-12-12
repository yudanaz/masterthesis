#include "prosilicavimba.h"
#include "VmbTransform.h"

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
		bool success = false;
		for (CameraPtrVector::iterator it = cameras.begin();
			 it != cameras.end(); ++it)
		{
			(*it)->GetModel(model);
			QString compModell(model.c_str());
			//qDebug() << compModell;
			if( compModell.contains("GT2050C") ) // (02-2627A) das ist die Prosilica!!!
			{
				pProsilica = (*it);
				success = true;
				break;
			}
		}

		if(!success) { throw CameraException("No Prosilica camera found!"); }
	}
	//otherwise, use the given IP
	else
	{
		if( VmbErrorSuccess == system.GetCameraByID( IPaddress.toStdString().c_str(), pProsilica) )
		{
			pProsilica->GetModel(model);
			QString compModell(model.c_str());
			if( !compModell.contains("GT2050C") )
			{
				throw CameraException("Wrong camera model found on given IP!");
			}
		}
		else
			throw CameraException("Did not find any camera with given IP!");
	}

	FeaturePtr pCmd;

	res = pProsilica->Open(VmbAccessModeFull);
	if ( res != VmbErrorSuccess )
			throw CameraException(QString("Can not open camera: %1").arg(convErrToMsg(res)));

	//set large packet size
//	res = pProsilica->GetFeatureByName("GVSPPacketSize", pCmd);

//	if( res == VmbErrorSuccess )
//	{
//		res = pCmd->SetValue(8096);
//	}
//	else
//	{
		res = pProsilica->GetFeatureByName("GVSPAdjustPacketSize", pCmd);

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
//	}
	if ( res != VmbErrorSuccess )
	{
		throw CameraException(QString("Can't adjust PacketSize! Code: %1").arg(convErrToMsg(res)));
	}

	//get image size
	VmbInt64_t width, height;
	res = pProsilica->GetFeatureByName("Width", pCmd);
	if ( VmbErrorSuccess == res )
	{
		res = pCmd->GetValue(width);
		if ( VmbErrorSuccess == res )
		{
			res = pProsilica->GetFeatureByName("Height", pCmd);
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

		qDebug() << "Image size: " << myWidth << "x" << myHeight;
	}
	else
		throw CameraException(QString("Invalid image size given by camera: %1x%2").arg(width).arg(height));

	//create frame observer for prosilica:
	myFrameObserver.reset( new FrameObserver(pProsilica) );

	connected = true;

	qDebug() << "Successfully connected to camera " << model.c_str() << endl;
}

void ProsilicaVimba::disconnect()
{
	if(configured) { pProsilica->StopContinuousImageAcquisition(); }
	pProsilica->Close();
	connected = false;
}


void ProsilicaVimba::configure(double exposureTime, quint8 bufferSize)
{
	VmbErrorType res;
	FeaturePtr pFeature;

	myBufferSize = bufferSize;

	if( !connected )
		throw CameraException("Can't configure: not connected to camera!");

	//if already configured and waiting for images, stop first:
	if(configured)
	{
		pProsilica->StopContinuousImageAcquisition();
	}

	//configure TriggerMode:
	res = pProsilica->GetFeatureByName("TriggerMode", pFeature);
	if( res == VmbErrorSuccess )
	{
		res = pFeature->SetValue(1);
	}
	if ( res != VmbErrorSuccess )
	{
		throw CameraException(QString("Can't set TriggerMode_On! Code: %1")
							  .arg(convErrToMsg(res)));
	}

	//set trigger source
	/*INFO: differently than for the Goldeye, the image acquisition can be started either by the
	StartContinuousImageAcquisition - feature software command, or through a trigger.
	Software trigger doesn't seem to work
	*/
//	res = pProsilica->GetFeatureByName("TriggerSource", pFeature);
//	if(res == VmbErrorSuccess){ res = pFeature->SetValue(5); } //5 = software trigger
//	if(res != VmbErrorSuccess)
//	{
//		throw CameraException(QString("Can't set TriggerSource! Code: %1")
//							  .arg(convErrToMsg(res)));
//	}

	//set pixel format and image height, width and offset
	res = pProsilica->GetFeatureByName("PixelFormat", pFeature);
	if(res == VmbErrorSuccess){ res = pFeature->SetValue("BayerGB8"); } //8bit BGR
//	if(res == VmbErrorSuccess){ res = pFeature->SetValue("Mono8"); } //8bit Monochromatic
	if(res != VmbErrorSuccess)
	{
		throw CameraException(QString("Can't set Pixelformat! Code: %1").arg(convErrToMsg(res)));
	}

	//get pixel format back from camera (for image conversion)
	VmbInt64_t m_nPixelFormat;
	res = pFeature->GetValue( m_nPixelFormat );
	pixelFormat = (VmbPixelFormatType)m_nPixelFormat;
	if(res != VmbErrorSuccess)
	{
		throw CameraException(QString("Can't get Pixelformat from camera! Code: %1").arg(convErrToMsg(res)));
	}

	res = pProsilica->GetFeatureByName("Height", pFeature);
	if(res == VmbErrorSuccess){ res = pFeature->SetValue(2048); } //max sensor heigth
	if(res != VmbErrorSuccess)
	{
		throw CameraException(QString("Can't set image Height! Code: %1").arg(convErrToMsg(res)));
	}

	res = pProsilica->GetFeatureByName("Width", pFeature);
	if(res == VmbErrorSuccess){ res = pFeature->SetValue(2048); } //max sensor width
	if(res != VmbErrorSuccess)
	{
		throw CameraException(QString("Can't set image Width! Code: %1").arg(convErrToMsg(res)));
	}

	res = pProsilica->GetFeatureByName("OffsetX", pFeature);
	if(res == VmbErrorSuccess){ res = pFeature->SetValue(0); }
	if(res != VmbErrorSuccess)
	{
		throw CameraException(QString("Can't set x-axis offset to zero! Code: %1").arg(convErrToMsg(res)));
	}

	res = pProsilica->GetFeatureByName("OffsetY", pFeature);
	if(res == VmbErrorSuccess){ res = pFeature->SetValue(0); }
	if(res != VmbErrorSuccess)
	{
		throw CameraException(QString("Can't set y-axis offset to zero! Code: %1").arg(convErrToMsg(res)));
	}


	//configure Gain to 1:
//	setHighgain(false);

//	//set default NUC correction:
//	setCorrectionDataset(exposureTime);

	configured = true;
}

void ProsilicaVimba::setExposureTime(double ms)
{
	VmbErrorType res;
	FeaturePtr pCmd;

	res = pProsilica->GetFeatureByName("ExposureTimeAbs", pCmd);
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

quint16 ProsilicaVimba::getMaxFPS()
{
	VmbErrorType res;
	FeaturePtr pCmd;

	if(connected)
	{

		//query maximum frame rate:
		double val;
		res = pProsilica->GetFeatureByName("AcquisitionFrameRateLimit", pCmd);
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
	else
	{
		return 100;
	}
}


Mat ProsilicaVimba::getCVFrame()
{
	FeaturePtr pfeature;
	FramePtr frame;
	VmbErrorType res;
	VmbFrameStatusType eReceiveStatus;

	Mat out(myHeight, myWidth, CV_16UC1);

	/*INFO: differently than for the Goldeye, the image acquisition can be started either by the
	StartContinuousImageAcquisition - feature software command, or through a trigger.
	Software trigger doesn't seem to work
	*/
	//start image acquisition
	res = pProsilica->StartContinuousImageAcquisition(myBufferSize, myFrameObserver);
	if ( res != VmbErrorSuccess )
	{
		throw CameraException(QString("Can't start acquisition! Code: %1")
							  .arg(convErrToMsg(res)));
	}

	//wait for frame
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
		if ( SP_ACCESS(frame)->GetImage( pBuffer ) == VmbErrorSuccess)
		{
			//create new Mat with correct size and use buffer as data:
			Mat newMat(myHeight, myWidth, CV_8UC3);
			convertImageFormat(pBuffer, newMat);

			//scale the input mat to output
			out = newMat.clone();
		}
		pProsilica->QueueFrame(frame);
	}
	else
	{
		pProsilica->QueueFrame(frame);

		throw CameraReadException(QString("Corrupt frame: %1").arg(convErrToMsg(eReceiveStatus)));
	}

	//stop image aquisition
	res = pProsilica->StopContinuousImageAcquisition();
	if ( res != VmbErrorSuccess )
	{
		throw CameraException(QString("Can't stop acquisition! Code: %1")
							  .arg(convErrToMsg(res)));
	}

	return out;
}

void ProsilicaVimba::reset()
{
	pProsilica->StopContinuousImageAcquisition();

	SP_DYN_CAST(myFrameObserver, FrameObserver)->clearFrameQueue();

	pProsilica->StartContinuousImageAcquisition(myBufferSize, myFrameObserver);
}


void ProsilicaVimba::triggerViaSoftware()
{
	FeaturePtr pFeature;
	VmbErrorType res = pProsilica->GetFeatureByName("TriggerSoftware", pFeature);
	if(res  == VmbErrorSuccess )
	{
		res = pFeature->RunCommand();
	}
	if(res != VmbErrorSuccess)
	{
		throw CameraException(QString("Can't send Software Trigger! Code: %1")
						  .arg(convErrToMsg(res)));
	}
}

void ProsilicaVimba::convertImageFormat(VmbUchar_t *pInBuffer, Mat &outImg)
{
	VmbImage            SourceImage,DestImage;
	VmbErrorType        res;
	SourceImage.Size    = sizeof( SourceImage );
	DestImage.Size      = sizeof( DestImage );

	//set format of the input image
	res = (VmbErrorType)VmbSetImageInfoFromPixelFormat( pixelFormat, myWidth, myHeight, &SourceImage );
	if ( res != VmbErrorSuccess )
	{
		throw CameraException(QString("Can't set image info from pixel format! Code: %1").arg(convErrToMsg(res)));
	}

	//set format for output image
	QString outputFormat = "BGR8";
	res = (VmbErrorType)VmbSetImageInfoFromString( outputFormat.toStdString().c_str(), outputFormat.length(),myWidth, myHeight, &DestImage );
	if ( res != VmbErrorSuccess )
	{
		throw CameraException(QString("Can't set image info from format string! Code: %1").arg(convErrToMsg(res)));
	}

	//set pointer of VMB images to in buffer and opencv Mat buffer
	SourceImage.Data    = pInBuffer;
//	QImage *debugImg = new QImage( myWidth, myHeight, QImage::Format_RGB888 );
//	DestImage.Data      = debugImg->bits();
	DestImage.Data = outImg.data;

	//make format transformation
	res = (VmbErrorType)VmbImageTransform( &SourceImage, &DestImage, NULL, 0 );
	if ( res != VmbErrorSuccess )
	{
		throw CameraException(QString("Can't transform image! Code: %1").arg(convErrToMsg(res)));
	}
}


