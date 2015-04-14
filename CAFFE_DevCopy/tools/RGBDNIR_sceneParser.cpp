#include <cstring>
#include <cstdlib>
#include <vector>

#include <string>
#include <iostream>
#include <stdio.h>
#include "../../include/caffe/caffe.hpp"
#include "../../include/caffe/util/io.hpp"
#include "../../include/caffe/blob.hpp"
#include "opencv2/opencv.hpp"

using namespace caffe;
using namespace std;


int main(int argc, char** argv)
{
//	if (argc < 4 || argc > 6) {
//	LOG(ERROR) << "PLEASE ENTER 5 ARGUMENTS: (1) test_net (2) net_proto (3) pretrained_net_proto (4) iterations (5) [CPU/GPU]";
//	return 1;
//	}

	if (argc != 4) {
	LOG(ERROR) << "PLEASE ENTER 3 ARGUMENTS: (1) net_proto (2) pretrained_net_proto (3) image";
	return 1;
	}

//	//Setting CPU or GPU
//	if (argc >= 5 && strcmp(argv[4], "GPU") == 0)
//	{
//		Caffe::set_mode(Caffe::GPU);
//		int device_id = 0;
//		if (argc == 6)
//		{
//			device_id = atoi(argv[5]);
//		}
//			Caffe::SetDevice(device_id);
//			LOG(ERROR) << "Using GPU #" << device_id;
//	}
//	else
	{
		LOG(ERROR) << "Using CPU";
		Caffe::set_mode(Caffe::CPU);
	}

	//get the net
	Net<float> caffe_test_net(argv[1], caffe::TEST);
	//get trained net
	caffe_test_net.CopyTrainedLayersFrom(argv[2]);

	// load image files, resize and added to vector
	cv::Mat image = cv::imread(argv[3]);
	cv::Mat rszimage;

	// The mean file image size is 256x256, need to resize the input image to 256x256
	cv::resize(image, rszimage, cv::Size(256, 256));

	std::vector<cv::Mat> images(1, rszimage);
	std::vector<int> labels(1, 0);

	shared_ptr< MemoryDataLayer<float> > md_layer =
		boost::dynamic_pointer_cast < MemoryDataLayer<float> >(caffe_test_net.layers()[0]);
	if (!md_layer) {
		std::cout << "The first layer is not a MemoryDataLayer!\n";
		return -1;
	}

	md_layer->AddMatVector(images, labels);

	float loss;
	caffe_test_net.ForwardPrefilled(&loss);
	std::cout << "loss: " << loss << "\n";
		// read the 'prob' layer and get the result
	shared_ptr< Blob<float> > prob = caffe_test_net.blob_by_name("prob");
	float maxval= 0;
	int   maxinx= 0;
	for (int i = 0; i < prob->count(); i++)
	{
		float val= prob->cpu_data()[i];
		if (val> maxval)
		{
			maxval= val;
			maxinx= i;
		}
		std::cout << "[" << i << "]" << val<< "\n";
	}
	std::cout << "Max value = " << maxval<< ", Max index = " << maxinx<< "\n";
}

