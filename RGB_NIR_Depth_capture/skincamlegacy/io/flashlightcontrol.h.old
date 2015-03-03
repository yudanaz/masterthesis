#ifndef FLASHLIGHTCONTROL_H
#define FLASHLIGHTCONTROL_H

#include <QString>
#include <QMutex>
#include <QMutexLocker>
#include <QtDebug>

#include <iostream>

//#define QSERIAL //uncomment if QSerialPort should be used in LINUX

#ifdef _WIN32
#ifndef QSERIAL
#define QSERIAL
#endif
#endif

#ifdef QSERIAL
#include <QtSerialPort/QSerialPort>
#else
#include <SerialPort.h>
#endif

#include "../io/imagesource.h"

using namespace std;

/*!
 \brief This is an error/exception class representing an error
 with the flashlight, derived from the general Image Source exception.
*/
class FlashlightException : public ImageSourceException
{
public:
	FlashlightException(QString message) : ImageSourceException(message) {}
};

/*!
 \brief a class implementing methods to interface the ring flashlight of an
 active multispectral camera system.
*/
class FlashlightControl
{
public:
	/*!
	 \brief the default constructor, configuring the available wavebands.
	*/
	FlashlightControl();

	/*!
	 \brief destructor, disconnecting the serial interface if still open.
	*/
	~FlashlightControl();

	/*!
	 \brief opens a serial interface connection to the flashlight.

	 \param port is the adress or port number of the serial interface.
	*/
	void connect (QString port);

	/*!
	 \brief close the connection to the flashlight.
	*/
	void disconnect ();

	/*!
	 \brief triggers one series of strobe pulses, one pulse for each enabled
	 waveband, to get one complete set of waveband images.
	*/
	void triggerSeries ();

	/*!
	 \brief triggers one single strobe pulse with the given waveband or without
	 illumination / dark (default).

	 \param waveband is the band to use for illumination (0 is dark [default])
	*/
	void triggerBand(quint8 waveband = 0);

	/*!
	 \brief turns on continouus illumination in the currently active waveband.
	 The ring light can be turned off again by calling stop().
	*/
	void continuous ();

	/*!
	 \brief starts the default working mode of the ring light: sending out
	 strobe pulses with switching wavebands (cycling through all enabled bands),
	 using the configured pulsewidth and frequency.
	*/
	void run ();

	/*!
	 \brief stops the current operating mode and turns off illumination.
	*/
	void halt ();

	/*!
	 \brief a method encapsulating the actual serial transmission.

	 \param cmd is a QString defining the command to be sent.
	 \param ack if true, requests and waits for an acknowledgement.
	*/
	void sendCommand (QString cmd, bool ack = false);

	/*!
	 \brief returns the "frame key", which denot#endifes the active waveband and
	 allows to relate each frame from the camera to the respective illumination.

	 \return quint8 is the active waveband.
	*/
	quint8 getFrameKey ();

	/*!
	 \brief returns a list of the available wavebands.

	 \return QList<qint16> contains all available wavebands.
	*/
	QList<qint16> getBands();

	/*!
	 \brief transmits an 'A' character to the flash light as acknowledgement.
	*/
	void sendAck();

private:
#ifdef QSERIAL
	QSerialPort *serial;
#else
	SerialPort *serial;
#endif

	QList<qint16> bands;	/*!< a list of the available wavebands. */

};

#endif // FLASHLIGHTCONTROL_H
