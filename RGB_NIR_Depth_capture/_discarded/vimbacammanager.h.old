#ifndef VIMBACAMMANAGER_H
#define VIMBACAMMANAGER_H

#include<QString>
#include<QStringList>
#include<QList>
#include<QDebug>
#include<VimbaCPP/Include/VimbaCPP.h>
#include<opencv2/opencv.hpp>

using namespace AVT::VmbAPI;
using namespace cv;

class VimbaCamManager
{
public:
	VimbaCamManager();
	~VimbaCamManager();

	/**
	 * @brief Returns a list of names of detected VIMBA cameras and saves cams in a
	 * private pointer list and opens them.
	 * @param repeatDetection should be set to true in order to repeat the detection process.
	 * @return a list of names of detected VIMBA cameras
	 */
	QStringList detectCameras(bool repeatDetection = false);

	/**
	 * @brief closes cameras that have been detected and open before.
	 */
	void closeCameras();

	QList<Mat> getCamImages();

private:
	void startVimbaAPI();

	VimbaSystem &vimbaSystem;
	bool APIrunning;

	CameraPtrVector cameras;
	QStringList cameraNames;
	QList< QPair<qint16,qint16> > cameraSizes;
	bool camsDetected;
};

#endif // VIMBACAMMANAGER_H
