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
     * (standard deviation)to make it more machine-learning-friendly.
     * \param image1Channel a monochromatic (1channel) image.
     * \return the standardized image.
     */
    static Mat Standardize(Mat image1Channel);

    /*!
     * \brief Prints a 1 channel matrix to the console using qDebug()
     */
    static void Print1ChMatrixToConsole(Mat mat);
};

#endif // HELPER_H
