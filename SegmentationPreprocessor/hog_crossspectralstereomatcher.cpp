#include "hog_crossspectralstereomatcher.h"
#include <QDebug>

HOG_crossSpectralStereoMatcher::HOG_crossSpectralStereoMatcher():
    resizing_output_images_(false)
{
}

void HOG_crossSpectralStereoMatcher::process(Mat imgRGB_L, Mat imgNIR_R, Mat& out_disp)
{
    //HOG works top down, so we'll transpose the image matrices
//    flip(imgRGB_L, imgRGB_L,0);
//    flip(imgNIR_R, imgNIR_R,0);
    transpose(imgRGB_L, imgRGB_L);
    transpose(imgNIR_R, imgNIR_R);

    int orig_w = imgRGB_L.cols;
    int orig_h = imgRGB_L.rows;

    //multispectral stereo matching with HOG descriptors like in
    //"On cross-spectral stereo matching using dense gradient features" [Pinggera]
    HOGDescriptor d1;
    int blockSize = 18;
    int cellSize = 6;
    d1.blockSize = Size(blockSize, blockSize); //param from paper
    d1.cellSize = Size(cellSize, cellSize);// n x n subcells, n = 3, nCells = 3*3 = 9, param from paper

    //define a dynamic window size, must be aligned with blocksize and should be close to original size
    int win_w = ((int)(orig_w / blockSize)) * blockSize;
    int win_h = ((int)(orig_h / blockSize)) * blockSize;
    d1.winSize = Size(win_w, win_h);

    d1.blockStride = Size(1, 1); // -> stride = 1 means every goddamn pixel, less blocksize-1 border, i.e.
                               // nrOfGradients = blocks * cellsPerBlock * Bins
                               // and blocks = (width - blockSize + 1) * (height - blockSize + 1) => 613 * 487 * 9 * 9 = 24181011 in this case
    d1.nbins = 9;
    vector<float> descriptorsValues_RGB;
    vector<Point> locations_RGB;
    vector<float> descriptorsValues_NIR;
    vector<Point> locations_NIR;

    //resize images to match HOG window size, inter are is best method for down-sampling an image, as it doesn't cause moire-effects (http://docs.opencv.org/modules/imgproc/doc/geometric_transformations.html#resize)
    Mat rgb_resized, nir_resized;
    resize(imgRGB_L, rgb_resized, d1.winSize, INTER_AREA);
    resize(imgNIR_R, nir_resized, d1.winSize, INTER_AREA);

    d1.compute(rgb_resized, descriptorsValues_RGB, Size(0,0), Size(0,0), locations_RGB);
    d1.compute(nir_resized, descriptorsValues_NIR, Size(0,0), Size(0,0), locations_NIR);

//    qDebug() << "image size: " << win_w << "x" << win_h;
//    qDebug() << "total nr of bins: " << descriptorsValues_RGB.size();
//    qDebug() << INT_MAX;
//    qDebug() << ULONG_MAX;

    Mat disp = makeDisparity_WTA(descriptorsValues_RGB, descriptorsValues_NIR, d1.winSize, blockSize,
                                 blockSize/cellSize * blockSize/cellSize, d1.nbins);

    //resize back to input image size; use NEAREST NEIGHBOR INTERPOLATION because in-between interpolation doesn't make sense for disparity
    if(!resizing_output_images_)
    {
        resize(disp, out_disp, Size(orig_w, orig_h), INTER_NEAREST);
    }
    else //if output images are downsampled anyways, don't upsample now in order to not decrease quality further
    {
        out_disp = disp;
    }

    //transpose the result back
    transpose(out_disp, out_disp);

//    Mat hogDescrImg = Helper::get_hogdescriptor_visual_image(rgb_resized, descriptorsValues_RGB, d.winSize, d.cellSize, 1, 1.0);
//    imshow("hog descriptors", hogDescrImg);
}

Mat HOG_crossSpectralStereoMatcher::makeDisparity_WTA(vector<float> &values_L, vector<float> &values_R,
                                                      Size imgSize, int blockSize, int nCells, int nBins)
{
    int w = imgSize.width;
    int h = imgSize.height;

    //get actual width of descriptor matrix - smaller than image, because of border caused by blocks
    int w_descr = (w - blockSize + 1);
    int h_descr = (h - blockSize + 1);

    Mat disp_border(h, w, CV_8UC1, Scalar(255));

    int range = HOG_crossSpectralStereoMatcher::dispRange;
    int binsInDescriptor = nCells * nBins; //every step is one block centered around one pixel in original images
    float smallestDistance = FLT_MAX; //here the smallest distance is stored when searching for best match for every pixel
    int bestMatch = 0;

//    qDebug() << "total nr of bins: " << w_descr * h_descr * binsInDescriptor;

    vector<float> descr_L(binsInDescriptor);
    vector<float> descr_R(binsInDescriptor);

    //go through all descriptors of left image and compare with <range> of descriptors in right image
    //(every descriptor stands for one image pixel)
    for(int y = range; y < h_descr; ++y)
    {
        for(int x = 0; x < w_descr; ++x) //respect horizontal border caused by disparity-range (we're looking for pixels to the left)
        {
            ulong i = y * w_descr + x;// the real index into descriptor vector

            //get left descriptor
            getDescriptor(values_L, descr_L, binsInDescriptor, i);

//            ///DEBUG
//            int cost = 99;
//            int cnt = 0;// h_descr * w_descr;
//            while(cost != 0)
//            {
//                getDescriptor(values_R, descr_R, binsInDescriptor, cnt++);
//                cost = getL1Distance(descr_L, descr_R);
//            }
//            qDebug() << "found descr. " << i << " for left at " << cnt << "in right";
//            ///DEBUG


            //step through a range of descriptors for right image and find the best match
            smallestDistance = FLT_MAX; //reset
            for(int j = 0; j <= range; j++)
            {
                //get right descriptor
                getDescriptor(values_R, descr_R, binsInDescriptor, i-j); //look to the left! stuff in right camera is more to the left!

                //get L1 (Manhattan) distance between descriptor vectors. is it the smallest yet?
                float dist = getL1Distance(descr_L, descr_R);
//                qDebug() << "distance: " << dist;
                if(dist < smallestDistance)
                {
                    bestMatch = j;
                    smallestDistance = dist;
                }
            }

//            qDebug() << i << " dist: " << smallestDistance << " best match: " << bestMatch;

            //write the disparity for the best match to disparity image
            uchar dispValue = (uchar)(bestMatch/(float)range * 255.0 + 0.5); //cast to 8bit range, 255 = closest, 0 = farest
            disp_border.at<uchar>(y + blockSize/2, x + blockSize/2) = dispValue;
        }//x
    }//y

    return disp_border;
}

void HOG_crossSpectralStereoMatcher::getDescriptor(vector<float>& allDescriptors, vector<float>& descriptor,
                                                   int binsInDescriptor, int currentDescriptor)
{
    //copy one block
    int startIndex = currentDescriptor * binsInDescriptor;
    for(int i = 0; i < binsInDescriptor; i++)
    {
       float v = allDescriptors[startIndex + i];
       descriptor[i] = v;
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



/******************************************************************************
 * SET HOG PARAMETERS IN STATIC FUNCTION
 *****************************************************************************/
void HOG_crossSpectralStereoMatcher::setParams(int disparityRange)
{
    dispRange = disparityRange;
}
int HOG_crossSpectralStereoMatcher::dispRange = 8;
