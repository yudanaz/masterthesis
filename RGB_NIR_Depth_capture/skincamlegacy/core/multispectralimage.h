#ifndef MULTISPECTRALIMAGE_H
#define MULTISPECTRALIMAGE_H

#include <QList>
#include <QMap>

#include <opencv2/opencv.hpp>

#include "../core/skincamexception.h"

using namespace cv;

/*!
 \brief This class represents an exception which is thrown when a new image or
 frame has a different size than excpected.
*/
class ImageSizeException : public SkinCamException
{
public:
	/*!
	 \brief constructor, taking the new and the old size as arguments

	 \param newS CvSize property of the new image / frame.
	 \param hasS CvSize property of the old image / frame.
	*/
	ImageSizeException ( CvSize newS, CvSize hasS )
	{
		newSize = newS;
		hasSize = hasS;
		myMessage = "New image has wrong size!";
	}

	/*!
	 \brief returns the (wrong) size of the new image.

	 \return CvSize size of the new image.
	*/
	CvSize getNewSize() { return hasSize; }

	/*!
	 \brief returns the expected size.

	 \return CvSize size of the old image.
	*/
	CvSize getHasSize() { return newSize; }

private:
	CvSize newSize; /*!< holds the (wrong) size of the new image. */
	CvSize hasSize; /*!< holds the expected image size. */
};


/*!
 \brief exception representing an error during image handling.
*/
class ImageHandlingException : public SkinCamException
{
public:
	ImageHandlingException(QString message = "") : SkinCamException(message) { }
};


/*!
 \brief A class representing a multispectral image.

 The MultispectralImage class represents a multispectral image, which consists
 of several channels (typically more than one in order to be truly multi-
 spectral).
*/
class MultispectralImage
{
public:
	/*!
	 \brief A standard constructor, not doing anything.
	*/
	MultispectralImage();

	/*!
	 \brief A constructor including all important parameters, but no image data.

	 \param newWidth defines the width of the image.
	 \param newHeight defines the height of the image.
	 \param newDepth defines the number of bits per channel (i.e. color depth).
	*/
	MultispectralImage( quint16 newWidth,
						quint16 newHeight,
						quint8 newDepth );

	/*!
	 \brief A copy-constructor, which creates a copy of a given instance.
	 If specified, the contained images will be cloned, by default they won't.

	 \param source is an existing instance of MultispectralImage.
	 \param clone is a flag specifying if contained Mat images shall be cloned.
	*/
	MultispectralImage( const MultispectralImage& source, bool clone = false );

	/*!
	 \brief defines the behaviour of the "=" operator, does not copy the data!

	 \param img the image that will be assigned to the first one
	 \return MultispectralImage &operator the image after assignment
	*/
	MultispectralImage& operator=( const MultispectralImage& img );

	/*!
	 \brief a destructor, clearing the images and wavelengths lists.
	*/
	~MultispectralImage();

	/*!
	 \brief returns the image of a single channel as a pointer. Does NOT create
	 a copy of the image!

	 \param wl defines the channel waveband, the image of which is returned.
	 \return Mat is the image corresponding to the selected channel.
	*/
	Mat getImageByWL (qint16 wl);

	/*!
	 \brief returns the image of a single channel as a pointer. Does NOT create
	 a copy of the image!

	 \param channel defines the channel number, the image of which is returned.
	 \return Mat is the image corresponding to the selected channel.
	*/
	Mat getImageByChannelNumber (qint8 channel);

	/*!
	 \brief returns the contained images of all wavebands as a map.

	 \return QMap<qint16, Mat> map containing all wavebands and their images.
	*/
	QMap<qint16, Mat> getImages();

	/*!
	 \brief stores a new image for a given channel. DOES create a copy of
	 the given image.

	 \param channel defines the channel for which a new image is given.
	 \param newImage is the new image.
	*/
	void setChannelImage (qint16 channel, Mat newImage);

	/*!
	 \brief create a side by side image out of a multispectral image object.

	 \param channels is a list containing the multispectral channels to be used.
	 \return Mat is the created openCV image.
	*/
	Mat getSideBySideImage (QList<qint16> channels);

	/*!
	 \brief create a false color image out of 3 channels of a multispectral
	 image object. The image will be converted to a different color depth, if given.

	 \param channels is a list containing the multispectral channels to be used.
	 \return Mat is the created openCV image.
	*/
	Mat getFalseColorImage (QList<qint16> channels);

	/*!
	 \brief returns the dark image of a multispectral image object in openCV
	 format. The image will be converted to a different color depth, if given.

	 \return Mat is the created openCV image.
	*/
	Mat getDarkImage ();

	/*!
	 \brief returns a map containing the values of a given pixel for all channels.

	 \param x x-coordinate of the pixel.
	 \param y y-coordinate of the pixel.
	 \return QMap is a map of channel / pixel value pairs.
	*/
	QMap<qint16, quint16> getPixelValueMap(quint16 x, quint16 y);

	/*!
	 \brief returns a pointer to an array containing the values of a
	 given pixel for all channels - faster than getPixelValueMap().

	 \param x x-coordinate of the pixel.
	 \param y y-coordinate of the pixel.
	 \return QList is a list of pixel values.
	*/
	QList<quint16> getPixelValueList(quint16 x, quint16 y);

	/*!
	 \brief returns the image's width.

	 \return quint16 the image's width.
	*/
	quint16 getWidth();

	/*!
	 \brief returns the image's height.

	 \return quint16 the image's height.
	*/
	quint16 getHeight();

	/*!
	 \brief sets width and height of the image.

	 \param width is the new width to be set.
	 \param height is the new heigth to be set.
	*/
	void setSize(quint16 width, quint16 height);

	/*!
	 \brief returns the image's color depth.

	 \return quint8 the image's color depth.
	*/
	quint8 getDepth();

	/*!
	 \brief sets the color depth of the image.

	 \param depth is the new depth to be set.
	*/
	void setDepth(quint8 depth);

	/*!
	 \brief returns the image's channel count.

	 \return quint8 the image's channel count.
	*/
	quint8 getChannelCount();

	/*!
	 \brief returns a list of the channels' peak wavelengths.

	 \return QList<qint16> a list of the channels' peak wavelengths
	*/
	QList<qint16> getWavebands();

	/*!
	 \brief marks that fixed pattern noise has been corrected in this image.

	 \param en sets true or false.
	*/
	void setFPNCorrected(bool en);

	/*!
	 \brief returns true if fixed pattern noise has been corrected in this image,
	 false otherwise.

	 \return bool
	*/
	bool getFPNCorrected();

	/*!
	 \brief marks that this image has been calibrated with a white reference.

	 \param en sets true or false.
	*/
	void setWhiteCalibrated(bool en);

	/*!
	 \brief returns true if this image has been calibrated with a white reference.

	 \return bool
	*/
	bool getWhiteCalibrated();

	/*!
	 \brief marks that the dark image has been subtracted from the different
	 channel images.

	 \param en sets true or false.
	*/
	void setDarkSubtracted(bool en);

	/*!
	 \brief returns true if the dark image has been subtracted from the different
	 channel images.

	 \return bool
	*/
	bool getDarkSubtracted();

	/*!
	 \brief marks that misalignment of moving objects has been compensated
	  over the different channel images.

	 \param en sets true or false.
	*/
	void setMotionCompensated(bool en);

	/*!
	 \brief returns true if misalignment of moving objects has been compensated
	  over the different channel images.

	 \return bool
	*/
	bool getMotionCompensated();

private:
	quint16 myWidth;				/*!< defines the width of the image. */
	quint16 myHeight;				/*!< defines the height of the image. */
	quint8 myDepth;					/*!< defines the number of bits per channel. */

	QMap<qint16, Mat> myChImages;	/*!< contains the waveband images. */

	bool isFPNCorrected;			/*!< flag: image is fixed pattern noise corrected. */
	bool isWhiteCalibrated;			/*!< flag: image is white calibrated. */
	bool isDarkSubtracted;			/*!< flag: image channels have dark subtracted. */
	bool isMotionCompensated;		/*!< flag: image channels aligned (motion compensated). */
};


#endif // MULTISPECTRALIMAGE_H
