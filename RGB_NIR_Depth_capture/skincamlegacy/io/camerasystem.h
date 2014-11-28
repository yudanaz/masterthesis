#ifndef CAMERASYSTEM_H
#define CAMERASYSTEM_H

#include <QString>
#include <QList>
#include <QMutex>
#include <QMutexLocker>
#include <QtDebug>

#include "imagesource.h"
#include "camerasystemconfig.h"
#include "flashlightcontrol.h"
#include "camerainterface.h"

#ifdef AVT_PLEORA
#include "goldeyePleora.h"
#endif
#ifdef AVT_VIMBA
#include "goldeyeVimba.h"
#endif

#include <iostream>

using namespace cv;

/*!
 \brief a class representing a camera system, which consists of a camera and
 a ringlight providing illumination in different (switchable) wavebands.

 The class implements an ImageSource and adds methods for the calibration of
 an actual camera's images.
*/
class CameraSystem : public ImageSource
{
public:
	/*!
	 \brief the default constructor, sets default values and dynamically creates
	 a default camera object (Goldeye P032) and a default flashlight object.
	*/
	CameraSystem();

	/*!
	 \brief destructor, will delete the dynamically created camera and flashlight
	 objects.
	*/
	~CameraSystem();

	/*!
	 \brief opens the connection to camera and ringlight and configures them.
	 The flashlight's serial port has to be given, the camera is searched on
	 the network automatically.

	 \param source is a string representing the address of the flashlight port.
	*/
	virtual void openSource (QString source);

	/*!
	 \brief grabs a new frame from the camera and returns it. Blocks until a
	 new frame is available.
	*/
	virtual MultispectralImage acquireNewFrame();

	/*!
	 \brief starts or stops the (autonomously working) camera acquisition.

	 \param doRun tells the camera system to start (true) or stop (false).
	*/
	void run (bool doRun);

	/*!
	 \brief returns the current camera configuration.

	 \return CameraSystemConfig
	*/
	CameraSystemConfig getConfig();

	/*!
	 \brief sets a new configuration, overwriting the currently active one.

	 \param newConfig is the new configuration.
	*/
	void setConfig(CameraSystemConfig newConfig);

protected:
	/*!
	 \brief initializes the MultispectralImage frame object (source specific)
	 by setting size, channel count, depth and wavebands used.
	*/
	void initializeFrame();

	/*!
	 \brief configures the ringlight according to the current camera system
	 configuration.
	*/
	void configureFlashlight();

	quint8 calcWavebandsValue(struct wavebands);


	CameraInterface		*myCamera;		/*!< interface to camera */
	FlashlightControl	*myFlashlight;	/*!< interface to flashlight */

	bool isOpen;					/*!< flag: status of connection. */
	bool isRunning;					/*!< flag: status of operation. */

	QMutex flashlightLock;			/*!< thread-safe lock for flashlight-access. */

private:
	CameraSystemConfig myConfig;	/*!< holds the configuration parameters. */
};

#endif // CAMERASYSTEM_H
