#ifndef IMGACQUISITIONWORKER_H
#define IMGACQUISITIONWORKER_H

#include <QObject>
#include <QThread>
#include"vimbacammanager.h"

class ImgAcquisitionWorker : public QThread//public QObject
{
	Q_OBJECT
//	QThread workerThread;
public:
	explicit ImgAcquisitionWorker(QObject *parent = 0);

	/*!
	 * \brief Sets the status of the acquisiton - used to start and stop acquisition
	 * in a while loop
	 * \param acquiring: The acquisition status.
	 */
	void setStatus(bool acquiring);

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
	void startAcquisition();

	/*!
	 * \brief stops the image acquisition
	 */
	void stopAcquisition();

private:
	VimbaCamManager vimbaCamManager;
	bool acquiring;

};

#endif // IMGACQUISITIONWORKER_H
