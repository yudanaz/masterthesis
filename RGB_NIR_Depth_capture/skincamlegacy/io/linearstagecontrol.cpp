#include "linearstagecontrol.h"

LinearStageControl::LinearStageControl()
{
	serial = NULL;

	curPos = -1;

	isInitialized = false;
}

LinearStageControl::~LinearStageControl()
{
	disconnect();
}

void LinearStageControl::connect(QString port)
{
	disconnect();

#ifdef QSERIAL
	serial = new QSerialPort(port);

	if(!serial->open(QSerialPort::ReadWrite))
		throw LinearStageException("Interface to linear stage could not be opened");

	if(!serial->setBaudRate(QSerialPort::Baud19200))
		throw LinearStageException("Error configuring flashlight interface: baudrate");
	if(!serial->setDataBits(QSerialPort::Data8))
		throw LinearStageException("Error configuring flashlight interface: data bits");
	if(!serial->setParity(QSerialPort::NoParity))
		throw LinearStageException("Error configuring flashlight interface: parity");
	if(!serial->setStopBits(QSerialPort::OneStop))
		throw LinearStageException("Error configuring flashlight interface: stop bits");
	if(!serial->setFlowControl(QSerialPort::NoFlowControl))
		throw LinearStageException("Error configuring flashlight interface: flow control");
#else
	try
	{
		serial = new SerialPort(port.toStdString());

		serial->Open(SerialPort::BAUD_19200, SerialPort::CHAR_SIZE_8,
					 SerialPort::PARITY_NONE, SerialPort::STOP_BITS_1,
					 SerialPort::FLOW_CONTROL_NONE);
	}
	catch (...)
	{
		throw LinearStageException("Interface to linear stage could not be opened!");
	}
#endif
}

void LinearStageControl::disconnect()
{
	if(serial != NULL)
	{
#ifdef QSERIAL
		if(serial->isOpen())
		{
			serial->close();
		}
#else
		if(serial->IsOpen())
		{
			serial->Close();
		}
#endif
		delete serial;
	}
}

qint16 LinearStageControl::getPosition()
{
	return (qint16)(curPos / mmSteps);
}

bool LinearStageControl::getInitialized()
{
	return isInitialized;
}

void LinearStageControl::syncPosition()
{
	ostringstream strValue;
	stringstream strConversion;

	try
	{
		//request current position info from stage
#ifdef QSERIAL
		if((serial->write("@0P\r") < 4) || !serial->waitForBytesWritten(500))
			throw LinearStageException("Could not write to stage");
#else
		try
		{
			serial->Write("@0P\r");
		}
		catch (...)
		{
			throw LinearStageException("Could not write to stage");
		}
#endif

		//wait for handshake
		readHandshake(1000);

		//read answer (6 bytes in hex)
		QByteArray ret;
#ifdef QSERIAL
		ret = serial->readAll();
		while(ret.length() < 6)
		{
			if(!serial->waitForReadyRead(500))
				throw LinearStageException("Timed out waiting for answer");

			ret.append(serial->readAll());
#else
		while(ret.length() < 6)
		{
			try
			{
				ret.append(serial->ReadByte(500));
			}
			catch (...)
			{
				throw LinearStageException("Timed out waiting for answer");
			}
#endif
		}

		for(int i = 0; i < 6; i++)
		{
			strValue << (char)ret.at(i);
		}
		//transfer string with hex-value into stringstream
		strConversion << std::hex << strValue.str();
		//read int-value from stringstream and overwrite current position
		strConversion >> curPos;
	}
	catch (LinearStageException)
	{
		throw;
	}
	catch (...)
	{
		throw LinearStageException("Error communicating with the stage");
	}
}

void LinearStageControl::initialize(QString port)
{
	if(isInitialized)
		return;

	try
	{
		//connect to given interface
		connect(port);

		//init device 0 as 1-axis system
#ifdef QSERIAL
		if((serial->write("@01\r") < 4) || !serial->waitForBytesWritten(500))
			throw LinearStageException("Could not write to stage");
#else
		try
		{
			serial->Write("@01\r");
		}
		catch (...)
		{
			throw LinearStageException("Could not write to stage");
		}
#endif

		readHandshake(1000);

		gotoReference();

		isInitialized = true;

		emit finished();
	}
	catch (LinearStageException e)
	{
		emit errorOccured(e);
	}
	catch (...)
	{
		emit errorOccured(LinearStageException("Error communicating with the stage"));
	}
}

void LinearStageControl::move(qint16 distance, quint16 speed)
{
	try
	{
		qint32 newPos = curPos + (distance * mmSteps);

		if (newPos < 0)
			newPos = 0;
		else if (newPos > endPosition)
			newPos = endPosition;

		moveToSteps(newPos, speed);

		emit finished();
	}
	catch (LinearStageException e)
	{
		emit errorOccured(e);
	}
}


void LinearStageControl::moveTo(quint16 position, quint16 speed)
{
	try
	{
		moveToSteps((position * mmSteps), speed);

		emit finished();
	}
	catch (LinearStageException e)
	{
		emit errorOccured(e);
	}
}

void LinearStageControl::gotoReference()
{
	try
	{
#ifdef QSERIAL
		//drive stage to reference (zero position)
		if((serial->write("@0R1\r") < 4) || !serial->waitForBytesWritten(500))
			throw LinearStageException("Could not write to stage");
#else
		try
		{
			serial->Write("@0R1\r");
		}
		catch (...)
		{
			throw LinearStageException("Could not write to stage");
		}
#endif

		//wait for completion

		readHandshake();

		//set current position to zero
		curPos = 0;

	}
	catch (LinearStageException)
	{
		throw;
	}
	catch (...)
	{
		throw LinearStageException("Error communicating with the stage");
	}
}

void LinearStageControl::readHandshake(qint16 timeout)
{
	QByteArray response;

#ifdef QSERIAL
	response = serial->readAll();

	if(response.length() == 0)
	{
		if(!serial->waitForReadyRead(timeout))
			throw LinearStageException("Timeout waiting for handshake from stage");

		response = serial->read(1);
	}
#else
	try
	{
		if(timeout < 0)
			timeout = 0;

		response.append(serial->ReadByte(timeout));
	}
	catch (...)
	{
		throw LinearStageException("Timeout waiting for handshake from stage");
	}
#endif

	if (response.at(0) != '0')
		throw LinearStageException("Incorrect handshake from stage");
}

void LinearStageControl::moveToSteps(quint32 stepCount, quint16 speed)
{
	ostringstream cmd;

	if(stepCount > endPosition)
	{
		stepCount = endPosition;
	}

	cmd << "@0M" << stepCount << "," << speed << "\r";

	try
	{
		//drive stage to given position
#ifdef QSERIAL
		if((serial->write(cmd.str().c_str()) < 7) || !serial->waitForBytesWritten(500))
			throw LinearStageException("Could not write to stage");
#else
		try
		{
			serial->Write(cmd.str());
		}
		catch (...)
		{
			throw LinearStageException("Could not write to stage");
		}

#endif

		//wait for completion - might take a while
		readHandshake();

		//set current position to target position
		curPos = stepCount;
	}
	catch (LinearStageException)
	{
		throw;
	}
	catch (...)
	{
		throw LinearStageException("Error communicating with the stage");
	}
}

