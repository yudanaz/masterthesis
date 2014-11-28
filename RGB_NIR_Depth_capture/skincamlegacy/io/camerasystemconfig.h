#ifndef CAMERASYSTEMCONFIG_H
#define CAMERASYSTEMCONFIG_H

#include <QString>
#include <QList>
#include <QDir>
#include <QMap>

#include <stdint.h>

/*!
 \brief a structure defining the number, wavelengths and status of the
 flashlight's wavebands (including dark = 0).
*/
struct wavebands {
	quint8 totalNumber;			/*!< number of available wavebands. */
	quint8 usedNumber;			/*!< number of selected / used wavebands. */
	QMap<qint16, bool> config;	/*!< defines which wavebands are available and in use. */
};

/*!
 \brief a class representing a camera system configuration.
*/
class CameraSystemConfig
{
public:
	/*!
	 \brief a simple standard constructor.
	*/
	CameraSystemConfig();

	/*!
	 \brief Additional constructor, given a list of wavebands available.

	 \param bands are the ringlights wavebands.
	*/
	CameraSystemConfig(QList<qint16> bands);

	/*!
	 \brief copy-constructor, duplicating a given configuration.

	 \param aConfig configuration to be duplicated.
	*/
	CameraSystemConfig(const CameraSystemConfig &aConfig);

	/*!
	 \brief defines the behaviour of the "=" operator

	 \param aConfig the configuration that will be assigned to the first one
	 \return CameraSystemConfig &operator the configuration after assignment
	*/
	CameraSystemConfig& operator=( const CameraSystemConfig& aConfig );

	/*!
	 \brief standard destructor, deleting the dark reference image from memory.
	*/
	~CameraSystemConfig();

	QString ringlightInterface;	/*!< stores the device / COM port of the ringlight. */

	bool autoCameraSearch;		/*!< flag: search for camera automatically. */
	QString IPadress;			/*!< IP adress of the camera. */
	quint8 bufferSize;			/*!< Size of the cameras frame buffer. */

	quint8 frequency;			/*!< stores the trigger frequency (=framerate). */
	quint8 maxFPS;				/*!< stores the maximum framerate of the camera system. */
	double integrationTime;		/*!< stores the pulsewidth/exposure time in milliseconds. */

	struct wavebands wavebands; /*!< holds information about the available and
									 enabled wavebands. */

private:
	/*!
	 \brief sets default parameters for the Goldeye P032 and Ringlight4NIR.

	 \param bands is a list of the wavebands.
	*/
	void initialize(QList<qint16> bands);
};

#endif // CAMERASYSTEMCONFIG_H
