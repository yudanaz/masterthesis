#include "flashlightcontrol.h"

FlashlightControl::FlashlightControl()
{
	serial = NULL;

	bands << 935 << 1060 << 1300 << 1550;
}

FlashlightControl::~FlashlightControl()
{
	disconnect();
}

void FlashlightControl::connect(QString port)
{
	disconnect();

#ifdef QSERIAL
	serial = new QSerialPort(port);

	if(!serial->open(QIODevice::ReadWrite))
		throw FlashlightException("Interface to flashlight could not be opened");

	if(!serial->setBaudRate(QSerialPort::Baud38400))
		throw FlashlightException("Error configuring flashlight interface: baudrate");
	if(!serial->setDataBits(QSerialPort::Data8))
		throw FlashlightException("Error configuring flashlight interface: data bits");
	if(!serial->setParity(QSerialPort::NoParity))
		throw FlashlightException("Error configuring flashlight interface: parity");
	if(!serial->setStopBits(QSerialPort::TwoStop))
		throw FlashlightException("Error configuring flashlight interface: stop bits");
	if(!serial->setFlowControl(QSerialPort::NoFlowControl))
		throw FlashlightException("Error configuring flashlight interface: flow control");
#else
	try
	{
		serial = new SerialPort(port.toStdString());

		serial->Open(SerialPort::BAUD_38400, SerialPort::CHAR_SIZE_8,
					 SerialPort::PARITY_NONE, SerialPort::STOP_BITS_2,
					 SerialPort::FLOW_CONTROL_NONE);
	}
	catch (...)
	{
		throw FlashlightException("Interface to flashlight could not be opened!");
	}
#endif

	sendAck();
	halt();

	qDebug() << "Flashlight connected on " << port;
}

void FlashlightControl::disconnect()
{
	if(serial != NULL)
	{
#ifdef QSERIAL
		if(serial->isOpen())
		{
			halt();
			serial->close();
		}
#else
		if(serial->IsOpen())
		{
			try
			{
				halt();
			}
			catch (...)
			{
				qDebug() << "Ringlight did not react on disconnect";
			}
			serial->Close();
		}
#endif
		delete serial;
	}
}

void FlashlightControl::triggerSeries()
{
	try
	{
		sendCommand("s", false);
	}
	catch (FlashlightException)
	{
		throw;
	}
}

void FlashlightControl::triggerBand(quint8 waveband)
{
	try
	{
		sendCommand(QString("b%1;").arg(waveband), false);
	}
	catch (FlashlightException)
	{
		throw;
	}
}

void FlashlightControl::continuous()
{
	try
	{
		sendCommand("c", true);
	}
	catch (FlashlightException)
	{
		throw;
	}
}

void FlashlightControl::run()
{
	try
	{
		sendCommand("r", true);
	}
	catch (FlashlightException)
	{
		throw;
	}
}

void FlashlightControl::halt()
{
	try
	{
		sendCommand("h", true);
	}
	catch (FlashlightException)
	{
		throw;
	}
}

QList<qint16> FlashlightControl::getBands()
{
	return bands;
}

void FlashlightControl::sendCommand(QString cmd, bool ack)
{
	QByteArray response;

#ifdef QSERIAL
	if((serial->write(cmd.toStdString().c_str()) < 1)
			|| !serial->waitForBytesWritten(500))
		throw FlashlightException("Could not send command to flashlight.");
#else
	try
	{
		serial->Write(cmd.toStdString());
	}
	catch (...)
	{
		throw FlashlightException("Flashlight could not send command.");
	}
#endif

	if(ack)
	{
		char a = 0;

		while(a != 'A')
		{
#ifdef QSERIAL
			response = serial->read(1);
			if(response.length() > 0)
			{
				a = response.at(0);
			}
			else
			{
				if(!serial->waitForReadyRead(500))
					throw FlashlightException("Did not receive acknowledgment from flashlight");
			}
#else
			try
			{
				a = serial->ReadByte(500);
			}
			catch (...)
			{
				throw FlashlightException("Did not receive acknowledgment from flashlight");
			}
#endif
		}
	}
}

void FlashlightControl::sendAck()
{
#ifdef QSERIAL
	if((serial->write("A", 1) < 1) || !serial->waitForBytesWritten(100))
		throw FlashlightException("Could not write ack to flashlight");
#else
	serial->WriteByte('A');
#endif
}

quint8 FlashlightControl::getFrameKey()
{
//	SerialPort::DataBuffer buffer;
	char key = 'A';
	QByteArray response;

#ifdef QSERIAL
	if(!serial->isOpen())
#else
	if(!serial->IsOpen())
#endif
		throw FlashlightException("Flashlight not connected");

	//wait for key
	while((key < '0') ||  (key > '4'))
	{
#ifdef QSERIAL
		response = serial->read(1);
		if(response.length() > 0)
		{
			key = response.at(0);
		}
		else
		{
			if(!serial->waitForReadyRead(500))
				throw FlashlightException("Timed out waiting for frame key");
		}
#else
		try
		{
			key = serial->ReadByte(1000);
		}
		catch (...)
		{
			throw FlashlightException("No frame key received");
		}
#endif
	}

	return key-48;
}

