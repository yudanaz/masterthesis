#ifndef IMAGEPREPROCESSOR_H
#define IMAGEPREPROCESSOR_H

#include <QDebug>
#include "opencv2/opencv.hpp"

using namespace cv;

/*!
 * \brief This class implements methods used for preprocessing images
 * for the use in machine learning algorithms, more specifically in
 * a multiscale ConvNet, as in
 * [Farabet et al. 2013, "Learning hierarchical features for scene labeling",
 * IEEE Transactions on Pattern Analysis and Machine Intelligence]
 */
class ImagePreprocessor
{
public:
    ImagePreprocessor();

    /*!
     * \brief Pre-processes an image to zero mean and unit variance
     * (standard deviation) for local neighborhoods in order to make
     * it more machine-learning-friendly.
     * inspired by http://bigwww.epfl.ch/demo/jlocalnormalization/
     * Local normalization is done using ESTIMATIONS for mean and
     * standard deviation achieved with a gaussian smoothing filter.
     * \param img is the original image. NOTE THAT IF IT'S A MULTICHANNEL IMAGE,
     * NORMALIZATION IS DONE FOR EACH CHANNEL SEPARATELY, THIS HAS BEEN TESTED
     * AND PROVED IN GIMP, SEE TEST IN MAINWINDOW!
     * \param neighborhoodSize is the local neighborhood for which
     * the normalization is performed. MUST BE ODD!
     * \return the standardized image as a float matrix.
     */
    Mat NormalizeLocally(Mat img, int neighborhoodSize, bool outputAs8bit = true);

    void makeImagePatches(Mat img, Mat labelImg, QStringList labels, int localNeighborhood, int patchSize, QString outName, QString outFolder);
};

#endif // IMAGEPREPROCESSOR_H
