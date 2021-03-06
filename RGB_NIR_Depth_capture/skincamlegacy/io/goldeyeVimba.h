#ifndef GOLDEYEVIMBA_H
#define GOLDEYEVIMBA_H

#include <QString>
#include <QtDebug>
#include <QMap>
//#include <QMutex>
//#include <QQueue>

#include <unistd.h>

#include "../io/camerainterface.h"
#include "frameobserver.h"

#include <VimbaCPP/Include/VimbaCPP.h>

using namespace AVT::VmbAPI;


/*!
 \brief an implementation of the Camera class / interface for the AVT Goldeye
 P032 camera connected via Gigabit Ethernet.

 This class uses the AVT Vimba SDK.
*/
class GoldeyeVimba : public CameraInterface
{
public:
	/*!
	 \brief constructor, setting default parameters and optionally setting
	 a flag enabling debug output.
	*/
	GoldeyeVimba();

	/*!
	 \brief destructor, releasing all dynamically created objects.
	*/
	virtual ~GoldeyeVimba();

	/*!
	 \brief opens a connection to the camera.
	*/
	virtual void connect ();

	/*!
	 \brief opens a connection to the camera at a specified IP adress.

	 \param IPaddress is the adress of the camera to which to connect.
	*/
	virtual void connect (QString IPaddress);

	/*!
	 \brief disconnects from the camera and resets the configuration.
	*/
	virtual void disconnect();

	/*!
	 \brief performs necessary configuration tasks.

	 \param bufferSize sets the number of frame buffers to use (3 by default).
	*/
	virtual void configure (double exposureTime = 10.0, quint8 bufferSize = 4);

	/*!
	 \brief changes the cameras exposure time (if applicable).

	 \param ms specifies the exposure time in milliseconds.
	*/
	virtual void setExposureTime (double ms);

	/*!
	 \brief returns the maximum framerate of the camera for it's
	 current exposure time.

	 \return quint16 is the maximum possible framerate for the camera.
	*/
	virtual quint16 getMaxFPS();

	/*!
	 \brief set the correction data set (stored internally in the camera)
	 according to the given integration time or pulsewidth, respectively.

	 \param intTime integration time in milliseconds.
	*/
	void setCorrectionDataset(double intTime);

	/*!
	 \brief sets the status of the high gain switch.

	 \param setGain true enables high gain.
	*/
	void setHighgain(bool setGain);

	/*!
	 \brief returns the status of the high gain switch.

	 \return bool true if high gain is enabled.
	*/
	bool getHighgain();

	/*!
	 \brief returns a new frame from the camera after converting it into openCV
	 image format. This is done by mapping the actual image data into an
	 IplImage.

	 \return Mat is the new frame from the camera in openCV format.
	*/
	virtual Mat getCVFrame ();

	/*!
	 \brief resets the camera and all image buffers.

	 \param ms is the time in milliseconds that will be waited for a new buffer.
	 \param pipeReset is a flag denoting if the image pipe should be reset after reading.
	*/
	virtual void reset ();

	//Methods by Maurice
	virtual void triggerViaSoftware();
	bool is_P_model(){ return isPmodel; }
	//endof methods by Maurice


private:
	VimbaSystem			&system;
	CameraPtr			pGoldeye;
	IFrameObserverPtr	myFrameObserver;

	bool isPmodel;
	quint8 myBufferSize;

	bool myGain; /*!< flag holding status of highgain setting. */
};


#endif // GOLDEYEVIMBA_H
