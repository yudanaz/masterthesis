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
#include"skincamlegacy/io/imagesource.h"
#include"prosilicavimba.h"

using namespace AVT::VmbAPI;
using namespace cv;

enum RGBDNIR_captureType
{
	RGB, Kinect_Depth, Kinect_RGB, NIR_Dark, NIR_935, NIR_1060, NIR_1300, NIR_1550
};
typedef QMap<RGBDNIR_captureType, Mat> RGBDNIR_MAP;


/*!
 * \brief The MyImageSource class is a very simple extension of H.Steiners ImageSource
 * class, and is used only to give access to the fixed-pattern-noise and
 * white reference calibration
 */
class MyImageSource : public ImageSource
{
public:
	virtual ~MyImageSource(){}
	virtual void openSource (QString source){}
	virtual MultispectralImage acquireNewFrame(){}
	void loadFPNRef(){ loadFPNReference(); }
	void doFPNCalib(Mat m){ applyFPNCalibration(m); }
	void doWhiteCalib(Mat m, int index);
};

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
	void connectCameras();

	/**
	 * @brief closes cameras that have been detected and open before.
	 */
	void closeCameras();


	void startFlashlight();
	void stopFlashlight();

	/*!
	 * \brief Captures the images of all connected cameras and returns them in a list.
	 * \return list of all images plus image type (RGB, NIR or Depth).
	 */
	RGBDNIR_MAP getCamImages();

	/*!
	 * \brief Returns a descriptive string of the RGBDNIR channel type.
	 * \param i the capture type (RGB, NIR channel or Depth)
	 * \return a descriptive string
	 */
	static QString getRGBDNIR_captureTypeString(RGBDNIR_captureType i);

private:
	void startVimbaAPI();

	VimbaSystem &vimbaSystem;
	bool APIrunning;

	FlashlightControl *flashlight;
	QMutex flashlightLock;
	bool flashLightRunning;

	CameraInterface *goldeye;
	CameraInterface *prosilica;

	bool connected_prosilica;
	bool connected_goldeye;
	bool connected_flashlight;

	MyImageSource myImageSource;

	quint16 maxFPS;
	long FramePause;
};

#endif // VIMBACAMMANAGER_H
