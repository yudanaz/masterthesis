#include "../io/camerainterface.h"

CameraInterface::CameraInterface()
{
	//init error and status strings
	errorCodeStrings[ VmbErrorSuccess ] =        "Success.";
	errorCodeStrings[ VmbErrorInternalFault ] =  "Unexpected fault in VmbApi or driver.";
	errorCodeStrings[ VmbErrorApiNotStarted ] =  "API not started.";
	errorCodeStrings[ VmbErrorNotFound ] =       "Not found.";
	errorCodeStrings[ VmbErrorBadHandle ] =      "Invalid handle ";
	errorCodeStrings[ VmbErrorDeviceNotOpen ] =  "Device not open.";
	errorCodeStrings[ VmbErrorInvalidAccess ] =  "Invalid access.";
	errorCodeStrings[ VmbErrorBadParameter ] =   "Bad parameter.";
	errorCodeStrings[ VmbErrorStructSize ] =     "Wrong DLL version.";
	errorCodeStrings[ VmbErrorMoreData ] =       "More data returned than memory provided.";
	errorCodeStrings[ VmbErrorWrongType ] =      "Wrong type.";
	errorCodeStrings[ VmbErrorInvalidValue ] =   "Invalid value.";
	errorCodeStrings[ VmbErrorTimeout ] =        "Timeout.";
	errorCodeStrings[ VmbErrorOther ] =          "TL error.";
	errorCodeStrings[ VmbErrorResources ] =      "Resource not available.";
	errorCodeStrings[ VmbErrorInvalidCall ] =    "Invalid call.";
	errorCodeStrings[ VmbErrorNoTL ] =           "TL not loaded.";
	errorCodeStrings[ VmbErrorNotImplemented ] = "Not implemented.";
	errorCodeStrings[ VmbErrorNotSupported ] =   "Not supported.";

	frameStatusStrings[VmbFrameStatusComplete]   = "Frame has been completed without errors";
	frameStatusStrings[VmbFrameStatusIncomplete] = "Frame could not be filled to the end";
	frameStatusStrings[VmbFrameStatusTooSmall]   = "Frame buffer was too small";
	frameStatusStrings[VmbFrameStatusInvalid]    = "Frame buffer was invalid";
}

CameraInterface::~CameraInterface()
{
	;
}

quint16 CameraInterface::getImgWidth(){
	return myWidth;
}

quint16 CameraInterface::getImgHeight(){
	return myHeight;
}

quint8 CameraInterface::getImgDepth()
{
	return myDepth;
}

bool CameraInterface::isConnected()
{
	return connected;
}

bool CameraInterface::isConfigured()
{
	return configured;
}

QString CameraInterface::convErrToMsg( VmbErrorType eErr )
{
	if ( !errorCodeStrings.contains(eErr) )
	{
		return "Unknown error code.";
	}
	else
	{
		return errorCodeStrings.value(eErr);
	}
}

QString CameraInterface::convErrToMsg( VmbFrameStatusType eStatus )
{
	if ( !frameStatusStrings.contains(eStatus) )
	{
		return "Unknown status code.";
	}
	else
	{
		return frameStatusStrings.value(eStatus);
	}
}

