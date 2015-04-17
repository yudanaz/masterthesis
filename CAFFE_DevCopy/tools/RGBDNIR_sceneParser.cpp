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
						  std::vector<Mat> mats_depth2
						  )
{
	//make dummy label
	std::vector<int> labels(1,0);

	//make vectors to feed to memory data layer
//	std::vector<Mat> mats_rgb0_Y;
//	std::vector<Mat> mats_rgb0_UV;
//	std::vector<Mat> mats_rgb1_Y;
//	std::vector<Mat> mats_rgb1_UV;
//	std::vector<Mat> mats_rgb2_Y;
//	std::vector<Mat> mats_rgb2_UV;

//	std::vector<Mat> mats_nir0_Y;
//	std::vector<Mat> mats_nir0_UV;
//	std::vector<Mat> mats_nir1_Y;
//	std::vector<Mat> mats_nir1_UV;
//	std::vector<Mat> mats_nir2_Y;
//	std::vector<Mat> mats_nir2_UV;

//	std::vector<Mat> mats_depth0;
//	std::vector<Mat> mats_depth1;
//	std::vector<Mat> mats_depth2;

//	if(hasNIR)
//	{
//		std::vector<Mat> patch_nir0_vect;
//		split(nirPatchPyr.at(0), patch_nir0_vect);
//		Mat nir0y = patch_nir0_vect[0];
//		Mat nir0uv;
//		std::vector<Mat> nir0uv_vec;
//		nir0uv_vec.push_back(patch_nir0_vect[1]);
//		nir0uv_vec.push_back(patch_nir0_vect[2]);
//		merge(nir0uv_vec, nir0uv);
//		mats_nir0_Y.push_back(nir0y);
//		mats_nir0_UV.push_back(nir0uv);

//		//level 1
//		std::vector<Mat> patch_nir1_vect;
//		split(nirPatchPyr.at(1), patch_nir1_vect);
//		Mat nir1y = patch_nir1_vect[0];
//		Mat nir1uv;
//		std::vector<Mat> nir1uv_vec;
//		nir1uv_vec.push_back(patch_nir1_vect[1]);
//		nir1uv_vec.push_back(patch_nir1_vect[2]);
//		merge(nir1uv_vec, nir1uv);
//		mats_nir1_Y.push_back(nir1y);
//		mats_nir1_UV.push_back(nir1uv);

//		//level 2
//		std::vector<Mat> patch_nir2_vect;
//		split(nirPatchPyr.at(2), patch_nir2_vect);
//		Mat nir2y = patch_nir2_vect[0];
//		Mat nir2uv;
//		std::vector<Mat> nir2uv_vec;
//		nir2uv_vec.push_back(patch_nir2_vect[1]);
//		nir2uv_vec.push_back(patch_nir2_vect[2]);
//		merge(nir2uv_vec, nir2uv);
//		mats_nir2_Y.push_back(nir2y);
//		mats_nir2_UV.push_back(nir2uv);
//	}
//	if(hasRGB)
//	{
//		std::vector<Mat> patch_rgb0_vect;
//		split(rgbPatchPyr.at(0), patch_rgb0_vect);
//		Mat rgb0y = patch_rgb0_vect[0];
//		Mat rgb0uv;
//		std::vector<Mat> rgb0uv_vec;
//		rgb0uv_vec.push_back(patch_rgb0_vect[1]);
//		rgb0uv_vec.push_back(patch_rgb0_vect[2]);
//		merge(rgb0uv_vec, rgb0uv);
//		mats_rgb0_Y.push_back(rgb0y);
//		mats_rgb0_UV.push_back(rgb0uv);

//		//level 1
//		std::vector<Mat> patch_rgb1_vect;
//		split(rgbPatchPyr.at(1), patch_rgb1_vect);
//		Mat rgb1y = patch_rgb1_vect[0];
//		Mat rgb1uv;
//		std::vector<Mat> rgb1uv_vec;
//		rgb1uv_vec.push_back(patch_rgb1_vect[1]);
//		rgb1uv_vec.push_back(patch_rgb1_vect[2]);
//		merge(rgb1uv_vec, rgb1uv);
//		mats_rgb1_Y.push_back(rgb1y);
//		mats_rgb1_UV.push_back(rgb1uv);

//		//level 2
//		std::vector<Mat> patch_rgb2_vect;
//		split(rgbPatchPyr.at(2), patch_rgb2_vect);
//		Mat rgb2y = patch_rgb2_vect[0];
//		Mat rgb2uv;
//		std::vector<Mat> rgb2uv_vec;
//		rgb2uv_vec.push_back(patch_rgb2_vect[1]);
//		rgb2uv_vec.push_back(patch_rgb2_vect[2]);
//		merge(rgb2uv_vec, rgb2uv);
//		mats_rgb2_Y.push_back(rgb2y);
//		mats_rgb2_UV.push_back(rgb2uv);
//	}
//	if(hasDepth)
//	{
//		mats_depth0.push_back(depthPatchPyr.at(0));
//		mats_depth1.push_back(depthPatchPyr.at(1));
//		mats_depth2.push_back(depthPatchPyr.at(2));
//	}



//	for(int layer_id = 0; layer_id < net->layers().size(); ++layer_id)
	{
//		Layer<float>* layer = net->layers()[layer_id].get(); //get stored pointer from shared pointer

		//if memory data layer, add correct datum vector to layer
//		if( strcmp( layer->type(), "MemoryData") == 0 )
		{
//			std::string nm = layer->layer_param().name();
//			LOG(INFO) << "Got Layer Name:" << nm;

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
		}
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

	if (argc != 6)
	{
		LOG(ERROR) << "PLEASE ENTER 3 ARGUMENTS: (1)net_proto (2)pretrained_net_proto (3)image without suffix (4)[NIR/DNIR/RGBNIR/RGBDNIR/RGBDNIRSkin] (5)[GPU/CPU]";
		return 1;
	}

	//Setting CPU or GPU
    if (argc >= 6 && strcmp(argv[5], "GPU") == 0)
	{
		Caffe::set_mode(Caffe::GPU);
//		int device_id = 0;
//		if (argc == 6)
//		{
//			device_id = atoi(argv[5]);
//		}
//			Caffe::SetDevice(device_id);
//			LOG(ERROR) << "Using GPU #" << device_id;
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
			fillMemoryDataLayers(&caffe_test_net, true, hasRGB, hasDepth, hasSkin,
								 mats_rgb0_Y, mats_rgb0_UV, mats_rgb1_Y, mats_rgb1_UV, mats_rgb2_Y, mats_rgb2_UV,
								 mats_nir0_Y, mats_nir0_UV, mats_nir1_Y, mats_nir1_UV, mats_nir2_Y, mats_nir2_UV,
                                 mats_depth0, mats_depth1, mats_depth2);

			//get prediction
			float loss;
			const vector<Blob<float>*>& result = caffe_test_net.ForwardPrefilled(&loss);
			// Now result will contain the argmax results.
			const float* argmaxs = result[0]->cpu_data();
//			for (int i = 0; i < result[0]->num(); ++i)
//			{
//				LOG(INFO) << " Image: "<< i << " class:" << argmaxs[i];
//			}

			//paint into label pediction image
			labelImg_predicted.at<uchar>(y,x) = (uchar)(argmaxs[0]);

		}
		LOG(INFO) << y;
	}

	imshow("NIR image", nirImg);
	imshow("Ground truth labels", labelImg*85);
	imshow("Predicted Labels", labelImg_predicted*85);
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

