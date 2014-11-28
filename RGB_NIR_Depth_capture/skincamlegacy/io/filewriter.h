#ifndef FILEWRITER_H
#define FILEWRITER_H

#include <QString>
#include <QDir>
#include <QFileInfo>
#include <QFile>
#include <QTextStream>

#include <opencv2/opencv.hpp>

#include <cstdio>
#include <ctime>
#include <stdexcept>

#include "../core/skincamexception.h"

#include "../core/multispectralimage.h"

using namespace std;

/*!
 \brief Exception class indicating an error with the File Writer, derived
 from the general SkinCamException.
*/
class FileWriterException : public SkinCamException
{
public:
	/*!
	 \brief a constructor taking both an error message (which is given to the
	 super class constructor) and the file writer's target.

	 \param message is a QString with the error message.
	 \param target is a QString naming the target file.
	*/
	FileWriterException(QString message, QString target)
		: SkinCamException(message) { myTarget = target; }

	/*!
	 \brief returns the target file and path.

	 \return QString is the target file and path.
	*/
	QString getTarget() { return myTarget; }

private:
	QString myTarget; /*!< holds the target file name and path. */
};

/*!
 \brief a class specifying an interface and common methods to write images to
 files.

 This class is an interface for child classes implementing still image or
 video file writers. It defines methods to write MultispectralImage objects
 or openCV-images to a defined target file.
*/
class FileWriter
{
public:
	/*!
	 \brief a standard constructor. Will set a default filename for the target.
	*/
	FileWriter();

	/*!
	 \brief a constructor allowing to specify the target file.

	 \param file denotes the full path to the target file.
	*/
	FileWriter(QString file);

	/*!
	 \brief a constructor allowing to specify the target file and a list of the
	 images wavebands. The peak wavelengths of all channels will be converted
	 into text format and added to the filename of the target file.

	 Format: "(<channel_1>,<channel_2>,...,<channel_n>)", e.g. "(1060,1300,1550)"

	 \param file denotes the full path to the target file.
	 \param wavebands is an optional array containing the peak wavelengths of all
			channels.
	*/
	FileWriter(QString file, QList<qint16> wavebands);


	/**
	 * \brief simple destructor.
	 */
	virtual ~FileWriter();

	/*!
	 \brief writes a MultispectralImage object to the target file.

	 \param frame is the image to be written.
	*/
	virtual void writeMultispectral (MultispectralImage frame) = 0;

	/*!
	 \brief writes an Mat (openCV-format) to the target file.

	 \param frame is the image to be written.
	*/
	virtual void writeMatImage (Mat frame) = 0;

protected:
	/*!
	 \brief This method reads a given list of wavebands and creates a string
	 containing the wavebands' peak wavelengths, which is written to the class
	 variable "filenameMetadata".

	 \param wavebands a list of the image's wavebands.
	*/
	void encodeMetadata(QList<qint16> wavebands);

	/*!
	 \brief This method reads the metadata of an image from a MultispectralImage
	 object and writes them to a metadata.txt file in a given target directory.

	 \param targetDir is the target directory for the new metadata.txt file.
	 \param frame is the MultispectralImage object used as source for the metadata.
	 \param fps (optional) denotes the image source's framerate.
	*/
	void writeMetadataFile(QDir targetDir, MultispectralImage frame, quint8 fps = 0);

	QString targetFile;			/*!< is the file to which the image or video
									 will be written. */
	QString filenameMetadata;	/*!< is the encoded metadata string, which will
									 be attached to the filename. */
};

#endif // FILEWRITER_H
