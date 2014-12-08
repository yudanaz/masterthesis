#ifndef CAMERAINTERFACE_H
#define CAMERAINTERFACE_H

#include <opencv2/opencv.hpp>
#include <VimbaCPP/Include/VimbaCPP.h>

#include "../core/skincamconfig.h"
#include "../io/imagesource.h"

using namespace AVT::VmbAPI;
using namespace std;

/*!
 \brief This is an error/exception class representing an error
 with the camera system.
*/
class CameraException : public ImageSourceException
{
public:
	CameraException(QString message) : ImageSourceException(message) {}
};

/*!
 \brief Specific CameraException, representing a timeout during frame acquisition.
*/
class CameraTimeoutException : public CameraException
{
public:
	CameraTimeoutException() : CameraException("Frame acquision timed out") {}
};

/*!
 \brief Specific CameraException, representing an error while reading image data.
*/
class CameraReadException : public CameraException
{
public:
	CameraReadException(QString message) : CameraException(message) {}
};

/*!
 \brief This acstract class represents an interface to an actual camera. It
 defines functions that need to be implemented by a child class, making the
 use of these functions independent from their actual implementation for a
 specific camera.
*/
class CameraInterface
{
public:
	CameraInterface();

	virtual ~CameraInterface();

	/*!
	 \brief opens a connection to the camera (automatically searching it).
	*/
	virtual void connect () = 0;

	/*!
	 \brief opens a connection to the camera at a given IP adress.

	 \param IPadress specifies the IP adress of a camera to connect to.
	*/
	virtual void connect (QString IPadress) = 0;

	/*!
	 \brief disconnects from the camera and resets the configuration.
	*/
	virtual void disconnect() = 0;

	/*!
	 \brief performs necessary configuration tasks.

	 \param bufferSize sets the number of frame buffers to use.
	*/
	virtual void configure (double exposureTime = 10.0, quint8 bufferSize = 3) = 0;

	/*!
	 \brief changes the cameras exposure time (if applicable).

	 \param ms specifies the exposure time in milliseconds.
	*/
	virtual void setExposureTime (double ms) = 0;

	/*!
	 \brief returns the maximum framerate of the camera for it's
	 current exposure time.

	 \return quint16 is the maximum possible framerate for the camera.
	*/
	virtual quint16 getMaxFPS() = 0;

	/*!
	 \brief returns a new frame from the camera after converting it into openCV
	 image format.

	 \return Mat is the new frame from the camera in openCV format.
	*/
	virtual Mat getCVFrame () = 0;

	/*!
	 \brief resets the camera and all image buffers.
	*/
#ifdef AVT_PLEORA
	virtual void reset (quint8 ms = 1, bool pipeReset = false) = 0;
#else
	virtual void reset () = 0;
#endif

	/*!
	 \brief returns the width of the images created by the actual camera.

	 \return quint16 image width.
	*/
	quint16 getImgWidth();

	/*!
	 \brief returns the height of the images created by the actual camera.

	 \return quint16 image height.
	*/
	quint16 getImgHeight();

	/*!
	 \brief returns the depth of the images created by the actual camera.

	 \return quint8 image depth.
	*/
	quint8 getImgDepth();

	//Methods from Maurice
	/*!
	 * \brief returns true if the camera has been successfully connected.
	 * \return a boolean connection state.
	 */
	bool isConnected();

	/*!
	 * \brief returns true if the camera has been successfully configured.
	 * \return a boolean configure state.
	 */
	bool isConfigured();
	//endof Methods from Maurice

	QString convErrToMsg( VmbErrorType eErr );
	QString convErrToMsg( VmbFrameStatusType eStatus );

	QMap<VmbErrorType, QString> errorCodeStrings;
	QMap<VmbFrameStatusType, QString> frameStatusStrings;

protected:
	quint16 myWidth;	/*!< width of the images captured by the camera. */
	quint16 myHeight;	/*!< height of the images captured by the camera. */
	quint8  myDepth;	/*!< depth of the images captured by the camera. */

	bool connected;		/*!< flag telling if the camera is connected. */
	bool configured;	/*!< flag telling if the camera is configured. */
};


#endif // CAMERAINTERFACE_H
