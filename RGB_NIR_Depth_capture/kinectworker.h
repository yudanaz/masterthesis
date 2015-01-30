#ifndef KINECTWORKER_H
#define KINECTWORKER_H

#include"imgacquisitionworker.h"

class KinectWorker : public ImgAcquisitionWorker
{
public:
	KinectWorker();
	~KinectWorker();
	void triggerIRcapture();
	void switch_RGB_IR(bool captureRGB);
	bool isCapturingRGB();

public slots:
	void startAcquisition();

private:
	KinectCamManager kinectCamManager;
};

#endif // KINECTWORKER_H
