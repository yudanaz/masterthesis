#ifndef KINECTWORKER_H
#define KINECTWORKER_H

#include"imgacquisitionworker.h"

class KinectWorker : public ImgAcquisitionWorker
{
public:
	KinectWorker();
	~KinectWorker();
	void triggerIRcapture();

public slots:
	void startAcquisition();

private:
	KinectCamManager kinectCamManager;
};

#endif // KINECTWORKER_H
