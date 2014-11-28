#include "goldeyePleora.h"

GoldeyePleora::GoldeyePleora(bool setDebug)
{
	//configure Camera class
	myDepth = 16; //actually only 12 bit, but upscaled to 16
	myWidth = 636;
	myHeight = 508;

	//initialize pointer variables
	newFrame = NULL;
	unscaledFrame = NULL;
	myBuffer = NULL;
	myDeviceInfo = NULL;
	myPipeline = NULL;
	myDeviceParams = NULL;

	myGain = false;

	debug = setDebug;

	connected = false;
}

GoldeyePleora::~GoldeyePleora()
{
	if(connected)
	{
		disconnect();
	}

	//delete pipeline as it has been created using "new"
	if(myPipeline != NULL)
		delete myPipeline;

	//cleanup openCV images
	if(newFrame != NULL)
		cvReleaseImage(&newFrame);

	if(unscaledFrame != NULL)
		cvReleaseImage(&unscaledFrame);
}

void GoldeyePleora::disconnect()
{
	//Stop and disconnect camera
	myDeviceParams->ExecuteCommand("AcquisitionStop" );
	myDeviceParams->SetIntegerValue( "TLParamsLocked", 0 );
	myPipeline->Stop();
	myStream.Close();
	myDevice.Disconnect();
}

void GoldeyePleora::connect()
{
	connect(QString());
}

void GoldeyePleora::connect(QString IPaddress)
{
	bool found = false;

	quint8 i, d;

	PvResult cmd;
	PvInterface * lInterface;
	PvUInt32 lDeviceCount;

	// Find all GEV Devices on the network.
	mySystem.SetDetectionTimeout( 2000 );
	cmd = mySystem.Find();
	if( !cmd.IsOK() )
	{
		throw CameraException(QString("PvSystem:Find Error: %1")
						  .arg(cmd.GetCodeString().GetAscii()));
	}

	// Get the number of GEV Interfaces that were found using GetInterfaceCount.
	PvUInt32 lInterfaceCount = mySystem.GetInterfaceCount();

	if(debug)
	{
		// Display information about all found interface
		// For each interface, display information about all devices.
		for( PvUInt32 x = 0; x < lInterfaceCount; x++ )
		{
			// get pointer to each interface
			lInterface = mySystem.GetInterface( x );

			qDebug() << "Interface " << x << "\n"
					 << "MAC Address: " << lInterface->GetMACAddress().GetAscii() << "\n"
					 << "IP Address: " << lInterface->GetIPAddress().GetAscii() << "\n"
					 << "Subnet Mask: " << lInterface->GetSubnetMask().GetAscii() << "\n"
					 << endl;

			// Get the number of GEV devices that were found using GetDeviceCount.
			lDeviceCount = lInterface->GetDeviceCount();

			for( PvUInt32 y = 0; y < lDeviceCount ; y++ )
			{
				myDeviceInfo = lInterface->GetDeviceInfo( y );
				qDebug() << "Device " << y << "\n"
						 << "MAC Address: " << myDeviceInfo->GetMACAddress().GetAscii() << "\n"
						 << "IP Address: " << myDeviceInfo->GetIPAddress().GetAscii() << "\n"
						 << "Serial number: " << myDeviceInfo->GetSerialNumber().GetAscii() << "\n"
						 << endl;
			}
		}
	}
	//if IP address given, search for device with that address
	if(!IPaddress.isEmpty())
	{
		for( i = 0; i < lInterfaceCount; i++ )
		{
			// get pointer to each interface
			lInterface = mySystem.GetInterface( i );

			// Get the number of GEV devices that were found using GetDeviceCount.
			lDeviceCount = lInterface->GetDeviceCount();

			//search through all devices
			for( d = 0; d < lDeviceCount ; d++ )
			{
				myDeviceInfo = lInterface->GetDeviceInfo( d );

				QString tmpIP(myDeviceInfo->GetIPAddress().GetAscii());

				if(IPaddress.startsWith(tmpIP))
				{
					found = true;
					break;
				}
			}

			if(found)
			{
				break;
			}
		}
	}
	//otherwise, just use the first found camera
	else
	{
		for (i = 0; i < lInterfaceCount; i++)
		{
			// get pointer to interface
			lInterface = mySystem.GetInterface( i );

			// get the GEV device count on that interface.
			lDeviceCount = lInterface->GetDeviceCount();

			// check every device if it's a camera (transmitter)
			for(d = 0; d < lDeviceCount; d++)
			{
				myDeviceInfo = lInterface->GetDeviceInfo( d );

				// if this is a camera, stop searching
				if(myDeviceInfo->GetClass() == PvDeviceClassTransmitter)
				{
					found = true;
					break;
				}
			}
			if(found)
			{
				break;
			}
		}
	}

	// Connect to the found GEV device.
	if( found )
	{
		cmd = myDevice.Connect( myDeviceInfo );
		if ( !cmd.IsOK() )
		{
			throw CameraException(QString("Unable to connect to %1. %2: %3")
								  .arg(myDeviceInfo->GetIPAddress().GetAscii())
								  .arg(cmd.GetCodeString().GetAscii())
								  .arg(cmd.GetDescription().GetAscii()));
		}
		else
		{
			qDebug() << "Successfully connected to camera with IP "
					 << myDeviceInfo->GetIPAddress().GetAscii() << "\n"
					 << endl;
		}
	}
	else
	{
		throw CameraException("No device found");
	}

	connected = true;
}

void GoldeyePleora::configure(double exposureTime, quint8 bufferSize)
{
	// Get device parameters needed to control streaming
	myDeviceParams = myDevice.GetGenParameters();

	// Perform automatic packet size negotiation
	myDevice.NegotiatePacketSize( 0, 8164 );

	// Open stream - have the PvDevice do it for us
	myStream.Open( myDeviceInfo->GetIPAddress() );

	myPipeline = new PvPipeline( &myStream );

	// Reading payload size from device
	PvInt64 size = 0;
	myDeviceParams->GetIntegerValue( "PayloadSize", size );

	// Set the Buffer size and the Buffer count
	myPipeline->SetBufferSize( static_cast<PvUInt32>( size ) );
	myPipeline->SetBufferCount( bufferSize );
	// Increase for high frame rate without missing block IDs

	// Have to set the Device IP destination to the Stream
	myDevice.SetStreamDestination( myStream.GetLocalIPAddress(),
								   myStream.GetLocalPort() );

	// IMPORTANT: the pipeline needs to be "armed", or started before
	// we instruct the device to send us images
	myPipeline->Start();

	// TLParamsLocked is optional but when present, it MUST be set to 1
	// before sending the AcquisitionStart command
	myDeviceParams->SetIntegerValue( "TLParamsLocked", 1 );

	myDeviceParams->ExecuteCommand("GevTimestampControlReset" );

	//Set acquisition mode to single shot
	myDeviceParams->SetEnumValue("AcquisitionMode" , 0);

	//Set correction pattern according to exposure time
	setCorrectionDataset((quint8)exposureTime);

	//Set starting parameters
	myDeviceParams->SetEnumValue("TriggerMode", 1);

	myDeviceParams->ExecuteCommand("AcquisitionStart" );
}

void GoldeyePleora::setCorrectionDataset(quint8 intTime)
{
	quint8 corrData;

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
	myDeviceParams->SetIntegerValue("CorrectionDataSet", corrData);
}

quint16 GoldeyePleora::getMaxFPS()
{
	//calculate maximum fps based on exposure time
	if(exposureTime < 33)
	{
		return 30;
	}
	else
	{
		return (quint16)(1.0/exposureTime);
	}
}

void GoldeyePleora::setExposureTime(double ms)
{
	exposureTime = ms;
}

void GoldeyePleora::setHighgain(bool setGain)
{
	myGain = setGain;
	if (myGain)
	{
		myDeviceParams->SetIntegerValue("Gain", 20);
	}
	else
	{
		myDeviceParams->SetIntegerValue("Gain", 1);
	}
}

bool GoldeyePleora::getHighgain()
{
	return myGain;
}

Mat GoldeyePleora::getCVFrame()
{
	PvUInt32 code;
	int errCode;

	// Retrieve next buffer
	PvResult bufResult = myPipeline->RetrieveNextBuffer( &myBuffer, 500 );

	if ( bufResult.IsOK() )
	{
		if ( myBuffer->GetOperationResult().IsOK() )
		{
			if(myBuffer->GetPayloadType() == PvPayloadTypeImage )
			{
				//Get image from buffer and create openCV IplImage if necessary
				if ( newFrame == NULL )
				{
					newFrame = cvCreateImage( cvSize( myBuffer->GetImage()->GetWidth(),
							myBuffer->GetImage()->GetHeight() ), IPL_DEPTH_16U, 1 );
					unscaledFrame = cvCreateImage( cvSize( myBuffer->GetImage()->GetWidth(),
							myBuffer->GetImage()->GetHeight() ), IPL_DEPTH_16U, 1 );

					myIntermediateBuffer.GetImage()->Attach( unscaledFrame->imageData,
							myBuffer->GetImage()->GetWidth(),
							myBuffer->GetImage()->GetHeight(), PvPixelMono12);
				}
				// convert pixel format
				bufResult = myConverter.Convert( myBuffer, &myIntermediateBuffer, false );
				if ( !bufResult.IsOK() )
				{
					// release the buffer back to the pipeline
					myPipeline->ReleaseBuffer( myBuffer );

					throw CameraException(QString("Failed to convert from pixel format %1 to %2")
									  .arg(myBuffer->GetImage()->GetPixelType())
									  .arg(PvPixelMono16));
				}
			}
			else
			{
				// release the buffer back to the pipeline
				myPipeline->ReleaseBuffer( myBuffer );

				throw CameraException("Payload is not an image!");
			}
		}
		else
		{
			code = myBuffer->GetOperationResult().GetCode();

			if (code == PvResult::Code::NOT_INITIALIZED)
				errCode = 1;
			if (code == PvResult::Code::NOT_FOUND)
				errCode = 2;
			if (code == PvResult::Code::STATE_ERROR)
				errCode = 3;
			if (code == PvResult::Code::INVALID_DATA_FORMAT)
				errCode = 4;
			if (code == PvResult::Code::ABORTED)
				errCode = 5;
			if (code == PvResult::Code::NOT_ENOUGH_MEMORY)
				errCode = 6;
			if (code == PvResult::Code::GENERIC_ERROR)
				errCode = 7;
			if (code == PvResult::Code::INVALID_PARAMETER)
				errCode = 8;
			if (code == PvResult::Code::CANCEL)
				errCode = 9;
			if (code == PvResult::Code::PENDING)
				errCode = 10;
			if (code == PvResult::Code::TIMEOUT)
				errCode = 11;
			if (code == PvResult::Code::GENICAM_XML_ERROR)
				errCode = 12;
			if (code == PvResult::Code::ERR_OVERFLOW)
				errCode = 13;
			if (code == PvResult::Code::IMAGE_ERROR)
				errCode = 14;
			if (code == PvResult::Code::MISSING_PACKETS)
				errCode = 15;
			if (code == PvResult::Code::BUFFER_TOO_SMALL)
				errCode = 16;
			if (code == PvResult::Code::TOO_MANY_RESENDS)
				errCode = 17;
			if (code == PvResult::Code::RESENDS_FAILURE)
				errCode = 18;
			if (code == PvResult::Code::TOO_MANY_CONSECUTIVE_RESENDS)
				errCode = 19;
			if (code == PvResult::Code::AUTO_ABORTED)
				errCode = 20;
			if (code == PvResult::Code::BAD_VERSION)
				errCode = 21;
			if (code == PvResult::Code::NO_AVAILABLE_DATA)
				errCode = 22;
			if (code == PvResult::Code::NETWORK_ERROR)
				errCode = 23;
			if (code == PvResult::Code::THREAD_ERROR)
				errCode = 24;
			if (code == PvResult::Code::NO_MORE_ITEM)
				errCode = 25;

			// release the buffer back to the pipeline
			myPipeline->ReleaseBuffer( myBuffer );

			throw CameraReadException(QString("Read error - OpResult Code: %1 / %2").arg(code).arg(errCode));
		}
	}
	else if(bufResult.GetCode() == PvResult::Code::TIMEOUT)
	{
		throw CameraTimeoutException();
	}
	else
	{
		throw CameraException("Unknown error");
	}

	// release the buffer back to the pipeline
	myPipeline->ReleaseBuffer( myBuffer );

	//Scale from 12 to 16 bit
	cvConvertScale(unscaledFrame, newFrame, 16, 0);

	return Mat(newFrame, true);
}

void GoldeyePleora::reset(quint8 ms, bool pipeReset)
{
	//get images from buffer until it is empty
	PvResult lResult = myPipeline->RetrieveNextBuffer(&myBuffer, ms);
	while(lResult.IsOK())
	{
		//release the buffer back to the pipeline
		myPipeline->ReleaseBuffer( myBuffer );
		lResult = myPipeline->RetrieveNextBuffer(&myBuffer, ms);
	}

	if(pipeReset)
	{
		myPipeline->Reset();
	}
}
