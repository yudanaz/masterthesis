#include "imagepreprocessor.h"
#include <QProgressDialog>
#include <QProcess>
#include <QDir>
#include <QFile>
#include <QTextStream>

ImagePreprocessor::ImagePreprocessor()
{

}

Mat ImagePreprocessor::NormalizeLocally(Mat img, int neighborhoodSize, bool outputAs8bit)
{
    Mat floatImg, mean, stdDev, out;

    //convert to float image
    img.convertTo(floatImg, CV_32F);//, 0.003921569); // 1/255 = 0.003921569

    //estimate image mean with gaussian blur
    GaussianBlur(floatImg, mean, Size(neighborhoodSize, neighborhoodSize), 0);
    floatImg = floatImg - mean;

    //estimate standard deviation with gaussian blur by doing sqrt( gauss_blur(img²) )
    GaussianBlur(floatImg.mul(floatImg), mean, Size(neighborhoodSize, neighborhoodSize), 0); //re-use mean matrix
    cv::pow(mean, 0.5, stdDev);
    floatImg = floatImg / stdDev;

    if(outputAs8bit)
    {
        //cast back to [0, 255] interval, so it can be saved as a JPG image (or other lossy compression)
        cv::normalize(floatImg, floatImg, 0, 1, NORM_MINMAX);
        floatImg.convertTo(out, CV_8U, 255);
        return out;
    }
    else
    {
        return floatImg;
    }
}


void ImagePreprocessor::makeImagePatches(QList<Mat> rgbdnir, Mat labelImg, int localNeighborhood,
                                         int patchSize, QString outName, QString outFolder)
{
    //set image compression
    vector<int> pngParams;
    pngParams.push_back(CV_IMWRITE_PNG_COMPRESSION);
    pngParams.push_back(8);

    //make progress dialog
    int maxCnt = rgbdnir.first().cols * rgbdnir.first().rows;
    int cnt = 0;
    QProgressDialog progress("Making patches...", "cancel", 0, maxCnt);
    progress.setValue(0);
    progress.setMinimumWidth(450);
    progress.setMinimumDuration(100);
    progress.setWindowModality(Qt::WindowModal);

    //make temporary folder to store images before compressing into tar
    QDir dir;
    dir.mkdir(outFolder + "/" + outName);

    //prepare text stream for labels
    QFile outFile(outFolder + "/" + outName + "_labels.txt");
    outFile.open(QFile::WriteOnly);
    QTextStream txtOut(&outFile);

    //for all imgs in list
    vector<Mat> channels;
    int border = patchSize/2;
    foreach(Mat img, rgbdnir)
    {
        //apply local normalization for neigborhodd twice the size of convolution kernel
        //each channel of the input image is normalized separately, see NormalizeLocally()
        Mat imgNorm = NormalizeLocally(img, localNeighborhood);

        //make padding (borders) according to convolution kernel size
        //!NOTE! that the patch size must always be even, due to the nature of the ConvNet because
        //of subsampling (reducing size/2 between layer) for ex kernelsize 7 => borders = 6: 46 conv-> 40 subs-> 20 conv-> 14 subs-> 7 conv-> 1
        Mat imgPadded;
        copyMakeBorder(imgNorm, imgPadded, border, border-1, border, border-1, BORDER_CONSTANT, Scalar(0)); //black border

        //separate channels in multichannel images
        quint8 ch = imgPadded.channels();
        if(ch > 1)
        {
            vector<Mat> img_channels(ch);
            cv::split(imgPadded, img_channels);
            channels.insert(channels.end(), img_channels.begin(), img_channels.end());
        }
        else
        {
            channels.push_back(imgPadded);
        }
    }
    quint8 nrOfChannels = channels.size();

    //for every pixel (except border/padding pixels)
    int w = channels[0].cols;
    int h = channels[0].rows;

    for (int y = border; y < h-border+1; ++y)
    {
        for (int x = border; x < w-border+1; ++x)
        {
            //save in temporary folder: each channel of the image as separate file, in order to use compression and minimize file size
            //!NOTE! that we use PNG with highest compression, because it gives us the smallest file size for such small images (here 46x46)
            QString relativeUrlNm = outName + "/" + outName + "_p" + QString::number(x-border) + "x" + QString::number(y-border);
            QString nm = outFolder + "/" + relativeUrlNm;

            Mat concat(patchSize, patchSize * nrOfChannels, CV_8UC1);

            //concatenate all channels in one image in order to make saving faster and use less disk space
            for (int ch = 0; ch < nrOfChannels; ++ch)
            {
                //make patch ROI and copy to output list
                //!NOTE! that because of the even patch size, pixel is not exactly in the middle of patch!
                Mat patch;
                cv::Rect roi(x-border, y-border, patchSize, patchSize);
                channels[ch](roi).copyTo(patch);

//              imwrite((nm + "_ch" + QString::number(ch) + ".png").toStdString(), patch_channels.at(ch), pngParams);
                patch.copyTo(concat(Rect(ch * patchSize, 0, patchSize, patchSize)));
            }
            imwrite((nm + ".png").toStdString(), concat, pngParams);

            //get label and write name and label to file (only the  relative URL to a patch name is printed,
            //the separate channels ("_ch1 .. _chN") have to be merged later when read in a Mat object)
            uchar label = labelImg.at<uchar>(y-border, x-border);
            txtOut << relativeUrlNm << " " << label << "\n";

            //report progress
            progress.setValue(++cnt);
            if(progress.wasCanceled()){ return; }
        }
    }

    txtOut.flush();
    outFile.close();
    progress.setValue(maxCnt);

//    //pack as TAR or ZIP file in order to minimize space. a 46x46 PNG file will have ~0.6K but will need 4K on disk because of block size
//    QProcess tarProc;
//    tarProc.setWorkingDirectory(outFolder);
//    tarProc.start("tar", QStringList() << "-cvf" << outName + ".tar" << outName ); // "tar -cvf <tarName> <dirName>"

//    maxCnt = 500;
//    cnt = 0;
//    QProgressDialog progress2("Compressing to TAR file...", "cancel", 0, maxCnt);
//    progress2.setValue(0);
//    progress2.setMinimumWidth(450);
//    progress2.setMinimumDuration(100);
//    progress2.setWindowModality(Qt::WindowModal);
//    while(!tarProc.waitForFinished(250))
//    {
//        cnt = cnt < maxCnt ? cnt : 0;
//        progress2.setValue(cnt);
//        cnt += 25;
//        if(progress2.wasCanceled()){ return; }
//    }
//    progress2.setValue(maxCnt);

//    //delete temporary folder
//    QProcess delDirProc;
//    delDirProc.setWorkingDirectory(outFolder);
//    delDirProc.start("rm -r " + outName);
//    delDirProc.waitForFinished();
}
