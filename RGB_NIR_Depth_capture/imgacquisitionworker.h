#ifndef IMGACQUISITIONWORKER_H
#define IMGACQUISITIONWORKER_H

#include <QObject>
#include"vimbacammanager.h"

class ImgAcquisitionWorker : public QObject
{
	Q_OBJECT
//	QThread workerThread;
public:
	explicit ImgAcquisitionWorker(QObject *parent = 0);

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
