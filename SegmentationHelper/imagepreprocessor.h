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

    void cutImageIn4Pieces(int patchSize, QString fileName);

    /*!
     * \brief Pre-processes an image to zero mean and unit variance
     * (standard deviation) for local neighborhoods in order to make
     * it more machine-learning-friendly.
     * inspired by
     * http://bigwww.epfl.ch/demo/jlocalnormalization/
     * and
     * http://bigwww.epfl.ch/sage/soft/localnormalization/]
     *
     * Local normalization is done using ESTIMATIONS for mean and
     * standard deviation achieved with a gaussian smoothing filter.
     * \param img is the original image. NOTE THAT IF IT'S A MULTICHANNEL IMAGE,
     * NORMALIZATION IS DONE FOR EACH CHANNEL SEPARATELY, THIS HAS BEEN TESTED
     * AND PROVED IN GIMP, SEE TEST IN MAINWINDOW!
     * \param meanKernel: The Gaussian kernel size used for mean estimation.
     * \param stdDevKernel: The Gaussian kernel size used for standard deviation estimation.
     * \return the standardized image as a float matrix.
     */
    Mat NormalizeLocally(Mat img, int meanKernel, int stdDevKernel, bool outputAs8bit = true);

    /*!
     * \brief Creates patches for every pixel of the normalized original and rescaled images,
     * i.e. every pixel yields a new image containing a pixel neighborhood. Image channels are
     * separated and concatenated horizontally in the output image to reduce size and time.
     * \param img: A list of original images. A list is used because the original image may
     * be composed of more than 3 (RGB) channels, e.g. RGB + NIR + depth.
     * Also creates a text file containing every image patch and corresponding label.
     * \param labelImg: grayscale image containing labels at pixel positions.
     * \param localNeighborhood: The neighborhood used for local normalization.
     * \param patchSize: Size of the patch around each pixel.
     * \param scales: How many scales should the image pyramid have. (always downscale by 0.5)
     * \param outName: The prefix of the names of the output files (patches).
     * \param outFolder: The folder to save image patches and label text files.
     * \param imgIndex: Index of this image if > 1 image are beeing processed (for progress bar).
     * \param imgTotal: Total amount of images (for progress bar).
     */
    void makeImagePatches(QList<Mat> img, Mat labelImg, int localNeighborhood, int patchSize, int scales, QString outName, QString outFolder, int imgIndex, int imgTotal);

    Mat downSampleWithoutSmoothing(Mat grayImg);

private:
    vector<int> pngParams;
    vector<int> jpgParams;
};

#endif // IMAGEPREPROCESSOR_H
