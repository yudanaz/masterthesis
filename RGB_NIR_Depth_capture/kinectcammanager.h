#ifndef KINECTCAMMANAGER_H
#define KINECTCAMMANAGER_H

#include"cammanager.h"
#include"libfreenect.hpp"
#include"myfreenectopencvdevice.h"

class KinectCamManager : public CamManager
{
public:
	KinectCamManager();

	void connectCameras();
	void closeCameras();
	void getImages(QMap<RGBDNIR_captureType, Mat> &camImgs);
	void triggerIRcapture();
	void switch_RGB_IR(bool captureRGB);

	bool isConnected();
	bool isCapturingRGB(){ return capturingRGB; }

private:
	Freenect::Freenect freenect;
	MyFreenectOpenCVDevice *freenectDevice;
	bool connected;
	bool captureOneIrFrame;
	bool queue_switchRGB2IR;
	bool queue_switchIR2RGB;
	bool capturingRGB;
};

#endif // KINECTCAMMANAGER_H
