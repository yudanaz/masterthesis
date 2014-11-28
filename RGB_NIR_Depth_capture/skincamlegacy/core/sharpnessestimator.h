#ifndef SHARPNESSESTIMATOR_H
#define SHARPNESSESTIMATOR_H

#include <QDebug>
#include "../core/processingtask.h"
#include "../core/skincamconfig.h"

class SharpnessEstimator
{
public:
	SharpnessEstimator();

	/*!
	 * \brief This method defines local sharpness pixel-wise using the openCV Sobel operators
	 * to find horizontal and vertical gradient strength.
	 * \param The openCV matrix image on which local sharpness is to be computed.
	 * \return A grayscale image encoding local sharpness pixel/block-wise.
	 */
	static Mat sobel(Mat img);

	/*!
	 * \brief This method defines local sharpness pixel-wise using the Laplacian operator suggested
	 * by [Krotkov, Eric Paul. Active computer vision by cooperative focus and stereo.
	 * Springer-Verlag New York, Inc., 1989.] to find horizontal and vertical gradient strength.
	 * \param The openCV matrix image on which local sharpness is to be computed.
	 * \return A grayscale image encoding local sharpness pixel/block-wise.
	 */
	static Mat laplaceKrotkov(Mat img);

	/*!
	 * \brief This method defines local sharpness pixel-wise using the openCV Laplacian operator
	 * to find horizontal and vertical gradient strength.
	 * \param The openCV matrix image on which local sharpness is to be computed.
	 * \return A grayscale image encoding local sharpness pixel/block-wise.
	 */
	static Mat laplaceOpenCV(Mat img);

	/*!
	 * \brief !NOT WORKING YET! This method defines local sharpness of the image block-wise by computing the slope
	 * of the magnitude spectrum, according to [Vu, Cuong T., and Damon M. Chandler. "S3: a spectral
	 * and spatial sharpness measure." Advances in Multimedia, 2009. MMEDIA'09.
	 * First International Conference on. IEEE, 2009.]
	 * \param The openCV matrix image on which local sharpness is to be computed.
	 * \return A grayscale image encoding local sharpness pixel/block-wise.
	 */
	static Mat spectral(Mat img);

	/*!
	 * \brief This method estimates the camera-to-point distance for each pixel in a given
	 * image using local sharpness information on diferent wave bands.
	 * \param img is the original multi spectral image
	 * \return is a MultispectralImage with sharpness images for corresponding bands
	 * and color-encoded distance image on band 0.
	 */
};

#endif // SHARPNESSESTIMATOR_H
