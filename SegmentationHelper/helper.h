#ifndef HELPER_H
#define HELPER_H

#include <QDebug>
#include "opencv2/opencv.hpp"

using namespace cv;

/*!
 * \brief The Helper class implements useful functionalities as static methods.
 */
class Helper
{
public:
    Helper();

    /*!
     * \brief Pre-processes an image to zero mean and unit variance
     * (standard deviation) for local neighborhoods in order to make
     * it more machine-learning-friendly.
     * inspired by http://bigwww.epfl.ch/demo/jlocalnormalization/
     * Local normalization is done using ESTIMATIONS for mean and
     * standard deviation achieved with a gaussian smoothing filter.
     * \param image1Channel a monochromatic (1channel) image.
     * \param neighborhoodSize is the local neighborhood for which
     * the normalization is performed. MUST BE ODD!
     * \return the standardized image as a float matrix.
     */
    static Mat NormalizeLocally(Mat image1Channel, int neighborhoodSize);

    /*!
     * \brief Prints a 1 channel matrix to the console using qDebug()
     */
    static void Print1ChMatrixToConsole(Mat mat);
};

#endif // HELPER_H
