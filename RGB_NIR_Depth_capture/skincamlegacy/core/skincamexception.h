#ifndef SKINCAMEXCEPTION_H
#define SKINCAMEXCEPTION_H

#include <QString>
#include <QDir>

#include <stdexcept>

using namespace std;

/*!
 \brief This is a basic error/exception class.
*/
class SkinCamException
{
public:
	SkinCamException() {}

	/*!
	 \brief constructor requiring an error message as string.

	 \param message is the error message.
	*/
	SkinCamException(QString message)
	{
		myMessage = message;
	}

	/*!
	 \brief returns the error message as string.

	 \return string is the error message.
	*/
	QString getMessage()
	{
		return myMessage;
	}

	/*!
	 \brief returns the error message as cstring (char*), compliant to standard
	 exceptions.

	 \return const char
	*/
	const char* what() const
	{
		return myMessage.toStdString().c_str();
	}

protected:
	QString myMessage; /*!< stores the error message as string. */
};


#endif // SKINCAMEXCEPTION_H

