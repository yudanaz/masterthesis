#include "imageprovider.h"

ImageProvider::ImageProvider(ImageSource *newSource)
{
	myImageSource = newSource;
	myStatus = false;
}

void ImageProvider::setImageSource(ImageSource *newSource)
{
	lockImageSource.lockForWrite();
	myImageSource = newSource;
	lockImageSource.unlock();
}

void ImageProvider::setRun(bool status)
{
	lockStatus.lockForWrite();
	myStatus = status;
	lockStatus.unlock();
}

void ImageProvider::process()
{
	MultispectralImage newFrame;
	bool run = true;
	quint8 errCount = 0;

	lockImageSource.lockForRead();

	if (myImageSource == NULL)
		return;

	do
	{
		//update image source
		try
		{
			newFrame = myImageSource->acquireNewFrame();

			emit newFrameReady(newFrame);

			if(errCount > 0)
				errCount--;

			lockStatus.lockForRead();
			run = myStatus;
			lockStatus.unlock();
		}
		catch (ImageSourceException e)
		{
			myLastError = e;

			if(errCount++ > 3)
			{
				run = false;
				emit acquisitionError(e);
			}
		}
	}
	while(run);

	lockImageSource.unlock();
}

