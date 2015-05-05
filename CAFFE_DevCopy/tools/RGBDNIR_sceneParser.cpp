#include <cstring>
#include <cstdlib>
#include <vector>

#include <fstream>
#include <string>
#include <iostream>
#include <stdio.h>
#include "../../include/caffe/caffe.hpp"
#include "../../include/caffe/util/io.hpp"
#include "../../include/caffe/blob.hpp"
#include "../../include/caffe/rgbdnir_preproc.hpp"
#include "opencv2/opencv.hpp"

#define PATCHSIZE 46

using namespace caffe;
using namespace std;
using namespace cv;

void fillMemoryDataLayers(Net<float> *net, bool hasNIR, bool hasRGB, bool hasDepth, bool hasSkin,
//						  std::vector<Mat> nirPatchPyr,
//						  std::vector<Mat> depthPatchPyr,
//						  std::vector<Mat> rgbPatchPyr,
//						  std::vector<Mat> skinPatchPyr
						  std::vector<Mat> mats_rgb0_Y,
						  std::vector<Mat> mats_rgb0_UV,
						  std::vector<Mat> mats_rgb1_Y,
						  std::vector<Mat> mats_rgb1_UV,
						  std::vector<Mat> mats_rgb2_Y,
						  std::vector<Mat> mats_rgb2_UV,

						  std::vector<Mat> mats_nir0_Y,
						  std::vector<Mat> mats_nir0_UV,
						  std::vector<Mat> mats_nir1_Y,
						  std::vector<Mat> mats_nir1_UV,
						  std::vector<Mat> mats_nir2_Y,
						  std::vector<Mat> mats_nir2_UV,

						  std::vector<Mat> mats_depth0,
						  std::vector<Mat> mats_depth1,
                          std::vector<Mat> mats_depth2,

                          std::vector<Mat> mats_skin0,
                          std::vector<Mat> mats_skin1,
                          std::vector<Mat> mats_skin2
						  )
{
	//make dummy label
	std::vector<int> labels(1,0);

    //NIR//////////////////////////////////////////////////////////////
    if(hasNIR)
    {
        ((MemoryDataLayer<float>*)net->layer_by_name("nir0_Y").get())->AddMatVector(mats_nir0_Y, labels);
        ((MemoryDataLayer<float>*)net->layer_by_name("nir0_UV").get())->AddMatVector(mats_nir0_UV, labels);
        ((MemoryDataLayer<float>*)net->layer_by_name("nir1_Y").get())->AddMatVector(mats_nir1_Y, labels);
        ((MemoryDataLayer<float>*)net->layer_by_name("nir1_UV").get())->AddMatVector(mats_nir1_UV, labels);
        ((MemoryDataLayer<float>*)net->layer_by_name("nir2_Y").get())->AddMatVector(mats_nir2_Y, labels);
        ((MemoryDataLayer<float>*)net->layer_by_name("nir2_UV").get())->AddMatVector(mats_nir2_UV, labels);
    }

    //DEPTH////////////////////////////////////////////////////////////
    if(hasDepth)
    {
        ((MemoryDataLayer<float>*)net->layer_by_name("depth0").get())->AddMatVector(mats_depth0, labels);
        ((MemoryDataLayer<float>*)net->layer_by_name("depth1").get())->AddMatVector(mats_depth1, labels);
        ((MemoryDataLayer<float>*)net->layer_by_name("depth2").get())->AddMatVector(mats_depth2, labels);
    }

    //RGB//////////////////////////////////////////////////////////////
    if(hasRGB)
    {
        ((MemoryDataLayer<float>*)net->layer_by_name("rgb0_Y").get())->AddMatVector(mats_rgb0_Y, labels);
        ((MemoryDataLayer<float>*)net->layer_by_name("rgb0_UV").get())->AddMatVector(mats_rgb0_UV, labels);
        ((MemoryDataLayer<float>*)net->layer_by_name("rgb1_Y").get())->AddMatVector(mats_rgb1_Y, labels);
        ((MemoryDataLayer<float>*)net->layer_by_name("rgb1_UV").get())->AddMatVector(mats_rgb1_UV, labels);
        ((MemoryDataLayer<float>*)net->layer_by_name("rgb2_Y").get())->AddMatVector(mats_rgb2_Y, labels);
        ((MemoryDataLayer<float>*)net->layer_by_name("rgb2_UV").get())->AddMatVector(mats_rgb2_UV, labels);
    }

    //DEPTH////////////////////////////////////////////////////////////
    if(hasSkin)
    {
        ((MemoryDataLayer<float>*)net->layer_by_name("skin0").get())->AddMatVector(mats_skin0, labels);
        ((MemoryDataLayer<float>*)net->layer_by_name("skin1").get())->AddMatVector(mats_skin1, labels);
        ((MemoryDataLayer<float>*)net->layer_by_name("skin2").get())->AddMatVector(mats_skin2, labels);
    }
}


Mat getImgPatch(Mat img, int x, int y)
{
	//cut out patch twice the size
	cv::Mat patch;
	cv::Rect roi(x, y, PATCHSIZE, PATCHSIZE);
	img(roi).copyTo(patch);
	return patch;
}

void separate_Y_UV_channels(Mat &yuv, Mat &yOut, Mat &uvOut)
{
	std::vector<Mat> yuvVect;
	split(yuv, yuvVect);
	std::vector<Mat> uv_vec;
	uv_vec.push_back(yuvVect[1]);
	uv_vec.push_back(yuvVect[2]);

	yOut = yuvVect[0];
	merge(uv_vec, uvOut);
}

void parseImage(Net<float>& caffe_test_net, string imgName, string labelSuffix, const char* modeName, const char* outputNameSuffix)
{
    RGBDNIR_preproc preproc;
    int borderSz = PATCHSIZE / 2;

    //get model modality
    bool hasDepth = false, hasRGB = false, hasSkin = false;
    if(strcmp(modeName, "NIR") == 0){  }
    else if(strcmp(modeName, "DNIR") == 0){ hasDepth = true; }
    else if(strcmp(modeName, "RGBNIR") == 0){ hasRGB = true; }
    else if(strcmp(modeName, "RGBDNIR") == 0){ hasDepth = true; hasRGB = true; }
    else if(strcmp(modeName, "RGBDNIRS") == 0){ hasDepth = true; hasRGB = true; hasSkin = true; }

    // load image files
    Mat nirImg, rgbImg, depthImg, skinImg, labelImg;
    labelImg = cv::imread(imgName + labelSuffix + ".png", IMREAD_GRAYSCALE);
    nirImg = cv::imread(imgName + "_nir.png", IMREAD_COLOR);
    if(hasDepth){ depthImg = cv::imread(imgName + "_depth.png", IMREAD_GRAYSCALE); }
    if(hasRGB){ rgbImg = cv::imread(imgName + "_rgb.png", IMREAD_COLOR); }
    if(hasSkin){ skinImg = cv::imread(imgName + "_skin.png", IMREAD_GRAYSCALE); }
    int h = labelImg.rows;
    int w = labelImg.cols;

    LOG(INFO) << "Success: Loaded image: " << imgName;

    //imshow("nir rgb", nirImg); cvWaitKey();

    //convert to YUV space if color image
    cv::cvtColor(nirImg, nirImg, CV_BGR2YCrCb);
    if(hasRGB){ cv::cvtColor(rgbImg, rgbImg, CV_BGR2YCrCb); }

    //imshow("nir yuv", nirImg); cvWaitKey();

    //make image pyramid
    std::vector<Mat> nirPyr, depthPyr, rgbPyr, skinPyr;
    nirPyr = preproc.makePyramid(nirImg, 3);
    if(hasDepth){ depthPyr = preproc.makePyramid(depthImg, 3, INTER_NEAREST); }
    if(hasRGB){ rgbPyr = preproc.makePyramid(rgbImg, 3); }
    if(hasSkin){ skinPyr = preproc.makePyramid(skinImg, 3, INTER_NEAREST); }

    //imshow("nir pyr 1", nirPyr[0]); cvWaitKey();

    //normalize locally
    for(int i = 0; i < 3; ++i)
    {
        Mat pyrLevelNIR = nirPyr[i];
        preproc.normalizeEachChannelLocally(pyrLevelNIR, 15);
        if(hasRGB){ preproc.normalizeEachChannelLocally(rgbPyr[i], 15); }
    }

    //imshow("nir pyr 1 normalized", nirPyr[0]); cvWaitKey();

    //make border
    std::vector<Mat> nirPyr_(3);
    std::vector<Mat> rgbPyr_(3);
    std::vector<Mat> depthPyr_(3);
    std::vector<Mat> skinPyr_(3);
    cv::copyMakeBorder(nirPyr.at(0), nirPyr_.at(0), borderSz, borderSz-1, borderSz, borderSz-1, cv::BORDER_CONSTANT, cv::Scalar(0));
    cv::copyMakeBorder(nirPyr.at(1), nirPyr_.at(1), borderSz, borderSz-1, borderSz, borderSz-1, cv::BORDER_CONSTANT, cv::Scalar(0));
    cv::copyMakeBorder(nirPyr.at(2), nirPyr_.at(2), borderSz, borderSz-1, borderSz, borderSz-1, cv::BORDER_CONSTANT, cv::Scalar(0));

    if(hasRGB)
    {
        cv::copyMakeBorder(rgbPyr.at(0), rgbPyr_.at(0), borderSz, borderSz-1, borderSz, borderSz-1, cv::BORDER_CONSTANT, cv::Scalar(0));
        cv::copyMakeBorder(rgbPyr.at(1), rgbPyr_.at(1), borderSz, borderSz-1, borderSz, borderSz-1, cv::BORDER_CONSTANT, cv::Scalar(0));
        cv::copyMakeBorder(rgbPyr.at(2), rgbPyr_.at(2), borderSz, borderSz-1, borderSz, borderSz-1, cv::BORDER_CONSTANT, cv::Scalar(0));
    }
    if(hasDepth)
    {
        cv::copyMakeBorder(depthPyr.at(0), depthPyr_.at(0), borderSz, borderSz-1, borderSz, borderSz-1, cv::BORDER_CONSTANT, cv::Scalar(0));
        cv::copyMakeBorder(depthPyr.at(1), depthPyr_.at(1), borderSz, borderSz-1, borderSz, borderSz-1, cv::BORDER_CONSTANT, cv::Scalar(0));
        cv::copyMakeBorder(depthPyr.at(2), depthPyr_.at(2), borderSz, borderSz-1, borderSz, borderSz-1, cv::BORDER_CONSTANT, cv::Scalar(0));
    }
    if(hasSkin)
    {
        cv::copyMakeBorder(skinPyr.at(0), skinPyr_.at(0), borderSz, borderSz-1, borderSz, borderSz-1, cv::BORDER_CONSTANT, cv::Scalar(0));
        cv::copyMakeBorder(skinPyr.at(1), skinPyr_.at(1), borderSz, borderSz-1, borderSz, borderSz-1, cv::BORDER_CONSTANT, cv::Scalar(0));
        cv::copyMakeBorder(skinPyr.at(2), skinPyr_.at(2), borderSz, borderSz-1, borderSz, borderSz-1, cv::BORDER_CONSTANT, cv::Scalar(0));
    }

    cout << "Success: " << imgName <<  " preprocessed. Parsing scene.";
    cout.flush();

    //separate Y from U,V channels inr NIR and RGB image
    std::vector<Mat> nirPyr_Y, nirPyr_UV, rgbPyr_Y, rgbPyr_UV;

    Mat niry, niruv;
    separate_Y_UV_channels(nirPyr_.at(0), niry, niruv);
    nirPyr_Y.push_back(niry.clone());
    nirPyr_UV.push_back(niruv.clone());
    separate_Y_UV_channels(nirPyr_.at(1), niry, niruv);
    nirPyr_Y.push_back(niry.clone());
    nirPyr_UV.push_back(niruv.clone());
    separate_Y_UV_channels(nirPyr_.at(2), niry, niruv);
    nirPyr_Y.push_back(niry.clone());
    nirPyr_UV.push_back(niruv.clone());

    if(hasRGB)
    {
        Mat rgby, rgbuv;
        separate_Y_UV_channels(rgbPyr_.at(0), rgby, rgbuv);
        rgbPyr_Y.push_back(rgby.clone());
        rgbPyr_UV.push_back(rgbuv.clone());
        separate_Y_UV_channels(rgbPyr_.at(1), rgby, rgbuv);
        rgbPyr_Y.push_back(rgby.clone());
        rgbPyr_UV.push_back(rgbuv.clone());
        separate_Y_UV_channels(rgbPyr_.at(2), rgby, rgbuv);
        rgbPyr_Y.push_back(rgby.clone());
        rgbPyr_UV.push_back(rgbuv.clone());
    }

    //make scene parsing of image with CNN
    Mat labelImg_predicted(h, w, CV_8UC1, Scalar(0));
    std::vector<Mat> nirPatchPyr_Y(3);
    std::vector<Mat> nirPatchPyr_UV(3);
    std::vector<Mat> rgbPatchPyr_Y(3);
    std::vector<Mat> rgbPatchPyr_UV(3);
    std::vector<Mat> depthPatchPyr(3);
    std::vector<Mat> skinPatchPyr(3);

//    for (int y = 0; y < 1; y += 1) //for debug
    for (int y = 0; y < h; y += 1)
    {
//        for (int x = 0; x < 10; x += 1) //for debug
        for (int x = 0; x < w; x += 1)
        {
            //get patches (for Y and UV channels) for all scales
            //scale 0
            nirPatchPyr_Y.at(0) = getImgPatch(nirPyr_Y.at(0), x, y);
            nirPatchPyr_UV.at(0) = getImgPatch(nirPyr_UV.at(0), x, y);

            if(hasRGB)
            {
                rgbPatchPyr_Y.at(0) = getImgPatch(rgbPyr_Y.at(0), x, y);
                rgbPatchPyr_UV.at(0) = getImgPatch(rgbPyr_UV.at(0), x, y);
            }
            if(hasDepth){ depthPatchPyr.at(0) = getImgPatch(depthPyr_.at(0), x, y); }
            if(hasSkin){ skinPatchPyr.at(0) = getImgPatch(skinPyr_.at(0), x, y); }

            //scale 1
//            if(x % 2 == 0 && y % 2 == 0)
            {
                nirPatchPyr_Y.at(1) = getImgPatch(nirPyr_Y.at(1), x/2, y/2);
                nirPatchPyr_UV.at(1) = getImgPatch(nirPyr_UV.at(1), x/2, y/2);
                if(hasRGB)
                {
                    rgbPatchPyr_Y.at(1) = getImgPatch(rgbPyr_Y.at(1), x/2, y/2);
                    rgbPatchPyr_UV.at(1) = getImgPatch(rgbPyr_UV.at(1), x/2, y/2);
                }
                if(hasDepth){ depthPatchPyr.at(1) = getImgPatch(depthPyr_.at(1), x/2, y/2); }
                if(hasSkin){ skinPatchPyr.at(1) = getImgPatch(skinPyr_.at(1), x/2, y/2); }
            }

            //scale 2
//            if(x % 4 == 0 && y % 4 == 0)
            {
                nirPatchPyr_Y.at(2) = getImgPatch(nirPyr_Y.at(2), x/4, y/4);
                nirPatchPyr_UV.at(2) = getImgPatch(nirPyr_UV.at(2), x/4, y/4);
                if(hasRGB)
                {
                    rgbPatchPyr_Y.at(2) = getImgPatch(rgbPyr_Y.at(2), x/4, y/4);
                    rgbPatchPyr_UV.at(2) = getImgPatch(rgbPyr_UV.at(2), x/4, y/4);
                }
                if(hasDepth){ depthPatchPyr.at(2) = getImgPatch(depthPyr_.at(2), x/4, y/4); }
                if(hasSkin){ skinPatchPyr.at(2) = getImgPatch(skinPyr_.at(2), x/4, y/4); }
            }

//            LOG(INFO) << nirPatchPyr_UV.at(0).channels();
//            imshow("nir y 0", nirPatchPyr_Y.at(0));cvWaitKey();
//            imshow("nir y 1", nirPatchPyr_Y.at(1));cvWaitKey();
//            imshow("nir y 2", nirPatchPyr_Y.at(2));cvWaitKey();
            std::vector<Mat> mats_nir0_Y; mats_nir0_Y.push_back(nirPatchPyr_Y.at(0));
            std::vector<Mat> mats_nir0_UV; mats_nir0_UV.push_back(nirPatchPyr_UV.at(0));
            std::vector<Mat> mats_nir1_Y; mats_nir1_Y.push_back(nirPatchPyr_Y.at(1));
            std::vector<Mat> mats_nir1_UV; mats_nir1_UV.push_back(nirPatchPyr_UV.at(1));
            std::vector<Mat> mats_nir2_Y; mats_nir2_Y.push_back(nirPatchPyr_Y.at(2));
            std::vector<Mat> mats_nir2_UV; mats_nir2_UV.push_back(nirPatchPyr_UV.at(2));


            std::vector<Mat> mats_rgb0_Y, mats_rgb0_UV, mats_rgb1_Y, mats_rgb1_UV, mats_rgb2_Y, mats_rgb2_UV;
            if(hasRGB)
            {
                mats_rgb0_Y.push_back(rgbPatchPyr_Y.at(0));
                mats_rgb0_UV.push_back(rgbPatchPyr_UV.at(0));
                mats_rgb1_Y.push_back(rgbPatchPyr_Y.at(1));
                mats_rgb1_UV.push_back(rgbPatchPyr_UV.at(1));
                mats_rgb2_Y.push_back(rgbPatchPyr_Y.at(2));
                mats_rgb2_UV.push_back(rgbPatchPyr_UV.at(2));
            }

            std::vector<Mat> mats_depth0, mats_depth1, mats_depth2;
            if(hasDepth)
            {
                mats_depth0.push_back(depthPatchPyr.at(0));
                mats_depth1.push_back(depthPatchPyr.at(1));
                mats_depth2.push_back(depthPatchPyr.at(2));
            }

            std::vector<Mat> mats_skin0, mats_skin1, mats_skin2;
            if(hasSkin)
            {
                mats_skin0.push_back(skinPatchPyr.at(0));
                mats_skin1.push_back(skinPatchPyr.at(1));
                mats_skin2.push_back(skinPatchPyr.at(2));
            }

            //feed network
            fillMemoryDataLayers(&caffe_test_net, true, hasRGB, hasDepth, hasSkin,
                                 mats_rgb0_Y, mats_rgb0_UV, mats_rgb1_Y, mats_rgb1_UV, mats_rgb2_Y, mats_rgb2_UV,
                                 mats_nir0_Y, mats_nir0_UV, mats_nir1_Y, mats_nir1_UV, mats_nir2_Y, mats_nir2_UV,
                                 mats_depth0, mats_depth1, mats_depth2,
                                 mats_skin0, mats_skin1, mats_skin2);
//            LOG(INFO) << "filled";

            //get prediction
            float loss = 0.0;
            /*const vector<Blob<float>*>& results = */caffe_test_net.ForwardPrefilled(&loss);
//            LOG(INFO) << "Result size: "<< results.size();
            // Log how many blobs were loaded
//            LOG(INFO) << "Blob size: "<< caffe_test_net.input_blobs().size();
//            LOG(INFO)<< "-------------";
//            LOG(INFO)<< " prediction :  ";

            // Get probabilities
//			const boost::shared_ptr<Blob<float> >& probLayer = caffe_test_net.blob_by_name("prob");
//			const float* probs_out = probLayer->cpu_data();

            // Get argmax results
            const boost::shared_ptr<Blob<float> >& argmaxLayer = caffe_test_net.blob_by_name("argmax");

            // Display results
//			LOG(INFO) << "---------------------------------------------------------------";
            const float* argmaxs = argmaxLayer->cpu_data();
            float predictedClass = 0.0;
            for (int i = 0; i < argmaxLayer->num(); i++)
            {
                predictedClass = argmaxs[i*argmaxLayer->height() + 0];
//                LOG(INFO) << "Pattern:"<< i << " class:" << argmaxs[i*argmaxLayer->height() + 0] << " Prob=" << probs_out[i*probLayer->height() + 0];
//				LOG(INFO) << "Pixel: ("<< x << "," << y << ") class:" << predictedClass
//						  << " Prob=" << probs_out[i*probLayer->height() + 0];
            }
//            LOG(INFO)<< "-------------";

            labelImg_predicted.at<uchar>(y,x) = (uchar)(predictedClass);
        }//endof for(x...)
        cout << ".";
        cout.flush();
    }//endof for(y...)
    cout << "\n";
    cout.flush();

//	imshow("NIR image", nirImg);
//	imshow("Ground truth labels", labelImg*85);
//	imshow("Predicted Labels", labelImg_predicted*85);
//    cvWaitKey();

    std::string outNm = imgName + "_predicted_" + string(modeName) + string(outputNameSuffix) + ".png";
    std::string outNm2 = imgName + "_predicted_" + string(modeName) + string(outputNameSuffix) + "_eq.png";
    imwrite(outNm, labelImg_predicted);
    double min, max;
    cv::minMaxLoc(labelImg_predicted, &min, &max);
    imwrite(outNm2, labelImg_predicted * (255/max));
}


int main(int argc, char** argv)
{
    if (argc != 8)
	{
        LOG(ERROR) << "PLEASE ENTER 6 ARGUMENTS: (1)net_proto (2)pretrained_net_proto (3)txt-file with test images"
                   << "(4)label suffix (5)[NIR/DNIR/RGBNIR/RGBDNIR/RGBDNIRS] (6)[GPU/CPU] (7)extra naming suffix";
		return 1;
	}

	//Setting CPU or GPU
    if (argc >= 6 && strcmp(argv[6], "GPU") == 0)
	{
        LOG(INFO) << "Using GPU";
        Caffe::set_mode(Caffe::GPU);
	}
	else
	{
		LOG(INFO) << "Using CPU";
		Caffe::set_mode(Caffe::CPU);
	}

	//get the net
	Net<float> caffe_test_net(argv[1], caffe::TEST);
	LOG(INFO) << "Success: Created net: " << argv[1];
	//get trained net
	caffe_test_net.CopyTrainedLayersFrom(argv[2]);
	LOG(INFO) << "Success: Loaded trained layers from " << argv[2];

    //get label image file suffix
    string labelSuffix = argv[4];

    //get modality
    const char* modeName = argv[5];
    LOG(INFO) << "Model type: " << modeName;

    const char* outputNameSuffix = argv[7];

    //read file names
    ifstream file(argv[3]);
    string imgName;
    while(getline(file, imgName))
    {
        parseImage(caffe_test_net, imgName, labelSuffix, modeName, outputNameSuffix);
    }


}

