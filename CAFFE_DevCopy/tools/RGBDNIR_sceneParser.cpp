#include <cstring>
#include <cstdlib>
#include <vector>

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
						  std::vector<Mat> mats_rgb_Y,
						  std::vector<Mat> mats_rgb_UV,
						  std::vector<Mat> mats_nir_Y,
						  std::vector<Mat> mats_nir_UV,
						  std::vector<Mat> mats_depth
						  )
{


	//make dummy label
	std::vector<int> labels(1,1);

	//NIR//////////////////////////////////////////////////////////////
	if(hasNIR)
	{
		((MemoryDataLayer<float>*)net->layer_by_name("nir_Y").get())->AddMatVector(mats_nir_Y, labels);
		((MemoryDataLayer<float>*)net->layer_by_name("nir_UV").get())->AddMatVector(mats_nir_UV, labels);
	}

	//DEPTH////////////////////////////////////////////////////////////
	if(hasDepth)
	{
		((MemoryDataLayer<float>*)net->layer_by_name("depth").get())->AddMatVector(mats_depth, labels);
	}

	//RGB//////////////////////////////////////////////////////////////
	if(hasRGB)
	{
		((MemoryDataLayer<float>*)net->layer_by_name("rgb_Y").get())->AddMatVector(mats_rgb_Y, labels);
		((MemoryDataLayer<float>*)net->layer_by_name("rgb_UV").get())->AddMatVector(mats_rgb_UV, labels);
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

int main(int argc, char** argv)
{
	RGBDNIR_preproc preproc;
	int borderSz = PATCHSIZE / 2;

	if (argc != 7)
	{
		LOG(ERROR) << "PLEASE ENTER 3 ARGUMENTS: (1)net_proto (2)pretrained_net_proto (3)image without suffix (4)[NIR/DNIR/RGBNIR/RGBDNIR/RGBDNIRSkin] (5)[GPU/CPU] (6 - Scale)[0/1/2]";
		return 1;
	}

	//Setting CPU or GPU
	if (strcmp(argv[5], "GPU") == 0)
	{
		Caffe::set_mode(Caffe::GPU);
		LOG(INFO) << "Using GPU";
	}
	else
	{
		Caffe::set_mode(Caffe::CPU);
		LOG(INFO) << "Using CPU";
	}

	//check which scale should be tested
	int scale = 0;
	if (strcmp(argv[6], "1") == 0){ scale = 1; }
	else if (strcmp(argv[6], "2") == 0){ scale = 2; }

	//get the net
	Net<float> caffe_test_net(argv[1], caffe::TEST);
	LOG(INFO) << "Success: Created net: " << argv[1];
	//get trained net
	caffe_test_net.CopyTrainedLayersFrom(argv[2]);
	LOG(INFO) << "Success: Loaded trained layers from " << argv[2];

	// load image files
	bool hasDepth = false, hasRGB = false, hasSkin = false;
	if(strcmp(argv[4], "NIR") == 0){  }
	else if(strcmp(argv[4], "DNIR") == 0){ hasDepth = true; }
	else if(strcmp(argv[4], "RGBNIR") == 0){ hasRGB = true; }
	else if(strcmp(argv[4], "RGBDNIR") == 0){ hasDepth = true; hasRGB = true; }
	else if(strcmp(argv[4], "RGBDNIRSkin") == 0){ hasDepth = true; hasRGB = true; hasSkin = true; }
	Mat nirImg, rgbImg, depthImg, skinImg, labelImg;
	string nm = string(argv[3]);
	labelImg = cv::imread(nm + "_labels.png", IMREAD_GRAYSCALE);
	nirImg = cv::imread(nm + "_nir.png", IMREAD_COLOR);
	if(hasDepth){ depthImg = cv::imread(nm + "_depth.png", IMREAD_GRAYSCALE); }
	if(hasRGB){ rgbImg = cv::imread(nm + "_rgb.png", IMREAD_COLOR); }
	if(hasSkin){ skinImg = cv::imread(nm + "_skin.png", IMREAD_GRAYSCALE); }
	int h = labelImg.rows;
	int w = labelImg.cols;

	LOG(INFO) << "Success: Loaded image: " << argv[3];

	//convert to YUV space if color image
	cv::cvtColor(nirImg, nirImg, CV_BGR2YCrCb);
	if(hasRGB){ cv::cvtColor(rgbImg, rgbImg, CV_BGR2YCrCb); }
	std::vector<Mat> nirPyr, depthPyr, rgbPyr, skinPyr;

	//make image pyramid
	nirPyr = preproc.makePyramid(nirImg, 3);
	if(hasDepth){ depthPyr = preproc.makePyramid(rgbImg, 3, INTER_NEAREST); }
	if(hasRGB){ rgbPyr = preproc.makePyramid(rgbImg, 3); }
	if(hasSkin){ skinPyr = preproc.makePyramid(skinImg, 3, INTER_NEAREST); }

	//normalize locally
	for(int i = 0; i < 3; ++i)
	{
		preproc.normalizeEachChannelLocally(nirPyr[i], 15);
		if(hasDepth){ preproc.normalizeEachChannelLocally(depthPyr[i], 15); }
		if(hasRGB){ preproc.normalizeEachChannelLocally(rgbPyr[i], 15); }
		if(hasSkin){ preproc.normalizeEachChannelLocally(skinPyr[i], 15); }
	}

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

	LOG(INFO) << "Success: Image(s) preprocessed.";

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
	Mat labelImg_predicted(h, w, CV_8UC1);
	std::vector<Mat> nirPatchPyr_Y(3);
	std::vector<Mat> nirPatchPyr_UV(3);
	std::vector<Mat> rgbPatchPyr_Y(3);
	std::vector<Mat> rgbPatchPyr_UV(3);
	std::vector<Mat> depthPatchPyr(3);
	std::vector<Mat> skinPatchPyr(3);

	for (int y = 0; y < h; ++y)
	{
		for (int x = 0; x < w; ++x)
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
			if(x % 2 == 0 && y % 2 == 0)
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
			if(x % 4 == 0 && y % 4 == 0)
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
				//TODO put skin images in vecs
			}

			//feed network
			switch (scale)
			{
				case 1:
					fillMemoryDataLayers(&caffe_test_net, true, hasRGB, hasDepth, hasSkin, mats_rgb1_Y, mats_rgb1_UV, mats_nir1_Y, mats_nir1_UV, mats_depth1);
					break;
				case 2:
					fillMemoryDataLayers(&caffe_test_net, true, hasRGB, hasDepth, hasSkin, mats_rgb2_Y, mats_rgb2_UV, mats_nir2_Y, mats_nir2_UV, mats_depth2);
					break;
				default:
					fillMemoryDataLayers(&caffe_test_net, true, hasRGB, hasDepth, hasSkin, mats_rgb0_Y, mats_rgb0_UV, mats_nir0_Y, mats_nir0_UV, mats_depth0);
					break;
			}

			//get prediction
			float loss;
//			const vector<Blob<float>*>& result = caffe_test_net.ForwardPrefilled(&loss);
//			// Now result will contain the argmax results.
//			const float* argmaxs = result[0]->cpu_data();
//			for (int i = 0; i < result[0]->num(); ++i)
//			{
//				LOG(INFO) << " Image: "<< i << " class:" << argmaxs[i] << " loss:" << loss;
//			}

			caffe_test_net.ForwardPrefilled(&loss);
//			LOG(INFO) << "loss: " << loss << "\n";
			// read the last layer of the MLP and get the result
			shared_ptr< Blob<float> > prob = caffe_test_net.blob_by_name("linearClassifier");
			float maxval= 0;
			int   maxindex= 0;
			for (int i = 0; i < prob->count(); i++)
			{
				float val= prob->cpu_data()[i];
				if (val> maxval)
				{
					maxval= val;
					maxindex= i;
				}
//				LOG(INFO) << "[" << i << "]" << val<< "\n";
			}
			LOG(INFO) << "y:" << y << " Max value = " << maxval<< ", Max index = " << maxindex<< "\n";


			//paint into label pediction image
			labelImg_predicted.at<uchar>(y,x) = (uchar)(maxindex);

		}
	}

	imshow("NIR image", nirImg);
	imshow("Ground truth labels", labelImg*85);
	imshow("Predicted Labels", labelImg_predicted*85);
	imwrite("predictedLables.png", labelImg_predicted*85);
	cvWaitKey();

	//TODO: compute accuracy for pixel and label (avg?)



//    LOG(INFO) << "loss: " << loss << "\n";
//        // read the last layer of the MLP and get the result
//    shared_ptr< Blob<float> > prob = caffe_test_net.blob_by_name("ip2");
//    float maxval= 0;
//    int   maxinx= 0;
//    for (int i = 0; i < prob->count(); i++)
//    {
//        float val= prob->cpu_data()[i];
//        if (val> maxval)
//        {
//            maxval= val;
//            maxinx= i;
//        }
//        LOG(INFO) << "[" << i << "]" << val<< "\n";
//    }
//    LOG(INFO) << "Max value = " << maxval<< ", Max index = " << maxinx<< "\n";
}

