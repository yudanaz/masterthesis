#ifndef SKINCAMCONFIG_H
#define SKINCAMCONFIG_H

#include <QList>
#include <QDir>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QMap>

#include <opencv2/opencv.hpp>

#include "../core/skincamexception.h"
#include "../io/camerasystemconfig.h"

/*!
 \brief This struct holds all parameters required for the calibration of the
 camera system, such as fixed pattern noise correction and white calibration.
*/
struct calibrationSettings
{
	QString fpnRefFile;		/*!< stores the filename of the FPN correction file. */

	QMap<qint16, double> whiteCal; /*!< correction factors for white calibration */

	bool subtractDark;			/*!< flag: subtract dark image from illuminated images. */
	bool applyFPNCal;			/*!< flag: use dark reference calibration. */
	bool applyWhiteCal;			/*!< flag: use white reference calibration. */
};

/*!
 \brief This struct holds all parameters of the linear stage control module
 plus those for the automatic (batch) acquisition of images using the stage.
*/
struct stageSettings
{
	QString serialInt;		/*!< stores the serial interface used for the stage. */
	QString targetFolder;	/*!< stores the path to the target folder for images. */
	quint16 start;			/*!< holds the starting position. */
	quint16 end;			/*!< holds the end position. */
	quint16 stepwidth;		/*!< holds the step width. */
	quint16 numImg;			/*!< the number of images to be taken for each step. */
	quint16 delay;			/*!< the delay in ms between changing position and taking an image. */
	bool inverted;			/*!< denotes if the stage is used from it's back side. */
	bool scaled;			/*!< denotes if images are saved in 8 instead of 16 bit. */
	bool mi;				/*!< denotes if images are saved as multispectral images. */
	bool fc;				/*!< denotes if images are saved as false color images. */
	bool sbs;				/*!< denotes if images are saved as side by side images. */
};

/*!
 \brief This struct holds all parameters of a quotient filter, specifying the
 channels used as well as the lower and upper bounds / thresholds.
*/
struct quotientFilter
{
	qint16	channel1;		/*!< is the first channel (waveband). */
	qint16	channel2;		/*!< is the second channel (waveband). */
	double	lowerThreshold; /*!< is the lower threshold for the filter. */
	double	upperThreshold; /*!< is the upper threshold for the filter. */
	quint16	minValue1;		/*!< is the minimum pixel value in channel 1. */
	quint16	minValue2;		/*!< is the minimum pixel value in channel 2. */
};

/*!
 \brief This struct holds an openCV image (Mat) together with a description
 (QString) as a pair.
*/
struct imgDesc
{
	cv::Mat	img;	/*!< holds the image. */
	QString desc;	/*!< holds the images description as string. */
};

enum sharpnessMetricMethod
{
	metric_Sobel,
	metric_LaplaceKrotkov,
	metric_LaplaceOpenCV,
	metric_Spectral
};

/*!
 \brief This class holds all configuration parameters for the SkinCam software.
 It stores the GUI configuration and interface setup as well as processing
 parameters and options. It can be written to and loaded from a file.
*/
class SkinCamConfig
{
public:
	/*!
	 \brief default constructor, setting default parameters.
	*/
	SkinCamConfig();

	/*!
	 \brief this constructor reads the configuration from a given file.

	 \param filename is the full path to the file to be opened.
	*/
	SkinCamConfig(QString filename);

	/*!
	 \brief This method writes the current configuration to a file specified
	 as parameter.

	 \param filename is the full path to the target file.
	*/
	void writeToFile(QString filename);

	//*** parameters for image source configuration:
	CameraSystemConfig cameraConfig;		/*!< contains the camera system configuration. */

	struct calibrationSettings calibration; /*!< contains image source calibration settings. */

	//*** parameters for GUI:
	QDir lastDir;							/*!< remembers the last used directory for saving. */
	QDir videoTarget;						/*!< remembers the destination for video recordings. */

	//*** parameters for external devices:
	struct stageSettings stage;				/*!< contains the linear stage settings. */

	//*** parameters for image processing:
	bool	contrastAutomatic;				/*!< flag: enable automatic contrast. */
	quint8	contrastValue;					/*!< value for manually set contrast. */
	quint16	contrastOffset;					/*!< dark offset for contrast. */

	bool	edgeDetection;					/*!< flag: enable edge detection on all channel images. */
	quint8	edgeThreshold;					/*!< threshold for edge detection. */

	bool	detectSkinByQuotient;			/*!< flag: enable skin detection by quotient filters. */
	QList<quotientFilter> quotientFilters;	/*!< list of quotient filters for skin detection. */

	bool	detectSkinBySVM;

	bool	showMaskContours;				/*!< flag: enable contour highlighting. */
	bool	morphResult;					/*!< flag: enable contour morphing. */
	bool	suppressMotion;					/*!< flag: suppress possible motion artifacts. */
	quint16	motionThreshold;

	bool estimateDistance;					/*!< flag: enable distance estimation by chrom. aberration*/
	sharpnessMetricMethod sharpMetric;		/*!< sharpness metric used for distance estimation. */
	int sharpnessNbrhdSize;                 /*!< the pixel neighborhood on each band used in sharpness value correlation */
	int distanceLowEnd;                     /*!< the lower end of the measured distance interval in mm */
	int distanceHighEnd;                    /*!< the higher end of the measured distance interval in mm*/

	//*** parameters for image display:
	QList<qint16> falseColorChannels;		/*!< list of channels for false color view. */
	QList<qint16> sideBySideChannels;		/*!< list of channels for side by side view. */

private:
	/*!
	 \brief sets default values for all parameters - used by default constructor
	 as well as starting point for loading a config file.
	*/
	void setDefaults();
};

#endif // SKINCAMCONFIG_H
