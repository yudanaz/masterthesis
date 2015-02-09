#ifndef IMGACQUISITIONWORKER_H
#define IMGACQUISITIONWORKER_H

#include <QObject>
#include <QThread>
#include <QReadWriteLock>
#include"vimbacammanager.h"
#include"kinectcammanager.h"

class ImgAcquisitionWorker : public QObject
{
	Q_OBJECT
public:
	ImgAcquisitionWorker();
	~ImgAcquisitionWorker();

	/*!
	 * \brief Sets the status of the acquisiton - used to start and stop acquisition
	 * in a while loop
	 * \param acquiring: The acquisition status.
	 */
	void setAcquiring(bool acquiring);
	bool isAcquiring();
	void stop();
	bool isStopped();

signals:
	/*!
	 * \brief Tells the main thread that images for this frame have been
	 * captured and delivers them.
	 * \param imgs: A List of all captured images with their type.
	 */
	void imagesReady(RGBDNIR_MAP imgs);

public slots:
	/*!
	 * \brief initiates the image acquisition
	 */
	virtual void startAcquisition() = 0;

protected:
	bool acquiring;
	bool stopped;

	QReadWriteLock lock;

};

#endif // IMGACQUISITIONWORKER_H
