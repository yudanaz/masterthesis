#ifndef IMAGEFILEWRITER_H
#define IMAGEFILEWRITER_H

#include <QProcess>

#include "../io/filewriter.h"

using namespace cv;

/*!
 \brief a class implementing the FileWriter interface, allowing to
 write single images to a file.

 This class implements the virtual methods writeMatImage() and
 writeMultispectral(). The first method will create a new PNG image file with a
 timestamp added to the defined filename each time it is called. The second
 method will create a tar-archive containing one PNG file for each spectral band
 with a timestamp added to the defined filename.
*/
class ImageFileWriter : public FileWriter
{
public:
	/*!
	 \brief a standard constructor.

	 The constructor configures the settings for images that are written. They
	 will use PNG format with a medium (5) compression level.
	*/
	ImageFileWriter();

	/*!
	 \brief a constructor defining the path to the target file.

	 The constructor also configures the settings for images that are written.
	 They will use PNG format with a medium (5) compression level.

	 \param path is the full path of the target file.
	*/
	ImageFileWriter(QString path);

	/*!
	 \brief a constructor allowing to specify the target file and a list of the
	 images wavebands. Will throw an exception if the path is not valid.
	 The peak wavelengths of all channels will be converted into text format
	 and added to the filename of the target file.

	 Format: "(<channel_1>,<channel_2>,...,<channel_n>)", e.g. "(1060,1300,1550)"

	 \param file denotes the full path to the target file.
	 \param wavebands is an optional array containing the peak wavelengths of all
			channels.
	*/
	ImageFileWriter(QString file, QList<qint16> wavebands);

	/*!
	 \brief writes a MultispectralImage object to the target file, which will
	 be a tar-archive containing one image in PNG format for each channel and
	 a text file with metadata.

	 \param frame is the image to be written.
	*/
	void writeMultispectral (MultispectralImage frame);

	/*!
	 \brief writes an Mat image (openCV-format) to the target file in PNG format.

	 \param frame is the image to be written.
	*/
	void writeMatImage (Mat frame);

private:
	vector<int> imageSettings; /*!< holds settings for cv::imwrite */
};

#endif // IMAGEFILEWRITER_H
