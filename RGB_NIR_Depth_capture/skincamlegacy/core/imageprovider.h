#ifndef IMAGEPROVIDER_H
#define IMAGEPROVIDER_H

#include <QObject>
#include <QReadWriteLock>
#include <QQueue>
#include <QMutex>

#include "../io/imagesource.h"
#include "../core/multispectralimage.h"

/*!
 \brief A "worker" class derived from QObject. It uses a given ImageSource
 to retrieve a new frame and writes it into a given MultispectralImage.
*/
class ImageProvider : public QObject
{
	Q_OBJECT

public:
	/*!
	 \brief standard constructor, optionally allowing to set the ImageSource.

	 \param newSource a pointer to an ImageSource.
	*/
	ImageProvider(ImageSource *newSource = NULL);

	/*!
	 \brief sets the ImageSource.

	 \param newSource a pointer to an ImageSource.
	*/
	void setImageSource(ImageSource *newSource);

	/*!
	 \brief starts or stops the image acquisition
	*/
	void setRun(bool status);

public slots:
	/*!
	 \brief this method is doing the actual work.
	*/
	void process();

signals:
	/*!
	 \brief emitted when a new frame has been acquired from the image source.

	 \param frame is the new frame.
	*/
	void newFrameReady(MultispectralImage frame);

	/*!
	 \brief emitted when an error occurs with the image source.

	 \param e is the thrown exception.
	*/
	void acquisitionError(ImageSourceException e);

private:
	ImageSource* myImageSource;			/*!< points to the used ImageSource. */
	ImageSourceException myLastError;	/*!< holds the last occured error. */

	QReadWriteLock lockImageSource;		/*!< thread-safe lock for changes of the image source. */
	QReadWriteLock lockStatus;			/*!< thread-safe lock for changes of the run-status. */

	bool myStatus;						/*!< while status is true, the image provider will continue acquiring new images. */
};

#endif // IMAGEPROVIDER_H
