#ifndef GOLDEYE_H
#define GOLDEYE_H

#include <QString>
#include <QtDebug>

#include <iostream>

#include <PvSystem.h>
#include <PvInterface.h>
#include <PvDevice.h>
#include <PvString.h>
#include <PvGenParameterArray.h>
#include <PvGenParameter.h>
#include <PvStream.h>
#include <PvStreamRaw.h>
#include <PvBuffer.h>
#include <PvBufferConverter.h>
#include <PvPipeline.h>

#include "io/camerainterface.h"

using namespace std;

/*!
 \brief an implementation of the Camera class / interface for the AVT Goldeye
 P032 camera connected via Gigabit Ethernet (eBus SDK).

 This class uses the GigE / eBus SDK by Pleora.
*/
class GoldeyePleora : public CameraInterface
{
public:
	/*!
	 \brief constructor, setting default parameters and optionally setting
	 a flag enabling debug output.
	*/
	GoldeyePleora(bool setDebug = false);

	/*!
	 \brief destructor, releasing all dynamically created objects.
	*/
	~GoldeyePleora();

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

	 \param bufferSize sets the number of frame buffers to use.
	*/
	virtual void configure (double exposureTime = 10.0, quint8 bufferSize = 16);

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
	void setCorrectionDataset(quint8 intTime);

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

	 \return IplImage is the new frame from the camera in openCV format.
	*/
	Mat getCVFrame ();

	/*!
	 \brief resets the camera and all image buffers.

	 \param ms is the time in milliseconds that will be waited for a new buffer.
	 \param pipeReset is a flag denoting if the image pipe should be reset after reading.
	*/
	void reset(quint8 ms = 1, bool pipeReset = false);

private:
	// Related to camera connection
	PvSystem mySystem;			/*!< denotes the GEV network system */
	PvDeviceInfo *myDeviceInfo; /*!< interface info of the GEV device */
	PvDevice myDevice;			/*!< a GEV device */

	// Related to camera configuration
	PvGenParameterArray *myDeviceParams;/*!< configuration handler of a GEV device */
	PvStream myStream;					/*!< video stream from a GEV device */
	PvPipeline *myPipeline;				/*!< video data pipeline */

	// Related to frame acquisition
	PvBuffer *myBuffer;				/*!< frame buffer, coming from myPipeline */
	PvBuffer myIntermediateBuffer;	/*!< temporary used for image format conversion */
	PvBufferConverter myConverter;	/*!< converts the image format to be openCV compliant */

	IplImage* newFrame;		/*!< the new frame in 16bit openCV image format */
	IplImage* unscaledFrame;/*!< the new frame, unscaled (12 bit) */

	bool myGain;			/*!< flag holding status of highgain setting. */
	bool debug;				/*!< flag holding status of debug setting. */

	double exposureTime;	/*!< holds the currently used exposure time. */
};

#endif // GOLDEYE_H
