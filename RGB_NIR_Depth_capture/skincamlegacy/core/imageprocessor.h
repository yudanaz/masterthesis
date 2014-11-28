#ifndef IMAGEPROCESSING_H
#define IMAGEPROCESSING_H

#include <QThread>
#include <QReadWriteLock>
#include <QCoreApplication>
#include <QMap>
#include <QMapIterator>
#include <QList>

#include <iostream>

#include "../io/imagesource.h"
#include "../core/multispectralimage.h"
#include "../core/processingtask.h"
#include "../core/taskdetectskinbyquotient.h"
#include "../core/skincamexception.h"
#include "../core/skincamconfig.h"
#include "../core/taskestimatedistancebychromaberr.h"

using namespace cv;

/*!
 \brief This class provides methods for image processing. A configuration object
 has to be handed over which specifies the methods that will be applied as well
 as the parameters to be used.
*/
class ImageProcessor : public QObject
{
	Q_OBJECT

public:
	/*!
	 \brief standard constructor, optionally allowing to set ImageSource and
	 MultispectralImage (target).

	 \param aConfig is the image processors configuration.
	*/
	ImageProcessor(SkinCamConfig aConfig);

	/*!
	 \brief standard destructor.
	*/
	~ImageProcessor();

	/*!
	 \brief sets a new processing configuration.

	 \param newConfig is the new configuration for this processor.
	*/
	void setConfig(SkinCamConfig newConfig);

	/*!
	 * \brief gets the estimated distance at a pixel position.
	 * \param x is the x parameter of the pixel position.
	 * \param y is the y parameter of the pixel position.
	 * \return the estimated distance of the edge at pixel position.
	 */
	int getEstimatedDistanceAt(int x, int y);

public slots:
	/*!
	 \brief this method is doing the actual work. This includes downscaling the
	 image to 8bit per channel for display, if needed, as well as handling all
	 processing tasks such as skin detection.
	*/
	void process(MultispectralImage image);

	/*!
	 \brief this slot is called by a skin detection task (typically running
	 in another thread), handling over the result of the detection process.

	 \param result is an openCV binary matrix with pixel values of 0 (no skin)
			or 1 (skin), which can be used as a mask.
	*/
	void skinDetectionReady(Mat result);

	/*!
	 * \brief this slot is called by a distance estimation task (typically running
	 * in another thread), handling over the result.
	 * \param result is a MultispectralImage with sharpness images for corresponding bands
	 * and color-encoded distance image on band 0.
	 */
	void distEstimationReady(Mat result);

	/*!
	 \brief this slot is called by child-tasks (typically running in other
	 threads) if there has been an error during processing.
	*/
	void onError();

	/*!
	 * \brief this slot is called by the cannyThreshDialog object via mainwindow and sets several parameters for distance estimation.
	 */
	void setDistEstimParams(int lowCannyThresh, int highCannyThresh, int selectedImageForCanny, bool autoSelectCannyImage, int medianSmoothingKernel, int distLowEnd, int distHighEnd);

	/*!
	 * \brief this slot is called by the TaskEstimateDistanceByChromAberr object in order to report back useful information.
	 * \param sharpestBand is the band that was sharpest in this iteration.
	 */
	void reportFromDistEstimation(int sharpestBand);

signals:
	/*!
	 \brief This signal is emitted when processing is finished.

	 \param frame holds the final (processed) image frame.
	 \param frame8Bit holds the processed image, scaled to 8 bit (if necessary)
	 \param results holds a list of result images, e.g. classification masks
	*/
	void finishedProcessing(MultispectralImage frame,
							MultispectralImage frame8Bit,
							QList<imgDesc> results);

	/*!
	 \brief is emitted when an error occured during processing.

	 \param e is the exception which was thrown.
	*/
	void errorProcessing(ImageSourceException e);

	/*!
	 \brief this signal is emitted to skin detection tasks, handing over a new
	 frame for processing and starting the detection process.

	 \param frame is a new image frame for skin detection.
	*/
	void doSkinDetection(MultispectralImage frame);

	/*!
	 * \brief this signal is emitted to the distance estimation task
	 * \param frame is an openCV matrix image
	 */
	void doDistanceEstimation(MultispectralImage frame);

	/*!
	 * \brief Sets the sharpness metric method used in the distance estimation process.
	 * \param metric is the metric to be used.
	 */
	void setDistEstimParams(int metric, Mat edgeMask, int sharpnessNbrhdSize, int medianKernel);

private:
	/*!
	 \brief performs Canny Edge Detection on a given frame with a given threshold
	 and returns the result.

	 \param frame is the input image.
	 \param threshold is the edge threshold.
	 \return Mat is the edge image.
	*/
	Mat doEdgeDetection(Mat frame, quint8 threshold);

	/*!
	 * \brief creates an 8-bit color image representing the distance information contained
	 * in the 16-bit image from distance estimation. color code: blue = far, red = near
	 * \param edges16Bit is the image containing edges the estimated distance information on edge pixels. All other pixels should be zero.
	 * \param min_mm is the minimum distance that should be displayed. All values below will be mapped to this.
	 * \param max_mm is the maximum distance that should be displayed. All values above will be mapped to this.
	 * \return the color distance image in the blue-violet-red range.
	 */
	Mat make8BitColorDistanceImage(Mat edges16Bit, int min_mm, int max_mm);

	SkinCamConfig		myConfig;				/*!< holds the configuration. */
	QReadWriteLock		lockConfig;				/*!< lock for thread-safe access to config. */

	QList<QThread*>		subthreads;				/*!< a list of sub-threads for processing. */
	QList<TaskDetectSkinByQuotient*> skinTasks;	/*!< a list of sub-tasks for skin detetion. */
	QList<Mat>			skinDetectionResults;	/*!< a list holding the results of the skin detection tasks. */

	MultispectralImage lastFrame;				/*!< holds the previous frame for motion detection / compensation. */

	QList<QThread*> distThreads;				/*!< a list of threads for distance estimation. */
	TaskEstimateDistanceByChromAberr* distTask; /*!< ptr to task object for distance estimation*/
	QList<imgDesc> distEstimationResults;		/*!< a list of distance estimation results */
	int mouseClickNbhrd;                        /*!< the neighborhood around a mouse click pixel */
	int cannyLowThresh;							/*!< low threshold for canny edge detection*/
	int cannyHighThresh;						/*!< high threshold for canny edge detection*/
	bool autoSelectCannyImage;                  /*!< flag that defines if the image used for canny edge detection is determined automatically */
	int lastSharpestBand;                      /*!< holds index of the sharpest band image from the last frame */
	int cannyImage;								/*!< index of the image on which edges are to be computed*/
	int medianKernel;                           /*!< is the kernel size used for median smoothing the depth estimation*/
	int distLowEnd;                             /*!< lower end of the measured distance interval */
	int distHighEnd;                             /*!< higher end of the measured distance interval */

	bool errorOccurred;	/*!< marks if an error occured during the processing. */
};

#endif // IMAGEPROCESSING_H
