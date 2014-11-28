#ifndef IMAGEFILEREADER_H
#define IMAGEFILEREADER_H

#include "../io/filereader.h"

#include <QProcess>

using namespace std;

/*!
 \brief an implementation of FileReader, allowing to read still images from
 files and convert them to MultispectralImage objects.
*/
class ImageFileReader : public FileReader
{
public:
	/*!
	 \brief default constructor.
	*/
	ImageFileReader();

	/*!
	 \brief a constructor allowing to specify calibration settings for the
	 image source / file reader.

	 \param settings are the calibration settings to be used.
	*/
	ImageFileReader(struct calibrationSettings settings);

	/*!
	 \brief destructor
	*/
	~ImageFileReader();

	/*!
	 \brief opens the file at the given path. Will use openCVs imread() for
	 JPG, PNG or BMP files and decodes the multispectral channels contained in
	 false color or side by side images. Will also read multispectral image
	 archives (.tar) or folders containing a "metadata.txt" file.

	 \param source is the full path to the image file.
	*/
	virtual void openSource (QString source);

	/*!
	 \brief here, this method simply returns the decoded multispectral image.
	*/
	virtual MultispectralImage acquireNewFrame();

private:
	/*!
	 \brief creates a new MultispectralImage object from a given filename, which
	 points to a respective image archive (.tar) or folder (containig
	 metadata.txt).

	 \param source path to the multispectral image archive.
	*/
	void readMultispectral(QString source);

	MultispectralImage origImage;
};

#endif // IMAGEFILEREADER_H
