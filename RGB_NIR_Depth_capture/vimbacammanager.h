#ifndef VIMBACAMMANAGER_H
#define VIMBACAMMANAGER_H

#include<QString>
#include<QStringList>
#include<QList>
#include<QDebug>
#include<QMessageBox>
#include<QMutexLocker>
#include<QMutex>

#include<VimbaCPP/Include/VimbaCPP.h>
#include<opencv2/opencv.hpp>
#include"cammanager.h"
#include"skincamlegacy/io/goldeyeVimba.h"
#include"skincamlegacy/io/camerasystemconfig.h"
#include"skincamlegacy/io/flashlightcontrol.h"
#include"skincamlegacy/io/imagesource.h"
#include"prosilicavimba.h"
#include"libfreenect.hpp"

using namespace AVT::VmbAPI;
using namespace cv;


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

enum VimbaCamType { Vimba_Goldeye, Vimba_Prosilica };

class VimbaCamManager : public CamManager
{
public:
	VimbaCamManager(VimbaCamType camType);
	~VimbaCamManager();

	/*!
	 * \brief connects to goldeye and prosilica cameras, if present
	 */
	void connectCameras();

	/*!
	 * \brief closes cameras that have been detected and opened before.
	 */
	void closeCameras();


	/*!
	 * \brief Captures the images of all connected cameras and returns them in a list.
	 * \return list of all images plus image type (RGB, NIR or Depth).
	 */
	void getImages(QMap<RGBDNIR_captureType, Mat> &camImgs);

	/*!
	 * \brief Returns a descriptive string of the RGBDNIR channel type.
	 * \param i the capture type (RGB, NIR channel or Depth)
	 * \return a descriptive string
	 */
	static QString getRGBDNIR_captureTypeString(RGBDNIR_captureType i);

	bool goldeyeIsConnected();
	bool prosilicaIsConnected();

	void toggleMultichannelCapture(bool captureMultiChannel);


private:

	VimbaCamType myCamType;

	static QMutex mutex;

	int exceptionCnt;

	void startVimbaAPI();
	void startFlashlight();
	void stopFlashlight();

	VimbaSystem &vimbaSystem;
	bool APIrunning;

	FlashlightControl *flashlight;
	QMutex flashlightLock;
	bool flashLightRunning;

	CameraInterface *goldeye;
	CameraInterface *prosilica;

	int nrOfWavebands;
	int nrOfWavebands2Trigger;

	bool connected_prosilica;
	bool connected_goldeye;
	bool connected_flashlight;

	MyImageSource myImageSource;

	quint16 maxFPS;
};

#endif // VIMBACAMMANAGER_H
