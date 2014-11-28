#ifndef IMAGESOURCE_H
#define IMAGESOURCE_H

#include <QReadWriteLock>
#include <QString>
#include <QList>
#include <QStringList>
#include <QMap>
#include <QMapIterator>
#include <QProcess>

#include <QTime>
#include <QDebug>

#include <opencv2/opencv.hpp>

#include "../core/multispectralimage.h"
#include "../core/skincamexception.h"
#include "../core/skincamconfig.h"

//#include <cstdio>

using namespace std;

/*!
 \brief struct holding the linear correction parameters (scale and offset)
 in the form of two matrices.
*/
struct linCorr
{
	quint16 count;
	Mat *refImages;
	Mat *offsets;
	Mat *scales;
};


/*!
 \brief This is an error/exception class representing an error with the image
 source. Derived from the general SkinCamException.
*/
class ImageSourceException : public SkinCamException
{
public:
	/*!
	 \brief default constructor, calling the default constructor of the super class.
	*/
	ImageSourceException() : SkinCamException() {}

	/*!
	 \brief constructor with given error message, calling the constructor of the
	 super class with the message string as argument.

	 \param message is the error message as QString.
	*/
	ImageSourceException(QString message) : SkinCamException(message) {}
};

/*!
 \brief an abstract (pure virtual) class defining an interface for image sources.

 The class ImageSource contains some methods and data structures that are common
 for every type of image source. Additionally, it defines pure virtual methods
 that need to be implemented by classes representing actual image sources.
*/
class ImageSource
{
public:
	/*!
	 \brief a simple constructor.
	*/
	ImageSource();

	/*!
	 \brief additional constructor, allowing to specify calibration settings.

	 \param settings are the new default calibration settings.
	*/
	ImageSource(struct calibrationSettings settings);

	/*!
	 \brief a simple destructor.
	*/
	virtual ~ImageSource();

	/*!
	 \brief opens the image source, which might be either a file or camera.
	 Needs to be implemented accordingly by an actual image source.

	 \param source is a string representing the device or filename.
	*/
	virtual void openSource (QString source) = 0;

	/*!
	 \brief acquires a new frame. Needs to be implemented accordingly by
	 an actual source.
	*/
	virtual MultispectralImage acquireNewFrame() = 0;

	/*!
	 \brief returns the number of channels of the image source.
	 Needs to be implemented accordingly by an actual source.

	 \return quint8 is the source's channel count.
	*/
	quint8 getChannels();

	/*!
	 \brief returns the number of channels of the image source.
	 Needs to be implemented accordingly by an actual source.

	 \return quint8 is the source's channel count.
	*/
	QList<qint16> getWavebands();

	/*!
	 \brief returns the image source's framerate.

	 \return quint8 the framerate.
	*/
	quint8 getFPS();

	/*!
	 \brief

	 \param newCal
	*/
	void setImageCalibration(struct calibrationSettings newCal);

	/*!
	 \brief returns the current calibration settings for the image source.

	 \return imageCalibration structure containing all calibration info.
	*/
	struct calibrationSettings getCurrentCalibration();

	/*!
	 \brief calculates calibration coefficients to compensate for different
	 intensity levels between the channels of a multispectral image using a
	 white reference visible on a given image at a given position.

	 \param refPos is the position of the white reference in the image.
	*/
	void setWhiteReference (CvPoint refPos);

	/*!
	 \brief returns if the (original) image directly from the source is already
	 fixed pattern noise corrected, e.g. if an image file is loaded.

	 \return bool is true if the source provides FPN corrected images.
	*/
	bool getSourceIsFPNCorr();

	/*!
	 \brief returns if the (original) image directly from the source already
	 has the dark image subtracted from the other channels, e.g. if an image
	 file is loaded.

	 \return bool is true if dark is already subtracted.
	*/
	bool getSourceIsDarkSub();

	/*!
	 \brief returns if the (original) image directly from the source is already
	 white reference calibrated, e.g. if an image file is loaded.

	 \return bool is true if the source provides white calibrated images.
	*/
	bool getSourceIsWhiteCal();

	/*!
	 \brief returns the name of the source / file.
	*/
	QString getSourceName();

protected:
	/*!
	 \brief calibrates different intensity levels between the channels of a
	 multispectral image.
	*/
	void applyWhiteCalibration ();

	/*!
	 \brief calibrates the fixed pattern noise of a camera visible on dark
	 or very bright areas of an image using a given image, for example
	 acquired with completely closed apperture.

	 \param image is the Multispectral or single channel image to be fixed.
	*/
	void applyFPNCalibration(Mat image);
	void applyFPNCalibration(MultispectralImage image);

	/*!
	 \brief opens the FPN reference file specified in CalibrationSettings and
	 loads the values into the matrix structure myFPNcorr.
	*/
	void loadFPNReference();

	quint8 myFPS;				/*!< stores the framerate of the image source. */

	MultispectralImage myFrame; /*!< stores the current frame. */

	QString mySourceName;

	struct linCorr myFPNcorr;	/*!< dark reference image for calibration. */
	bool myFPNcorrLoaded;		/*!< flag is true if correction matrix has been created. */

	struct calibrationSettings myCalSettings; /*!< holds the active calibration parameters */
	QReadWriteLock calLock;		/*!< thread-safe lock for calibration parameters. */

	bool sourceIsFPNCorr;		/*!< stores whether the image source was already FPN corrected. */
	bool sourceIsDarkSub;		/*!< stores whether the image source was already dark subtracted. */
	bool sourceIsWhiteCal;		/*!< stores whether the image source was already white calibrated. */
};

#endif // IMAGESOURCE_H
