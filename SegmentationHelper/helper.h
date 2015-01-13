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
     * \brief Prints a 1 channel matrix to the console using qDebug()
     */
    static void Print1ChMatrixToConsole(Mat mat);
};

#endif // HELPER_H
