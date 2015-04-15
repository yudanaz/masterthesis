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

using namespace caffe;
using namespace std;
using namespace cv;

///////////////////////////////////////////////////////////////////////////////
//copied from netrgbdnir class:
///////////////////////////////////////////////////////////////////////////////
//void normalizeEachChannelLocally(Mat &img, int kernel)
//{
//    std::vector<Mat> chs(3);
//    split(img, chs);
//    for(int i = 0; i < 3; ++i)
//    {
//        normalizeLocally(chs[i], kernel);
////        normalizeLocally2(chs[i], kernel);
//    }
//    Mat img_;
//    merge(chs, img_);
//    img = img_;
//}

//vector<Mat> makePyramid(Mat img, int leveln, int interpolMethod)
//{
//    vector<Mat> levels;
//    Mat procImg;

//    levels.push_back(img);
//    for(int i = 1; i < leveln; ++i)
//    {
//        resize(img, procImg, Size((img.cols+1)/2, (img.rows+1)/2), 0, 0, interpolMethod);
//        levels.push_back(procImg.clone());
//        img = procImg.clone();
//    }
//    return levels;
//}

///////////////////////////////////////////////////////////////////////////////
//endof copied from netrgbdnir class:
///////////////////////////////////////////////////////////////////////////////

void fillMemoryDataLayers(Net<float> *net, bool hasNIR, bool hasRGB, bool hasDepth,
                          std::vector<Mat> nirPyr,
                          std::vector<Mat> depthPyr,
                          std::vector<Mat> rgbPyr
                          )
{
    //make dummy label
    std::vector<int> labels(1,0);

    //make vectors to feed to memory data layer
    std::vector<Mat> mats_rgb0_Y;
    std::vector<Mat> mats_rgb0_UV;
    std::vector<Mat> mats_rgb1_Y;
    std::vector<Mat> mats_rgb1_UV;
    std::vector<Mat> mats_rgb2_Y;
    std::vector<Mat> mats_rgb2_UV;

    std::vector<Mat> mats_nir0_Y;
    std::vector<Mat> mats_nir0_UV;
    std::vector<Mat> mats_nir1_Y;
    std::vector<Mat> mats_nir1_UV;
    std::vector<Mat> mats_nir2_Y;
    std::vector<Mat> mats_nir2_UV;

    std::vector<Mat> mats_depth0;
    std::vector<Mat> mats_depth1;
    std::vector<Mat> mats_depth2;

    if(hasNIR)
    {
        std::vector<Mat> patch_nir0_vect;
        split(nirPyr.at(0), patch_nir0_vect);
        Mat nir0y = patch_nir0_vect[0];
        Mat nir0uv;
        std::vector<Mat> nir0uv_vec;
        nir0uv_vec.push_back(patch_nir0_vect[1]);
        nir0uv_vec.push_back(patch_nir0_vect[2]);
        merge(nir0uv_vec, nir0uv);
        mats_nir0_Y.push_back(nir0y);
        mats_nir0_UV.push_back(nir0uv);

        //level 1
        std::vector<Mat> patch_nir1_vect;
        split(nirPyr.at(1), patch_nir1_vect);
        Mat nir1y = patch_nir1_vect[0];
        Mat nir1uv;
        std::vector<Mat> nir1uv_vec;
        nir1uv_vec.push_back(patch_nir1_vect[1]);
        nir1uv_vec.push_back(patch_nir1_vect[2]);
        merge(nir1uv_vec, nir1uv);
        mats_nir1_Y.push_back(nir1y);
        mats_nir1_UV.push_back(nir1uv);

        //level 2
        std::vector<Mat> patch_nir2_vect;
        split(nirPyr.at(2), patch_nir2_vect);
        Mat nir2y = patch_nir2_vect[0];
        Mat nir2uv;
        std::vector<Mat> nir2uv_vec;
        nir2uv_vec.push_back(patch_nir2_vect[1]);
        nir2uv_vec.push_back(patch_nir2_vect[2]);
        merge(nir2uv_vec, nir2uv);
        mats_nir2_Y.push_back(nir2y);
        mats_nir2_UV.push_back(nir2uv);
    }



    for(int layer_id = 0; layer_id < net->layers().size(); ++layer_id)
    {
        Layer<float>* layer = net->layers()[layer_id].get(); //get stored pointer from shared pointer

        //if memory data layer, add correct datum vector to layer
        if( strcmp( layer->type(), "MemoryData") == 0 )
        {
            std::string nm = layer->layer_param().name();
            LOG(INFO) << "Got Layer Name:" << nm;

            //NIR//////////////////////////////////////////////////////////////
            if(hasNIR)
            {
                if(nm == "nir0_Y")
                    { ((MemoryDataLayer<float>*)layer)->AddMatVector(mats_nir0_Y, labels);}
                else if(nm == "nir0_UV")
                    { ((MemoryDataLayer<float>*)layer)->AddMatVector(mats_nir0_UV, labels); }
                else if(nm == "nir1_Y")
                    { ((MemoryDataLayer<float>*)layer)->AddMatVector(mats_nir1_Y, labels); }
                else if(nm == "nir1_UV")
                    { ((MemoryDataLayer<float>*)layer)->AddMatVector(mats_nir1_UV, labels); }
                else if(nm == "nir2_Y")
                    { ((MemoryDataLayer<float>*)layer)->AddMatVector(mats_nir2_Y, labels); }
                else if(nm == "nir2_UV")
                    { ((MemoryDataLayer<float>*)layer)->AddMatVector(mats_nir2_UV, labels); }
            }

            //DEPTH////////////////////////////////////////////////////////////
            if(hasDepth)
            {
                if(nm == "depth0")
                    {((MemoryDataLayer<float>*)layer)->AddMatVector(mats_depth0, labels); }
                else if(nm == "depth1")
                    { ((MemoryDataLayer<float>*)layer)->AddMatVector(mats_depth1, labels); }
                else if(nm == "depth2")
                    { ((MemoryDataLayer<float>*)layer)->AddMatVector(mats_depth2, labels); }
            }

            //RGB//////////////////////////////////////////////////////////////
            if(hasRGB)
            {
                if(nm == "rgb0_Y")
                    { ((MemoryDataLayer<float>*)layer)->AddMatVector(mats_rgb0_Y, labels); }
                else if(nm == "rgb0_UV")
                    { ((MemoryDataLayer<float>*)layer)->AddMatVector(mats_rgb0_UV, labels); }
                else if(nm == "rgb1_Y")
                    { ((MemoryDataLayer<float>*)layer)->AddMatVector(mats_rgb1_Y, labels); }
                else if(nm == "rgb1_UV")
                    { ((MemoryDataLayer<float>*)layer)->AddMatVector(mats_rgb1_UV, labels); }
                else if(nm == "rgb2_Y")
                    { ((MemoryDataLayer<float>*)layer)->AddMatVector(mats_rgb2_Y, labels); }
                else if(nm == "rgb2_UV")
                    { ((MemoryDataLayer<float>*)layer)->AddMatVector(mats_rgb2_UV, labels); }
            }
        }
    }
}



int main(int argc, char** argv)
{
    RGBDNIR_preproc preproc;


    if (argc != 5)
    {
        LOG(ERROR) << "PLEASE ENTER 3 ARGUMENTS: (1) net_proto (2) pretrained_net_proto (3) image (4) [NIR/DNIR/RGBNIR/RGBDNIR]";
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
        LOG(INFO) << "Using CPU";
		Caffe::set_mode(Caffe::CPU);
	}

	//get the net
    Net<float> caffe_test_net(argv[1], caffe::TEST);
    LOG(INFO) << "Success: Created net: " << argv[1];
	//get trained net
	caffe_test_net.CopyTrainedLayersFrom(argv[2]);
    LOG(INFO) << "Success: Loaded trained layers from " << argv[2];

	// load image files, resize and added to vector
    Mat NIRimage = cv::imread(argv[3]);
//    imshow("loaded image", image); cvWaitKey();
    LOG(INFO) << "Success: Loaded image: " << argv[3];

//	shared_ptr< MemoryDataLayer<float> > md_layer =
//		boost::dynamic_pointer_cast < MemoryDataLayer<float> >(caffe_test_net.layers()[0]);
//	if (!md_layer) {
//		LOG(INFO) << "The first layer is not a MemoryDataLayer!\n";
//		return -1;
//	}
//	md_layer->AddMatVector(images, labels);

    //TEMP//
    cv::cvtColor(NIRimage, NIRimage, CV_BGR2YCrCb);
    std::vector<Mat> nirPyr;// = preproc.makePyramid(NIRimage, 3);
    nirPyr.push_back(NIRimage);nirPyr.push_back(NIRimage);nirPyr.push_back(NIRimage);
    std::vector<Mat> depthPyr;
    std::vector<Mat> rgbPyr;

    //compute zero mean and unit variance for each channel
    for(int i = 0; i < 3; ++i)
    {
        Mat pyrLevel = nirPyr[i];
        preproc.normalizeEachChannelLocally(pyrLevel, 15);
    }
    //TEMP//

    //!!!TODO: load whole image, make pyramid and normalization there, then cut out patches!!

    fillMemoryDataLayers(&caffe_test_net, true, false, false, nirPyr, depthPyr, rgbPyr);
    LOG(INFO) << "Success: Filled memory layers.";

	float loss;
    const vector<Blob<float>*>& result = caffe_test_net.ForwardPrefilled(&loss);
    // Now result will contain the argmax results.
    const float* argmaxs = result[0]->cpu_data();
    for (int i = 0; i < result[0]->num(); ++i)
    {
        LOG(INFO) << " Image: "<< i << " class:" << argmaxs[i];
    }



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

