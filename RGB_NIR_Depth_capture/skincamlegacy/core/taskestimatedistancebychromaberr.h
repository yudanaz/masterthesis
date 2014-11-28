#ifndef TASKESTIMATEDISTANCEBYCHROMABERR_H
#define TASKESTIMATEDISTANCEBYCHROMABERR_H

#include <QDebug>

#include "../core/processingtask.h"
#include "../core/skincamconfig.h"
#include "../core/distanceestimator.h"

/*!
 * \brief This class represents the task of estimating the distance of visible points
 * from the camera by using differences in chromatic aberration in the waveband images
 * provided by the infrared camera.
 */

class TaskEstimateDistanceByChromAberr : public ProcessingTask
{
	Q_OBJECT

public:
	/*!
	 * \brief a simple constructor
	 */
	TaskEstimateDistanceByChromAberr();

public slots:

	/*!
	 \brief this slot starts processing on a given multispectral image.
	 \param msSource is the image to be processed.
	*/
	void process(MultispectralImage frame);

	/*!
	 * \brief placeholder method which doesn't do anything.
	 */
	void process(Mat frame);

	/*!
	 * \brief Sets the sharpness metric used ofr distance estimation.
	 * \param metric is the chosen metric cast to int for thread safety.
	 */
	void setParameters(int metric, Mat edgeMask, int sharpnessNbrhdSize, int medianSmoothingKernel);

signals:
	/*!
	 * \brief this signal is used to report information back to the parent thread.
	 * \param sharpestBand is the band that was sharpest in this iteration.
	 */
	void reportBack(int sharpestBand);

private:

	/*!
	 * \brief This method selects the correct method according to the selected
	 * sharpness metric.
	 * \param The image on which sharpness is to be computed.
	 * \return An openCV matrix image with grayscale encoded sharpness values.
	 */
	Mat computeLocalSharpness(Mat img);

	/*!
	 * \brief This method estimates the camera-to-point distance for each pixel in a given
	 * image using local sharpness information on diferent wave bands.
	 * \param img is the original multi spectral image
	 * \return is a 16 bit open cv Mat image, which encodes the distance of edges to camera.
	 */
	Mat estimateDistance(MultispectralImage img);

	/*!
	 * \brief Returns a median smoothed version of the input depth image. This method should be used to remove outliers from incorrect depth estimation.
	 * \param img is the image to be smoothed. This image contains depth information at pixels (instead of color info)
	 * \param kernel is the size of the neighborhood kernel used in median smoothing.
	 */
	void medianSmooth(Mat img, int kernel);

	/*!
	 * \brief Computes normalized differences between sharpness values of all wavebands.
	 * \param v1550 is the sharpness value of band 1550 nm.
	 * \param v1300 is the sharpness value of band 1300 nm.
	 * \param v1060 is the sharpness value of band 1060 nm.
	 * \param v935 is the sharpness value of band 935 nm.
	 * \return is a vector containing the normalized differences
	 */
	std::vector<double> computeNormalizedDifferences(double v1550, double v1300, double v1060, double v935);

	/*!
	 * \brief Returns a square rectangle representing a neighborhood around a pixel, while respecting the boundaries of the
	 * underlying region-of-interest (of an image).
	 * \param minMaxRect is rectangle defining the boundaries of the underlying region-of-interest.
	 * \param pixelIndex is the index of the pixel for which a neighborhood rectangle should be defined.
	 * \param neighborhoodSize is the numer of rows and columns of the neighborhood (rectangle is always square).
	 * \return a square neighborhood rectangle.
	 */
	Rect makeNeighborhood(Rect minMaxRect, int pixelIndex, int neighborhoodSize);

	/*!
	 * \brief Identifies the pixel with the highest value inside a pixe neighborhood in a given image.
	 * \param img is the original image.
	 * \param neighborhood is a region-of-interest inside the original image for which the highest value should be identified.
	 * \return the highest value inside the neighborhood / region-of-interest.
	 */
	int getHighestInNeighborhood(Mat img, Rect neighborhood);

	/*!
	 * \brief Computes the avg contrast of a given image ROI, according to the Michelson contrast in the range [0,1]
	 * \param roi: Image with a given region of interest.
	 * \return Michelson contrast value.
	 */
	double computeContrast(Mat roi);

	sharpnessMetricMethod metric;       /*!< Is the metric used to compute the local sharpness*/
	DistanceEstimator distEstimator;    /*!< Is the class object that implements the decision tree */
	Mat edgeMask;                       /*!< Is the mask containing the edge pixels on which sharpness values are correlated between bands*/
	int sharpnessNbrhd;                 /*!< Is the neighborhood size used in sharpnes correlation */
	QList<Mat> images;					/*!< Holds the original waveband images*/
	QList<Mat> sharpnessResults;		/*!< Holds results of sharpness estimation (before normalization)*/
	QList<quint16> waveBands;			/*!< Holds the waveband nanometer values of the multispectral image.*/
	int medianKernel;                   /*!< Is the kernel size used in median smoothing the estimation */
};

#endif // TASKESTIMATEDISTANCEBYCHROMABERR_H
