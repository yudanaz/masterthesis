#include "hog_crossspectralstereomatcher.h"
#include <QDebug>

HOG_crossSpectralStereoMatcher::HOG_crossSpectralStereoMatcher()
{
}

void HOG_crossSpectralStereoMatcher::process(Mat imgRGB_L, Mat imgNIR_R, Mat& out_disp)
{
    int orig_w = imgRGB_L.cols;
    int orig_h = imgRGB_L.rows;

    //multispectral stereo matching with HOG descriptors like in
    //"On cross-spectral stereo matching using dense gradient features" [Pinggera]
    HOGDescriptor d;
    int blockSize = 18;
    int cellSize = 6;
    d.blockSize = Size(blockSize, blockSize); //param from paper
    d.cellSize = Size(cellSize, cellSize);// n x n subcells, n = 3, nCells = 3*3 = 9, param from paper

    //define a dynamic window size, must be aligned with blocksize and should be close to original size
    int win_w = ((int)(orig_w / blockSize)) * blockSize;
    int win_h = ((int)(orig_h / blockSize)) * blockSize;
    d.winSize = Size(win_w, win_h);

    d.blockStride = Size(1,1); // -> every goddamn pixel, less blocksize-1 border, i.e.
                               // nrOfGradients = blocks * cellsPerBlock * Bins
                               // and blocks = (width - blockSize + 1) * (height - blockSize + 1) => 613 * 487 * 9 * 9 = 24181011 in this case
    d.nbins = 9;
    vector<float> descriptorsValues_RGB;
    vector<Point> locations_RGB;
    vector<float> descriptorsValues_NIR;
    vector<Point> locations_NIR;

    //resize images to match HOG window size
    Mat rgb_resized, nir_resized;
    resize(imgRGB_L, rgb_resized, d.winSize);
    resize(imgNIR_R, nir_resized, d.winSize);

    d.compute(rgb_resized, descriptorsValues_RGB, Size(0,0), Size(0,0), locations_RGB);
    d.compute(nir_resized, descriptorsValues_NIR, Size(0,0), Size(0,0), locations_NIR);

    qDebug() << "image size: " << win_w << "x" << win_h;
    qDebug() << "total nr of bins: " << descriptorsValues_RGB.size();

    Mat disp = makeDisparity_WTA(descriptorsValues_RGB, descriptorsValues_NIR, d.winSize, blockSize,
                                 blockSize/cellSize * blockSize/cellSize, d.nbins);
    resize(disp, out_disp, Size(orig_w, orig_h));

//    Mat hogDescrImg = Helper::get_hogdescriptor_visual_image(rgb_resized, descriptorsValues_RGB, d.winSize, d.cellSize, 1, 1.0);
//    imshow("hog descriptors", hogDescrImg);
}

Mat HOG_crossSpectralStereoMatcher::makeDisparity_WTA(vector<float> &values_L, vector<float> &values_R,
                                                      Size imgSize, int blockSize, int nCells, int nBins)
{
    int w = imgSize.width;
    int h = imgSize.height;
    Mat disp(h, w, CV_8UC1, Scalar(0));
    int nPixels = (h - blockSize + 1) * (w - blockSize + 1);

    int range = HOG_crossSpectralStereoMatcher::dispRange;
    int step = nCells * nBins; //every step is one block centered around one pixel in original images
    float smallestDistance = FLT_MAX; //here the smallest distance is stored when searching for best match for every pixel
    int bestMatch = 0;

    //go through all descriptors of left image and compare with <range> of descriptors in right image
    //(every descriptor stands for one image pixel)
    for(int i = 0; i < nPixels; ++i)
    {
//        qDebug() << "i: " <<  QString::number(i);

        vector<float> descr_L(step);
        vector<float> descr_R(step);

        //get left descriptor
        getDescriptor(values_L, descr_L, step, i);

        //step through a range of descriptors for right image and find the best match
        smallestDistance = FLT_MAX; //reset
        for(int j = 0; j < range; ++j)
        {
//            qDebug() << "j: " <<  QString::number(j);

            //get right descriptor
            getDescriptor(values_R, descr_R, step, i-j); //look to the left! stuff in right camera is more to the left!

            //get L1 (Manhattan) distance between descriptor vectors. is it the smallest yet?
            float dist = getL1Distance(descr_L, descr_R);
            if(dist < smallestDistance)
            {
                bestMatch = j;
                smallestDistance = dist;
            }
        }

        //get indices into disparity image while respecting borders imposed by block size
        int x = blockSize/2 + ( i % (w - blockSize + 1) );
        int y = blockSize/2 + ( i / (w - blockSize + 1) );

        //write the disparity for the best match to disparity image
        disp.at<uchar>(y,x) = (uchar)(bestMatch/(float)range * 255.0 + 0.5); //cast to 8bit range
    }

    return disp;
}

void HOG_crossSpectralStereoMatcher::getDescriptor(vector<float>& allDescriptors, vector<float>& descriptor,
                                                   int blockSize, int currentPixel)
{
    //copy one block
    int cnt = 0;
    for(int i = currentPixel * blockSize; i < currentPixel * blockSize + blockSize; ++i)
    {
       descriptor[cnt++] = allDescriptors[i];
    }
}

float HOG_crossSpectralStereoMatcher::getL1Distance(vector<float> a, vector<float> b)
{
    float dist = 0;
    for (int i = 0; i < a.size(); ++i)
    {
        dist += fabs(a[i] - b[i]);
    }
    return dist;
}


void HOG_crossSpectralStereoMatcher::setParams(int disparityRange)
{
    dispRange = disparityRange;
}

int HOG_crossSpectralStereoMatcher::dispRange = 8;
