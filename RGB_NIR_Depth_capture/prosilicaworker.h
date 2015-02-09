#ifndef PROSILICAWORKER_H
#define PROSILICAWORKER_H
#include"imgacquisitionworker.h"

class ProsilicaWorker : public ImgAcquisitionWorker
{
	Q_OBJECT
public:
	ProsilicaWorker();
	~ProsilicaWorker();

public slots:
	void startAcquisition();

private:
	VimbaCamManager vimbaCamManager;
};

#endif // PROSILICAWORKER_H
