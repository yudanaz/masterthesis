#ifndef KINECTWORKER_H
#define KINECTWORKER_H

#include"imgacquisitionworker.h"

class KinectWorker : public ImgAcquisitionWorker
{
public:
	KinectWorker();
	~KinectWorker();

public slots:
	void startAcquisition();

private:
	KinectCamManager kinectCamManager;
};

#endif // KINECTWORKER_H
