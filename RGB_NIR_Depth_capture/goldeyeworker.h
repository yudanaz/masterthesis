#ifndef GOLDEYEWORKER_H
#define GOLDEYEWORKER_H

#include "imgacquisitionworker.h"

class GoldeyeWorker : public ImgAcquisitionWorker
{
public:
	GoldeyeWorker();
	~GoldeyeWorker();

public slots:
	void startAcquisition();

private:
	VimbaCamManager vimbaCamManager;
};

#endif // GOLDEYEWORKER_H
