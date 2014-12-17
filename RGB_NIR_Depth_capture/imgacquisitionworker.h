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

	void setStatus(bool acquiring);

signals:
	void imagesReady(RGBDNIR_MAP imgs);

public slots:
	void startAcquisition();
	void stopAcquisition();

private:
	VimbaCamManager vimbaCamManager;
	bool acquiring;

};

#endif // IMGACQUISITIONWORKER_H
