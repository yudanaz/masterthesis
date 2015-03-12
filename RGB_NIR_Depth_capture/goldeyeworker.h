#ifndef GOLDEYEWORKER_H
#define GOLDEYEWORKER_H

#include "imgacquisitionworker.h"

class GoldeyeWorker : public ImgAcquisitionWorker
{
	Q_OBJECT
public:
	GoldeyeWorker(int USBPortNr);
	~GoldeyeWorker();

public slots:
	void startAcquisition();
	void toggleMultiChannelCapture(bool captureMulti);

private:
	VimbaCamManager vimbaCamManager;
};

#endif // GOLDEYEWORKER_H
