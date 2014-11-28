#ifndef LIBLINEARSTAGE_H
#define LIBLINEARSTAGE_H

#include <QObject>
#include <QList>

#include <stdexcept>
#include <string>
#include <sstream>

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

#include "../core/skincamexception.h"

using namespace std;

class LinearStageException : public SkinCamException
{
public:
	/*!
	 \brief default constructor, calling the default constructor of the super class.
	*/
	LinearStageException() : SkinCamException() {}

	/*!
	 \brief constructor with given error message, calling the constructor of the
	 super class with the message string as argument.

	 \param message is the error message as QString.
	*/
	LinearStageException(QString message) : SkinCamException(message) {}
};

/*!
 \brief This class provides an interface for an ISEL Linear Stage and implements
 methods to control it's movements by transmitting commands via a serial
 interface. It inherits from QObject and provides slots and signals that allow
 moving it to a QThread for parallel, non-blocking execution.
*/
class LinearStageControl : public QObject
{
	Q_OBJECT

public:
	/*!
	 \brief Standard constructor.
	*/
	LinearStageControl();

	/*!
	 \brief Destructor for cleaning u#include <QtSerialPort/QSerialPort>p.
	*/
	~LinearStageControl();

	/*!
	 \brief connects to the stage via given serial port.

	 \param port is the serial port the stage is connected to.
	*/
	void connect(QString port);

	/*!
	 \brief disconnects from the stage.
	*/
	void disconnect();

	/*!
	 \brief returns the current position of the stage in millimeters, measured
	 from the reference point.

	 \return qint16 current position in millimeters.
	*/
	qint16 getPosition();

	/*!
	 \brief synchronizes the current position variable held in this interface
	 to the the stage-internal position (step) counter.
	*/
	void syncPosition();

	/*!
	 \brief returns if the stage is already initialized or not.

	 \return bool is true if the stage is already initialized.
	*/
	bool getInitialized();

public slots:
	/*!
	 \brief initializes the stage.
	*/
	void initialize(QString port);

	/*!
	 \brief tells the stage to move a given distance in forwards (distance >0) or
	 backwards (distance <0) direction with the given speed.

	 \param distance is the distance to drive in millimeters.
	 \param speed is the speed the stage should use.
	*/
	void move(qint16 distance, quint16 speed = 10000);

	/*!
	 \brief tells the stage to move to a specific, absolute position.

	 \param position is the absolute target position in millimeters.
	 \param speed is the speed the stage should use.
	*/
	void moveTo(quint16 position, quint16 speed = 10000);

signals:
	/*!
	 \brief This signal is emitted if a command finished successfully.
	*/
	void finished();

	/*!
	 \brief This signal is emitted if an error occured during the execution
	 of a command.

	 \param LinearStageException describes the error.
	*/
	void errorOccured(LinearStageException error);

private:
	/*!
	 \brief tells the stage to drive to the reference (zero) position to reset
	 the internal position (step) counter.
	*/
	void gotoReference();

	/*!
	 \brief waits for the handshake message from the linear stage.
	*/
	void readHandshake(qint16 timeout = -1);

	/*!
	 \brief transmits a move command with an absolute position in step counts
	 to the linear stage.

	 \param stepCount is the absolute target position in steps.
	 \param speed is the speed the stage should use.
	*/
	void moveToSteps(quint32 stepCount, quint16 speed = 10000);

	/**
	# Length of linear axis: 302519
	# End: 301509
	# 1mm = 200 (202?) steps
	**/
	static const quint16 mmSteps = 200;		/*!< number of steps for 1mm */
	static const quint32 endPosition = 301509;	/*!< step count of end position */

	quint32 curPos;		/*!< current position in steps, internal only! */

#ifdef QSERIAL
	QSerialPort *serial;	/*!< serial interface to linear stage */
#else
	SerialPort *serial;
#endif

	bool isInitialized;	/*!< marks if the stage is already initialized. */
};

#endif // LIBLINEARSTAGE_H
