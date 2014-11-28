#ifndef FILEREADER_H
#define FILEREADER_H

#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QList>
#include <QtDebug>

#include "../io/imagesource.h"
#include "../core/multispectralimage.h"


/*!
 \brief This class represents an exception of the file reader and is derived
 from the more general Image Source exception. In addition to the error message,
 it takes a string naming the image source as argument and provides a method
 to return the source.
*/
class FileReaderException : public ImageSourceException
{
public:
	/*!
	 \brief Constructor taking the error message and the image source as
	 arguments.

	 \param message is a string containing the error message.
	 \param source is a string naming the image source.
	*/
	FileReaderException(QString message, QString source = "")
		: ImageSourceException(message) { mySource = source; }

	/*!
	 \brief returns the string denoting the image source.

	 \return QString denotes the image source.
	*/
	QString getSource() { return mySource; }


	/*!
	 \brief sets the image source as string.

	 \param source a string denoting the image source.
	*/
	void setSource(QString source) { mySource = source; }

private:
	QString mySource; /*!< denotes the iamge source. */
};

using namespace cv;

/*!
 \brief an abstract class defining an interface for an image or video file reader.

 The class FileReader contains some methods that are common for both still image
 and video files. Additionally, it defines virtual methods that need to be
 implemented by classes representing actual image or video file readers.
*/
class FileReader : public ImageSource
{
public:
	/*!
	 \brief a simple constructor.
	*/
	FileReader();

	FileReader(struct calibrationSettings settings);

	/*!
	 \brief a destructor.
	*/
	virtual ~FileReader();

	/*!
	 \brief opens the image or video file.
	 Needs to be implemented accordingly by a respective image or video reader.

	 \param source is a string representing the full path of the source file.
	*/
	virtual void openSource (QString source) = 0;

	/*!
	 \brief acquires and returns a new frame. Needs to be implemented
	 accordingly by an actual image or video file reader.
	*/
	virtual MultispectralImage acquireNewFrame() = 0;

protected:
	/*!
	 \brief initializes the MultispectralImage frame object (source specific)
	 by setting size, channel count, depth and wavebands used. Takes an openCV
	 (Mat) image and a string denoting the source as arguments. Metadata is
	 extracted out of the filename, if existing.

	 \param srcImage is the source image in Mat format.
	 \param srcName is the string denoting the source.
	*/
	void initializeFrame(Mat srcImage, QString srcName = "");

	/*!
	 \brief decodes the given Mat image. Resulting MultispectralImage object
	 is stored internally as a class variable of ImageSource.

	 \param srcImage contains the image to be decoded.
	*/
	void decodeCVImage(Mat srcImage);

	/*!
	 \brief creates a MultispectralImage out of a "false color" image with
	 3 channels (+1, if dark not given explicitly).

	 \param srcImage is the false color image used as image source.
	*/
	void decodeFalseColorImage(Mat srcImage);

	/*!
	 \brief creates a MultispectralImage out of a "side by side" image with
	 up to 6 channels (+1, if dark not given explicitly).

	 \param srcImage is the side by side image used as image source.
	*/
	void decodeSideBySideImage (Mat srcImage);

	/*!
	 \brief extracts the metadata (string) from the source path and reads the
	 peak wavelengths of all channels of an image.

	 \param source is the path to the source file.
	 \return QList containing the wavebands (extracted metadata) of the image.
	*/
	QList<qint16> decodeFilenameMetadata(QString source);

	/*!
	 \brief extracts the metadata from a given metadata-file and configures the
	 FileReader's image frame object accordingly.

	 \param source is the path to the metadata file.
	*/
	void decodeMultispectralMetadata(QString source);

	bool darkGiven; /*!< flag indicating if a dark channel is included in
						 the source file. */

	QList<qint16> wavebandsFromMetadata;
};

#endif // FILEREADER_H
