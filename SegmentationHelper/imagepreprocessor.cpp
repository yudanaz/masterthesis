#include "imagepreprocessor.h"
#include <QProgressDialog>
#include <QProcess>
#include <QDir>
#include <QFile>
#include <QTextStream>

ImagePreprocessor::ImagePreprocessor()
{
    //set image compression
    pngParams.push_back(CV_IMWRITE_PNG_COMPRESSION);
    pngParams.push_back(5);
    jpgParams.push_back(CV_IMWRITE_JPEG_QUALITY);
    jpgParams.push_back(95);
}

Mat ImagePreprocessor::NormalizeLocally(Mat img, int meanKernel, int stdDevKernel, bool outputAs8bit)
{
    Mat floatImg, mean, stdDev, out;

    //convert to float image
    img.convertTo(floatImg, CV_32F);//, 0.003921569); // 1/255 = 0.003921569

    //estimate image mean with gaussian blur
    GaussianBlur(floatImg, mean, Size(meanKernel, meanKernel), 0);
    floatImg = floatImg - mean;

    //estimate standard deviation with gaussian blur by doing sqrt( gauss_blur(img²) )
    GaussianBlur(floatImg.mul(floatImg), mean, Size(stdDevKernel, stdDevKernel), 0); //re-use mean matrix
    cv::pow(mean, 0.5, stdDev);
    floatImg = floatImg / stdDev;

    if(outputAs8bit)
    {
        //cast back to [0, 255] interval, so it can be saved as a JPG image (or other lossy compression)
        cv::normalize(floatImg, floatImg, 0, 1, NORM_MINMAX, -1);
        floatImg.convertTo(out, CV_8U, 255);
        return out;
    }
    else
    {
        return floatImg;
    }
}


void ImagePreprocessor::makeImagePatches(QList<Mat> rgbdnir, Mat labelImg, int localNeighborhood,
                                         int patchSize, int scales,
                                         QString outName, QString outFolder,
                                         int imgIndex, int imgTotal)
{
    int origWidth = rgbdnir[0].cols;
    int origHeight = rgbdnir[0].rows;

    //set image compression
//    vector<int> pngParams;
//    pngParams.push_back(CV_IMWRITE_PNG_COMPRESSION);
//    pngParams.push_back(8);
    vector<int> jpgParams;
    jpgParams.push_back(CV_IMWRITE_JPEG_QUALITY);
    jpgParams.push_back(94);

    //make temporary folder to store images before compressing into tar
    QDir dir;
    dir.mkdir(outFolder + "/" + outName);

    //Split all input images into their channels and join all channels in one vector
    vector<Mat> channels;
    int border = patchSize/2;
    foreach(Mat img, rgbdnir)
    {
        //apply local normalization for neigborhodd twice the size of convolution kernel
        //each channel of the input image is normalized separately, see NormalizeLocally()
        Mat imgNorm = NormalizeLocally(img, localNeighborhood, localNeighborhood*9);

        //separate channels in multichannel images
        quint8 ch = imgNorm.channels();
        if(ch > 1)
        {
            vector<Mat> img_channels(ch);
            cv::split(imgNorm, img_channels);
            channels.insert(channels.end(), img_channels.begin(), img_channels.end());
        }
        else
        {
            channels.push_back(imgNorm);
        }
    }
    quint8 nrOfChannels = channels.size();

    //make image pyramid, i.e. downsample all images / channels AND label image as many times as defined by parameter
    //ALSO make padding (borders) according to convolution kernel size
    //!NOTE! that the patch size must always be even, due to the nature of the ConvNet because
    //of subsampling (reducing size/2 between layer) for ex kernelsize 7 => borders = 6: 46 conv-> 40 subs-> 20 conv-> 14 subs-> 7 conv-> 1
    vector< vector<Mat> > channelPyr;
    vector<Mat> labelPyr;
    scales = scales < 1 ? 1 : scales; //correct invalid scales number

    for (int s = 0; s < scales; ++s) //jump first scale
    {

        //if scale = 1, only make padding in channel images, and just push back label image
        if(s == 0)
        {
            vector<Mat> imgPadded;

            for(int ch = 0; ch < nrOfChannels; ++ch)
            {
                //make padding (border)
                Mat chPadded;
                copyMakeBorder(channels[ch], chPadded, border, border-1, border, border-1, BORDER_CONSTANT, Scalar(0)); //black border

                imgPadded.push_back(chPadded);
            }

            channelPyr.push_back(imgPadded);
            labelPyr.push_back(labelImg);
        }
        //else downscale, then make padding
        else
        {
            vector<Mat> imgDown;
            for(int ch = 0; ch < nrOfChannels; ++ch)
            {
                //scale down, once for every scale step (thats what the for-loop is for)
                Mat chDown;
//                pyrDown(channels[ch], chDown);
//                for(int ss = 1; ss < s; ++ss){ pyrDown(chDown, chDown); }
                cv::resize(channels[ch], chDown, Size(), 0.5, 0.5, INTER_AREA);
                for(int ss = 1; ss < s; ++ss){ cv::resize(chDown, chDown, Size(), 0.5, 0.5, INTER_AREA); }

                //make padding (border)
                Mat chPadded;
                copyMakeBorder(chDown, chPadded, border, border-1, border, border-1, BORDER_CONSTANT, Scalar(0)); //black border

                imgDown.push_back(chPadded);
            }

            channelPyr.push_back(imgDown);

            //downsize label image WITHOUT SMOOTHING so no interpolated wrong labels are created, once for every scale step
            Mat labelDown = downSampleWithoutSmoothing(labelImg);
            for(int ss = 1; ss < s; ++ss){ labelDown = downSampleWithoutSmoothing(labelDown); }

            labelPyr.push_back(labelDown);
        }
    }

    //make progress dialog
    int cnt = 0;
    int maxCnt = 0;
    for (int s = 0; s < scales; ++s)
    {
        maxCnt += (channelPyr[s][0].cols - patchSize-1) * (channelPyr[s][0].rows - patchSize-1) * nrOfChannels;
    }
    QProgressDialog progress("Making patches for image " + QString::number(imgIndex) + " / " + QString::number(imgTotal), "cancel", 0, maxCnt);
    progress.setValue(0);
    progress.setMinimumWidth(450);
    progress.setMinimumDuration(100);
    progress.setWindowModality(Qt::WindowModal);

    //make patches and label text file for all scale, all channels
    for (int s = 0; s < scales; ++s)
    {
    //for every pixel (except border/padding pixels)
    int w = channelPyr[s][0].cols;
    int h = channelPyr[s][0].rows;

    //make output image, careful to round correctly, e.g. an 15x15 img downsampled is 8x8, not 7x7
    float divider = s == 0 ? 1.0 : s*2.0;
    int outHeight = patchSize * (int)(origHeight/divider+0.5);
    int outWidth = patchSize * (int)(origWidth/divider+0.5) * nrOfChannels;
    Mat scaleImg(outHeight, outWidth, CV_8UC1);

    //iterate over all pixel
    for (int y = border; y < h-border+1; ++y)
    {
    for (int x = border; x < w-border+1; ++x)
    {
        //make one big image for all channels, containing all pixel neighborhoods, and channel patches in the same row
        for (int ch = 0; ch < nrOfChannels; ++ch)
        {
            //make patch ROI and copy to big channel output image
            //!NOTE! that because of the even patch size, pixel is not exactly in the middle of patch!
            cv::Rect srcROI(x-border, y-border, patchSize, patchSize);
            cv::Rect destROI((x-border) * patchSize * nrOfChannels  + ch * patchSize, (y-border) * patchSize, patchSize, patchSize);
            channelPyr[s][ch](srcROI).copyTo(scaleImg(destROI));

            //report progress
            progress.setValue(cnt++);
            if(progress.wasCanceled()){ return; }
        }
    }
    }

    QString txtnm = outFolder + "/" + outName + "/" + outName
            + "_s" + QString::number(s);
//        imwrite((nm + ".png").toStdString(), channelImg, pngParams);
    imwrite((txtnm + ".jpg").toStdString(), scaleImg, jpgParams);

    //also write label img for every scale to output folder
    QString labelnm = outFolder + "/" + outName + "/" + outName + "_s" + QString::number(s) + "_labels.png";
    imwrite(labelnm.toStdString(), labelPyr[s]);
    }

    progress.setValue(maxCnt);
//    txtOut.flush();
//    outFile.close();

//    //make patches and label text file for all scale, all channels
//    for (int s = 0; s < scales; ++s)
//    {
//    //for every pixel (except border/padding pixels)
//    int w = channelPyr[s][0].cols;
//    int h = channelPyr[s][0].rows;

//    for (int y = border; y < h-border+1; ++y)
//    {
//    for (int x = border; x < w-border+1; ++x)
//    {
//        //save in temporary folder: each channel of the image as separate file, in order to use compression and minimize file size
//        //!NOTE! that we use PNG with highest compression, because it gives us the smallest file size for such small images (here 46x46)
//        QString relativeUrlNm = outName + "/" + outName + "_s" + QString::number(s)
//                + "_p" + QString::number(x-border) + "x" + QString::number(y-border);
//        QString nm = outFolder + "/" + relativeUrlNm;

//        Mat concat(patchSize, patchSize * nrOfChannels, CV_8UC1);

//        //concatenate all channels in one image in order to make saving faster and use less disk space
//        for (int ch = 0; ch < nrOfChannels; ++ch)
//        {
//            //make patch ROI and copy to output list
//            //!NOTE! that because of the even patch size, pixel is not exactly in the middle of patch!
//            Mat patch;
//            cv::Rect roi(x-border, y-border, patchSize, patchSize);
//            channelPyr[s][ch](roi).copyTo(patch);

////              imwrite((nm + "_ch" + QString::number(ch) + ".png").toStdString(), patch_channels.at(ch), pngParams);
//            patch.copyTo(concat(Rect(ch * patchSize, 0, patchSize, patchSize)));
//        }
//        imwrite((nm + ".png").toStdString(), concat, pngParams);

//        //get label and write name and label to file (only the  relative URL to a patch name is printed,
//        //the separate channels ("_ch1 .. _chN") have to be merged later when read in a Mat object)
//        uchar label = labelPyr[s].at<uchar>(y-border, x-border);
//        txtOut << relativeUrlNm << " " << label << "\n";

//        //report progress
//        progress.setValue(++cnt);
//        if(progress.wasCanceled()){ return; }
//    }
//    }
//    }
//    progress.setValue(maxCnt);
//    txtOut.flush();
//    outFile.close();

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



Mat ImagePreprocessor::downSampleWithoutSmoothing(Mat grayImg)
{
    int h = grayImg.rows;
    int w = grayImg.cols;
    Mat img2(h/2, w/2, CV_8UC1);

    for (int y = 1; y < h; y += 2)
    {
        for (int x = 1; x < w; x += 2)
        {
            img2.at<uchar>(y/2, x/2) = grayImg.at<uchar>(y, x);
        }
    }
    return img2;
}



void ImagePreprocessor::cutImageIn4Pieces(int patchSize, QString fileName)
{
    Mat orig = imread(fileName.toStdString(), IMREAD_UNCHANGED);

    QDir dir = QFileInfo(fileName).absoluteDir();
    QString outDir = dir.absolutePath() + "/pieces";
    dir.mkdir(outDir);
    QStringList list = fileName.split("/");
    fileName = list.last();

    QString typeSuffix;
    if(fileName.contains(".jpg"))
    {
        fileName.remove(".jpg"); typeSuffix = ".jpg";
    }
    else if(fileName.contains(".png"))
    {
        fileName.remove(".png"); typeSuffix = ".png";
    }

    QString channelSuffix;
    if(fileName.contains("_labels"))
    {
        fileName.remove("_labels"); channelSuffix = "_labels";
    }
    else if(fileName.contains("_nir"))
    {
        fileName.remove("_nir"); channelSuffix = "_nir";
    }
    else if(fileName.contains("_depth"))
    {
        fileName.remove("_depth"); channelSuffix = "_depth";
    }


    std::vector<Mat> pieces;
    int border = patchSize / 2;
    int middleX = orig.cols / 2;
    int middleY = orig.rows / 2;
    int pieceWidth = middleX + border;
    int pieceHeight = middleY + border;

    Rect roi1(0, 0, pieceWidth, pieceHeight);
    Rect roi2(middleX - border, 0, pieceWidth, pieceHeight);
    Rect roi3(0, middleY - border, pieceWidth, pieceHeight);
    Rect roi4(middleX - border, middleY - border, pieceWidth, pieceHeight);
    Mat out1, out2, out3, out4;

    orig(roi1).copyTo(out1);
    orig(roi2).copyTo(out2);
    orig(roi3).copyTo(out3);
    orig(roi4).copyTo(out4);

    pieces.push_back(out1);
    pieces.push_back(out2);
    pieces.push_back(out3);
    pieces.push_back(out4);

    //    int w = orig.cols / divideBy;
    //    int h = orig.rows / divideBy;
//    for (int y = 0; y < divideBy; ++y)
//    {
//        for (int x = 0; x < divideBy; ++x)
//        {
//            //if int division leaves over some pixels, make las vfgt images in row / column bigger by that
//            int h_, w_;
//            if(y == divideBy-1)
//            {
//                h_ = h + orig.rows - divideBy * h;
//            }
//            else { h_ = h; }
//            if(x == divideBy-1)
//            {
//                w_ = w + orig.cols - divideBy * w;
//            }
//            else { w_ = w; }

//            //make padding so that pieces overlap each other, but respecting image borders
//            //padding should ensure that pixels would be learned as border pixels but also
//            //inside their original neighborhood after cutting image into pieces
//            int border = patchSize/2;
//            int left = x * w;
//            int up = y * h;
//            int sizeX = w_;
//            int sizeY = h_;
//            int leftPad = (left - border < 0) ? 0 : border;
//            int upPad = (up - border < 0) ? 0 : border;
//            int rightPad = (left + sizeX + border > orig.cols) ? 0 : border;
//            int downPad = (up + sizeY + border > orig.rows) ? 0 : border;

//            Mat out;//(outSizeY, outSizeX, orig.type());
//            cv::Rect roi(left-leftPad, up-upPad, sizeX + leftPad + rightPad, sizeY + upPad + downPad) ;
//            orig(roi).copyTo(out);
//            pieces.push_back(out);
////            imshow("show", out);
////            cvWaitKey(100);
//        }
//    }

    for(int i = 0; i < pieces.size(); ++i)
    {
        QString nm = outDir + "/" + fileName + "_" + QString::number(i) + channelSuffix + typeSuffix;
        if(typeSuffix == ".jpg"){ imwrite(nm.toStdString(), pieces[i], jpgParams); }
        else if(typeSuffix == ".png"){ imwrite(nm.toStdString(), pieces[i], pngParams); }
    }
}




