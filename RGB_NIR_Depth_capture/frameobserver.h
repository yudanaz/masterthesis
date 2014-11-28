#ifndef FRAMEOBSERVER_H
#define FRAMEOBSERVER_H

#include <QMutex>
#include <QQueue>
#include <VimbaCPP/Include/VimbaCPP.h>

using namespace AVT::VmbAPI;

/**
 * @brief The FrameObserver class is originally from Holger Stainer's goldeyeVimba.h and cpp files.
 * In this project it has been separated in order to be usable by the RGB camera class, e.g. prosilica.
 */
class FrameObserver : virtual public IFrameObserver
{
  public:
	FrameObserver( CameraPtr pCamera ) : IFrameObserver( pCamera ) {;}

	virtual void FrameReceived( const FramePtr pFrame );

	FramePtr getFrame();

	void clearFrameQueue();

	bool isFrameAvailable();

  private:
	QQueue<FramePtr> myFrames;
	QMutex myFramesMutex;
};

#endif // FRAMEOBSERVER_H
