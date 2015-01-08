#include "imgacquisitionworker.h"

ImgAcquisitionWorker::ImgAcquisitionWorker() :
	acquiring(false),
	stopped(true)
{
}

ImgAcquisitionWorker::~ImgAcquisitionWorker()
{
}

void ImgAcquisitionWorker::setAcquiring(bool acquiring)
{
	this->acquiring = acquiring;
}

bool ImgAcquisitionWorker::isAcquiring()
{
	return this->acquiring;
}

void ImgAcquisitionWorker::stop()
{
	acquiring = false;
}

bool ImgAcquisitionWorker::isStopped()
{
	return stopped;
}


