#ifndef VIMBACAMMANAGER_H
#define VIMBACAMMANAGER_H

#include<QString>
#include<QStringList>
#include<QList>
#include<QDebug>
#include<QMessageBox>
#include<VimbaCPP/Include/VimbaCPP.h>
#include<opencv2/opencv.hpp>
#include"skincamlegacy/io/goldeyeVimba.h"
#include"skincamlegacy/io/camerasystemconfig.h"
#include"skincamlegacy/io/flashlightcontrol.h"
#include"prosilicavimba.h"

using namespace AVT::VmbAPI;
using namespace cv;

enum RGBDNIR_captureType
{
	RGB, Kinect_Depth, Kinect_RGB, NIR_Dark, NIR_935, NIR_1060, NIR_1300, NIR_1550
};
typedef QMap<RGBDNIR_captureType, Mat> RGBDNIR_MAP;

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
	void detectCameras();

	/**
	 * @brief closes cameras that have been detected and open before.
	 */
	void closeCameras();

	RGBDNIR_MAP getCamImages();
	static QString getRGBDNIR_captureTypeString(RGBDNIR_captureType i);

private:
	void startVimbaAPI();

	VimbaSystem &vimbaSystem;
	bool APIrunning;
//	CameraSystemConfig camConfig;
	FlashlightControl *flashlight;
	QMutex flashlightLock;
	CameraInterface *goldeye;
	CameraInterface *prosilica;

//	CameraPtrVector cameras;
//	QStringList cameraNames;
//	QList< QPair<qint16,qint16> > cameraSizes;

	bool detected_prosilica;
	bool detected_goldeye;
};

#endif // VIMBACAMMANAGER_H
