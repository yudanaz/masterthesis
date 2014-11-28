#include "../io/camerainterface.h"

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

