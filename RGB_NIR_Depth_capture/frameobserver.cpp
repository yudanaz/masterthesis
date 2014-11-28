#include "frameobserver.h"

void FrameObserver::FrameReceived( const FramePtr pFrame )
{
	// Lock the frame queue
	myFramesMutex.lock();

	// Add frame to queue
	myFrames.append( pFrame );

	// Unlock frame queue
	myFramesMutex.unlock();
}

// Returns the oldest frame that has not been picked up yet
FramePtr FrameObserver::getFrame()
{
	FramePtr res;

	// Lock the frame queue
	myFramesMutex.lock();

	if (!myFrames.isEmpty())
	{
		// Pop frame from queue
		res = myFrames.first();
		myFrames.removeFirst();
	}

	// Unlock frame queue
	myFramesMutex.unlock();
	return res;
}

void FrameObserver::clearFrameQueue()
{
	// Lock the frame queue
	myFramesMutex.lock();

	// Clear the frame queue and release the memory
	myFrames.clear();

	// Unlock the frame queue
	myFramesMutex.unlock();
}

bool FrameObserver::isFrameAvailable()
{
	myFramesMutex.lock();

	bool res = ( myFrames.count() > 0 );

	myFramesMutex.unlock();

	return res;
}


