#include "../../include/caffe/netrgbdnir.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include "../../include/caffe/data_layers.hpp"
#include "../../include/caffe/util/io.hpp"

//#define DEBUG_IMG

using namespace cv;

namespace caffe {

template<typename Dtype>
void NetRGBDNIR<Dtype>::setup(std::string imgsListURL, int patchsize, int batchSize, int batchesPerImage,
							  bool RGB, bool NIR, bool depth, bool skin, bool isMultiscale, std::string imageType, std::string labelImageSuffix)
{
	//LOG(INFO) << "Doing Setup of NetRGBDNIR";

	//read image names from file to list
	std::ifstream inFile;
	inFile.open((char*)imgsListURL.c_str());
	std::string line;
	imgCnt = -1; //init like this so it would jump to zero in first call of readNextImage()
	imgMax = 0;
	while(std::getline(inFile, line))
	{
		imgs.push_back(line);

        vector<long> randomImgPixels;
		randomPixels.push_back(randomImgPixels);
		randomPixelIndices.push_back(0);
//        imgs_uniformSubpatchSize.push_back(0); //just init here, set correct value in readNextImage()
//        imgs_uniformSubpatchIndex.push_back(0); //init here, incremented in setUniformPatches()
//        imgs_uniformSubpatchIndex_inits.push_back(false); //set all to "not initialized with random start index"

		imgMax++;
	}
//    LOG(INFO) << "max imgs in list: " << imgMax;
	inFile.close();

	//set the rest
	patchSz = patchsize;
	borderSz = patchsize / 2;
	patchCnt = 0;
	patchMax = 0; //is set correctly in readNextImage() each time an image is read, in case image sizes differ
	batchSz = batchSize;
    batchNr = 0;
	hasRGB = RGB;
	hasNIR = NIR;
	hasDepth = depth;
	hasSkin = skin;
	multiscale = isMultiscale;
	imgType = imageType;
	labelImgSuffix = labelImageSuffix;
	batchesPerImg = batchesPerImage;
	jitter_flipping = false;
	jitter_rotAngle = 0;
	jitter_scale_fac = 0;

	//load all images and set first one to active
	readAllImages();

	//debug
	iteration = 0;
}

template<typename Dtype>
void NetRGBDNIR<Dtype>::feedNextPatchesToInputLayers()
{
//    LOG(INFO) << "Feeding new patches to memory data layers";
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

	std::vector<Mat> mats_skin0;
	std::vector<Mat> mats_skin1;
	std::vector<Mat> mats_skin2;

	std::vector<int> labels;

	int batchCnt = 0;
	for(batchCnt = 0; batchCnt < batchSz; )
	{
		//load next image if enough batches have been read from the current image
		if(batchNr == 0)
		{
			getNextImage();
			batchNr = 0;
//            LOG(INFO) << "current image: " << imgCnt;
		}
		batchNr = (batchNr + 1) % batchesPerImg;


		//get the next random pixel, if the label is "unknown", go to next pixel
		int currentLabel, x, y;
		do
		{
			//get the correct pixel indices
			int w = img_labels.cols;
			int randomPixel = getNextRandomPixel();
			x = randomPixel % w;
			y = randomPixel / w;

			//get label for this patch
			currentLabel = img_labels.at<uchar>(y,x);
//                LOG(INFO) << "label: " << currentLabel <<" current pos: " << x << "  " << y << " random pixel: " << randomPixel;
		}
		while(currentLabel == 255); //jump the "unknown" label, training with this label is useless
		labels.push_back(currentLabel);


		///////////////////////////////////////////////////////////////////////
		/// READ PATCHES
		///////////////////////////////////////////////////////////////////////

		//set random variables for jitter (scaling, rotation and flip)
		setJitterRandomVars();

//		LOG(INFO) << "Get Patch for Scale 0";
		//read the current patch from the image (orig. scale 0)
		if(hasRGB)
		{
			Mat patch_rgb0 = getImgPatch(img_rgb0, x, y);
			preproc.normalizeEachChannelLocally(patch_rgb0, 15);
			std::vector<Mat> patch_rgb0_vect;
			split(patch_rgb0, patch_rgb0_vect);
			Mat rgb0y = patch_rgb0_vect[0];
			Mat rgb0uv;
			std::vector<Mat> rgb0uv_vec;
			rgb0uv_vec.push_back(patch_rgb0_vect[1]);
			rgb0uv_vec.push_back(patch_rgb0_vect[2]);
			merge(rgb0uv_vec, rgb0uv);
			mats_rgb0_Y.push_back(rgb0y);
			mats_rgb0_UV.push_back(rgb0uv);
#ifdef DEBUG_IMG
			imshow("rgb0y", rgb0y);
			imwrite("/home/maurice/rgb0y.png", rgb0y); cvWaitKey();
#endif

			if(multiscale)
			{
				//level 1
				Mat patch_rgb1 = getImgPatch(img_rgb1, x/2, y/2);
				preproc.normalizeEachChannelLocally(patch_rgb1, 15);
				std::vector<Mat> patch_rgb1_vect;
				split(patch_rgb1, patch_rgb1_vect);
				Mat rgb1y = patch_rgb1_vect[0];
				Mat rgb1uv;
				std::vector<Mat> rgb1uv_vec;
				rgb1uv_vec.push_back(patch_rgb1_vect[1]);
				rgb1uv_vec.push_back(patch_rgb1_vect[2]);
				merge(rgb1uv_vec, rgb1uv);
				mats_rgb1_Y.push_back(rgb1y);
				mats_rgb1_UV.push_back(rgb1uv);
#ifdef DEBUG_IMG
				imshow("rgb1y", rgb1y);
				imwrite("/home/maurice/rgb1y.png", rgb1y); cvWaitKey();
#endif

				//level 2
				Mat patch_rgb2 = getImgPatch(img_rgb2, x/4, y/4);
				preproc.normalizeEachChannelLocally(patch_rgb2, 15);
				std::vector<Mat> patch_rgb2_vect;
				split(patch_rgb2, patch_rgb2_vect);
				Mat rgb2y = patch_rgb2_vect[0];
				Mat rgb2uv;
				std::vector<Mat> rgb2uv_vec;
				rgb2uv_vec.push_back(patch_rgb2_vect[1]);
				rgb2uv_vec.push_back(patch_rgb2_vect[2]);
				merge(rgb2uv_vec, rgb2uv);
				mats_rgb2_Y.push_back(rgb2y);
				mats_rgb2_UV.push_back(rgb2uv);
#ifdef DEBUG_IMG
				imshow("rgb2y", rgb2y);
				imwrite("/home/maurice/rgb2y.png", rgb2y); cvWaitKey();
#endif
			}
		}

		if(hasNIR)
		{
			Mat patch_nir0 = getImgPatch(img_nir0, x, y);
			preproc.normalizeEachChannelLocally(patch_nir0, 15);
			std::vector<Mat> patch_nir0_vect;
			split(patch_nir0, patch_nir0_vect);
			Mat nir0y = patch_nir0_vect[0];
			Mat nir0uv;
			std::vector<Mat> nir0uv_vec;
			nir0uv_vec.push_back(patch_nir0_vect[1]);
			nir0uv_vec.push_back(patch_nir0_vect[2]);
			merge(nir0uv_vec, nir0uv);
			mats_nir0_Y.push_back(nir0y);
			mats_nir0_UV.push_back(nir0uv);
#ifdef DEBUG_IMG
			imshow("nir0y", nir0y);
			imwrite("/home/maurice/nir0y.png", nir0y); cvWaitKey();
#endif

			if(multiscale)
			{
				//level 1
				Mat patch_nir1 = getImgPatch(img_nir1, x/2, y/2);
				preproc.normalizeEachChannelLocally(patch_nir1, 15);
				std::vector<Mat> patch_nir1_vect;
				split(patch_nir1, patch_nir1_vect);
				Mat nir1y = patch_nir1_vect[0];
				Mat nir1uv;
				std::vector<Mat> nir1uv_vec;
				nir1uv_vec.push_back(patch_nir1_vect[1]);
				nir1uv_vec.push_back(patch_nir1_vect[2]);
				merge(nir1uv_vec, nir1uv);
				mats_nir1_Y.push_back(nir1y);
				mats_nir1_UV.push_back(nir1uv);
#ifdef DEBUG_IMG
				imshow("nir1y", nir1y);
				imwrite("/home/maurice/nir1y.png", nir1y); cvWaitKey();
#endif

				//level 2
				Mat patch_nir2 = getImgPatch(img_nir2, x/4, y/4);
				preproc.normalizeEachChannelLocally(patch_nir2, 15);
				std::vector<Mat> patch_nir2_vect;
				split(patch_nir2, patch_nir2_vect);
				Mat nir2y = patch_nir2_vect[0];
				Mat nir2uv;
				std::vector<Mat> nir2uv_vec;
				nir2uv_vec.push_back(patch_nir2_vect[1]);
				nir2uv_vec.push_back(patch_nir2_vect[2]);
				merge(nir2uv_vec, nir2uv);
				mats_nir2_Y.push_back(nir2y);
				mats_nir2_UV.push_back(nir2uv);
#ifdef DEBUG_IMG
				imshow("nir2y", nir2y);
				imwrite("/home/maurice/nir2y.png", nir2y); cvWaitKey();
#endif
			}
		}

		if(hasDepth)
		{
			mats_depth0.push_back(getImgPatch(img_depth0, x, y, true));
#ifdef DEBUG_IMG
			imshow("depth0", mats_depth0.at(0));
			imwrite("/home/maurice/depth0.png", mats_depth0.at(0)); cvWaitKey();
#endif
			if(multiscale)
			{
				mats_depth1.push_back(getImgPatch(img_depth1, x/2, y/2, true));
				mats_depth2.push_back(getImgPatch(img_depth2, x/4, y/4, true));
#ifdef DEBUG_IMG
				imshow("depth1", mats_depth1.at(0));
				imwrite("/home/maurice/depth1.png", mats_depth1.at(0)); cvWaitKey();
				imshow("depth2", mats_depth2.at(0));
				imwrite("/home/maurice/depth2.png", mats_depth2.at(0)); cvWaitKey();
#endif
			}
		}

		if(hasSkin)
		{
			mats_skin0.push_back(getImgPatch(img_skin0, x, y, true));
#ifdef DEBUG_IMG
			imshow("skin0", mats_skin0.at(0));
			imshow("/home/maurice/skin0.png", mats_skin0.at(0)); cvWaitKey();
#endif
			if(multiscale)
			{
				mats_skin1.push_back(getImgPatch(img_skin1, x/2, y/2, true));
				mats_skin2.push_back(getImgPatch(img_skin2, x/4, y/4, true));
#ifdef DEBUG_IMG
				imshow("skin1", mats_skin1.at(0));
				imwrite("/home/maurice/skin1.png", mats_skin1.at(0)); cvWaitKey();
				imshow("skin2", mats_skin2.at(0));
				imwrite("/home/maurice/skin2.png", mats_skin2.at(0)); cvWaitKey();
#endif
			}
		}

		batchCnt++;
		patchCnt++;
	}


	//LOG(INFO) << "Feed Datum vectors to Memory Data Layers";
	//feed images to corresponding memory_data_layers. We have different input layers for
	//RGB, NIR and Depth, so convolution won't mix those channels, plus 3 different scales
	//I.e. in total there are 3 * 3 = 9 input layers
	//NIR//////////////////////////////////////////////////////////////
	if(hasNIR)
	{
		((MemoryDataLayer<float>*)this->layer_by_name("nir0_Y").get())->AddMatVector(mats_nir0_Y, labels);
		((MemoryDataLayer<float>*)this->layer_by_name("nir0_UV").get())->AddMatVector(mats_nir0_UV, labels);
		((MemoryDataLayer<float>*)this->layer_by_name("nir1_Y").get())->AddMatVector(mats_nir1_Y, labels);
		((MemoryDataLayer<float>*)this->layer_by_name("nir1_UV").get())->AddMatVector(mats_nir1_UV, labels);
		((MemoryDataLayer<float>*)this->layer_by_name("nir2_Y").get())->AddMatVector(mats_nir2_Y, labels);
		((MemoryDataLayer<float>*)this->layer_by_name("nir2_UV").get())->AddMatVector(mats_nir2_UV, labels);
	}

	//DEPTH////////////////////////////////////////////////////////////
	if(hasDepth)
	{
		((MemoryDataLayer<float>*)this->layer_by_name("depth0").get())->AddMatVector(mats_depth0, labels);
		((MemoryDataLayer<float>*)this->layer_by_name("depth1").get())->AddMatVector(mats_depth1, labels);
		((MemoryDataLayer<float>*)this->layer_by_name("depth2").get())->AddMatVector(mats_depth2, labels);
	}

	//RGB//////////////////////////////////////////////////////////////
	if(hasRGB)
	{
		((MemoryDataLayer<float>*)this->layer_by_name("rgb0_Y").get())->AddMatVector(mats_rgb0_Y, labels);
		((MemoryDataLayer<float>*)this->layer_by_name("rgb0_UV").get())->AddMatVector(mats_rgb0_UV, labels);
		((MemoryDataLayer<float>*)this->layer_by_name("rgb1_Y").get())->AddMatVector(mats_rgb1_Y, labels);
		((MemoryDataLayer<float>*)this->layer_by_name("rgb1_UV").get())->AddMatVector(mats_rgb1_UV, labels);
		((MemoryDataLayer<float>*)this->layer_by_name("rgb2_Y").get())->AddMatVector(mats_rgb2_Y, labels);
		((MemoryDataLayer<float>*)this->layer_by_name("rgb2_UV").get())->AddMatVector(mats_rgb2_UV, labels);
	}

	//SKIN/////////////////////////////////////////////////////////////
	if(hasSkin)
	{
		((MemoryDataLayer<float>*)this->layer_by_name("skin0").get())->AddMatVector(mats_skin0, labels);
		((MemoryDataLayer<float>*)this->layer_by_name("skin1").get())->AddMatVector(mats_skin1, labels);
		((MemoryDataLayer<float>*)this->layer_by_name("skin2").get())->AddMatVector(mats_skin2, labels);
	}

	//debug
	iteration++;
}

template<typename Dtype>
void NetRGBDNIR<Dtype>::getNextImage()
{
	//increment image counter
	imgCnt = (imgCnt + 1) % imgMax;

	img_labels = imgs_labels.at(imgCnt);

	if(hasNIR)
	{
		img_nir0 = imgs_nir0.at(imgCnt);
		img_nir1 = imgs_nir1.at(imgCnt);
		img_nir2 = imgs_nir2.at(imgCnt);
	}
	if(hasRGB)
	{
		img_rgb0 = imgs_rgb0.at(imgCnt);
		img_rgb1 = imgs_rgb1.at(imgCnt);
		img_rgb2 = imgs_rgb2.at(imgCnt);
	}
	if(hasDepth)
	{
		img_depth0 = imgs_depth0.at(imgCnt);
		img_depth1 = imgs_depth1.at(imgCnt);
		img_depth2 = imgs_depth2.at(imgCnt);
	}
	if(hasSkin)
	{
		img_skin0 = imgs_skin0.at(imgCnt);
		img_skin1 = imgs_skin1.at(imgCnt);
		img_skin2 = imgs_skin2.at(imgCnt);
	}
}


template<typename Dtype>
void NetRGBDNIR<Dtype>::readAllImages()
{
    srand(time(0)); //set seed for random generator using current time

	for (int i = 0; i < imgMax; ++i)
	{
		//get next image URL, also circle through images (if iterations are > all available patches)
		std::string imageURL = imgs[i];
		LOG(INFO) << "Reading image " << i << ": " << imageURL;

		//load all image types (RGB, NIR and Depth) if available, create scales (image pyramid) pad images (make borders)
		std::string labelsNm = imageURL + labelImgSuffix + std::string(".png"); //labels lossless, always png
	//    std::string rgbNm = imageURL + std::string(".") + imgType; //for stanford
		std::string rgbNm = imageURL + std::string("_rgb.") + imgType;
		std::string nirNm = imageURL + std::string("_nir.") + imgType;
		std::string depthNm = imageURL + std::string("_depth.png"); //depth lossless, always png
		std::string skinNm = imageURL + std::string("_skin.png"); //skin binary image also lossless, always png

	//	LOG(INFO) << "Reading image " << imageURL;
		Mat labels = cv::imread(labelsNm, cv::IMREAD_GRAYSCALE); //label img isn't downsampled nor padded
		imgs_labels.push_back(labels);

		//make a shuffled list of pixel indices for each image
		if(randomPixels.at(i).size() == 0) //vector for current image not initialized yet
		{
			//fill vector holding all image pixels for current image, then shuffle it
			int totalNrOfPixels = labels.cols * labels.rows;

//            LOG(INFO) << "cols: " << labels.cols << " rows: " << labels.rows << "pixels: " << totalNrOfPixels;


			for (int j = 0; j < totalNrOfPixels; ++j)
			{
				randomPixels.at(i).push_back(j);
			}
			std::random_shuffle(randomPixels.at(i).begin(), randomPixels.at(i).end());
		}

		cv::Mat temp1;//, temp2, temp3;

		//Make the downsized images for the image pyramid and add the padding according to patch size
	//    LOG(INFO) << "Making scales for RGB";
		if(hasRGB)
		{
			//read image and convert ot YUV color space
			temp1 = cv::imread(rgbNm, cv::IMREAD_COLOR);
			cvtColor(temp1, temp1, CV_BGR2YCrCb);
			Mat rgb0, rgb1, rgb2;

			if(!multiscale)
			{
//				preproc.normalizeEachChannelLocally(temp1, 15);
				cv::copyMakeBorder(temp1, rgb0, borderSz, borderSz-1, borderSz, borderSz-1, cv::BORDER_CONSTANT, cv::Scalar(0)); //scale 0
			}
			else //make image pyramid
			{
				std::vector<Mat> pyramid = preproc.makePyramid(temp1, 3);

//				//compute zero mean and unit variance for each channel
//				for(int i = 0; i < 3; ++i)
//				{
//					preproc.normalizeEachChannelLocally(pyramid[i], 15);
//				}

				//make border padding around image for each pyramid level, border is whole patchsize to allow
				//for artificial jitter (rotation & scale), then the patches are cutout after applying jitter
				cv::copyMakeBorder(pyramid[0], rgb0, patchSz, patchSz-1, patchSz, patchSz-1, cv::BORDER_CONSTANT, cv::Scalar(0)); //scale 0
				cv::copyMakeBorder(pyramid[1], rgb1, patchSz, patchSz-1, patchSz, patchSz-1, cv::BORDER_CONSTANT, cv::Scalar(0)); //scale 1 (half the size)
				cv::copyMakeBorder(pyramid[2], rgb2, patchSz, patchSz-1, patchSz, patchSz-1, cv::BORDER_CONSTANT, cv::Scalar(0)); //scale 2 (1/4 the size)
			}
			imgs_rgb0.push_back(rgb0);
			imgs_rgb1.push_back(rgb1);
			imgs_rgb2.push_back(rgb2);
		}

	//    LOG(INFO) << "Making scales for NIR";
		if(hasNIR)
		{
			temp1 = cv::imread(nirNm, cv::IMREAD_COLOR);
			cvtColor(temp1, temp1, CV_BGR2YCrCb);
			Mat nir0, nir1, nir2;

			if(!multiscale)
			{
//				preproc.normalizeEachChannelLocally(temp1, 15);
				cv::copyMakeBorder(temp1, nir0, borderSz, borderSz-1, borderSz, borderSz-1, cv::BORDER_CONSTANT, cv::Scalar(0)); //scale 0
			}
			else //make image pyramid
			{
				std::vector<Mat> pyramid = preproc.makePyramid(temp1, 3);

				//compute zero mean and unit variance for each channel
//				for(int i = 0; i < 3; ++i)
//				{
//					preproc.normalizeEachChannelLocally(pyramid[i], 15);
//				}

				//make border padding around image for each pyramid level, border is whole patchsize to allow
				//for artificial jitter (rotation & scale), then the patches are cutout after applying jitter
				cv::copyMakeBorder(pyramid[0], nir0, patchSz, patchSz-1, patchSz, patchSz-1, cv::BORDER_CONSTANT, cv::Scalar(0)); //scale 0
				cv::copyMakeBorder(pyramid[1], nir1, patchSz, patchSz-1, patchSz, patchSz-1, cv::BORDER_CONSTANT, cv::Scalar(0)); //scale 1 (half the size)
				cv::copyMakeBorder(pyramid[2], nir2, patchSz, patchSz-1, patchSz, patchSz-1, cv::BORDER_CONSTANT, cv::Scalar(0)); //scale 2 (1/4 the size)
			}
			imgs_nir0.push_back(nir0);
			imgs_nir1.push_back(nir1);
			imgs_nir2.push_back(nir2);
		}

	//    LOG(INFO) << "Making scales for Depth";
		if(hasDepth)
		{
			temp1 = cv::imread(depthNm, cv::IMREAD_GRAYSCALE);
			Mat depth0, depth1, depth2;

			if(!multiscale)
			{
	//            preproc.normalizeEachChannelLocally(temp1, 15);
				cv::copyMakeBorder(temp1, depth0, borderSz, borderSz-1, borderSz, borderSz-1, cv::BORDER_CONSTANT, cv::Scalar(0)); //scale 0
			}
			else //make image pyramid
			{
				std::vector<Mat> pyramid = preproc.makePyramid(temp1, 3, INTER_NEAREST); //for depth, no interpolation when resizing

				//make border padding around image for each pyramid level, border is whole patchsize to allow
				//for artificial jitter (rotation & scale), then the patches are cutout after applying jitter
				cv::copyMakeBorder(pyramid[0], depth0, patchSz, patchSz-1, patchSz, patchSz-1, cv::BORDER_CONSTANT, cv::Scalar(0)); //scale 0
				cv::copyMakeBorder(pyramid[1], depth1, patchSz, patchSz-1, patchSz, patchSz-1, cv::BORDER_CONSTANT, cv::Scalar(0)); //scale 1 (half the size)
				cv::copyMakeBorder(pyramid[2], depth2, patchSz, patchSz-1, patchSz, patchSz-1, cv::BORDER_CONSTANT, cv::Scalar(0)); //scale 2 (1/4 the size)
			}
			imgs_depth0.push_back(depth0);
			imgs_depth1.push_back(depth1);
			imgs_depth2.push_back(depth2);
		}

		if(hasSkin)
		{
			temp1 = cv::imread(skinNm, cv::IMREAD_GRAYSCALE);
			Mat skin0, skin1, skin2;

			if(!multiscale)
			{
				cv::copyMakeBorder(temp1, skin0, borderSz, borderSz-1, borderSz, borderSz-1, cv::BORDER_CONSTANT, cv::Scalar(0)); //scale 0
			}
			else //make image pyramid
			{
				std::vector<Mat> pyramid = preproc.makePyramid(temp1, 3, INTER_NEAREST); //for skin, no interpolation when resizing
				cv::copyMakeBorder(pyramid[0], skin0, patchSz, patchSz-1, patchSz, patchSz-1, cv::BORDER_CONSTANT, cv::Scalar(0)); //scale 0
				cv::copyMakeBorder(pyramid[1], skin1, patchSz, patchSz-1, patchSz, patchSz-1, cv::BORDER_CONSTANT, cv::Scalar(0)); //scale 1 (half the size)
				cv::copyMakeBorder(pyramid[2], skin2, patchSz, patchSz-1, patchSz, patchSz-1, cv::BORDER_CONSTANT, cv::Scalar(0)); //scale 2 (1/4 the size)
			}
			imgs_skin0.push_back(skin0);
			imgs_skin1.push_back(skin1);
			imgs_skin2.push_back(skin2);
		}
	//    LOG(INFO) << "done";
	}
}//endof readAllImages



template<typename Dtype>
cv::Mat NetRGBDNIR<Dtype>::getImgPatch(cv::Mat img, int x, int y, bool isDepth)
{
	//cut out patch twice the size
	cv::Mat patch2x, patchJitter, patch;
	int patchSz2x = patchSz*2;
	cv::Rect roi(x, y, patchSz2x, patchSz2x);
	img(roi).copyTo(patch2x);
//    imshow("patch", patch2x); cvWaitKey();
//    imwrite("/home/maurice/"+s+"orig.png", img);
//    imwrite("/home/maurice/"+s+"patch.png", patch2x);

	//apply jitter and crop to actual patch size
	if(isDepth){ patchJitter = makeJitter(patch2x, true); }
	else{ patchJitter = makeJitter(patch2x); }
//    Mat patchrgb;
//	cvtColor(patchJitter, patchrgb, CV_YCrCb2BGR);
//	imshow("patch with jitter", patchrgb); cvWaitKey();
//    imwrite("/home/maurice/"+s+"patch_jitter.png", patchJitter);

	int offset = borderSz - (patchSz2x - patchJitter.cols) / 2; //cols and rows should be equal, it's a square after all
	cv::Rect roi2(offset, offset, patchSz, patchSz);
	patchJitter(roi2).copyTo(patch);
//    cvtColor(patch, patchrgb, CV_YCrCb2BGR);
//    imshow("patch with jitter resized", patchrgb); cvWaitKey();
	return patch;
}

template<typename Dtype>
void NetRGBDNIR<Dtype>::setJitterRandomVars()
{
	//make random numbers for flip, rescale and rotation
	boost::uniform_int<> dist_flip(0, 1);
	boost::uniform_int<> dist_rotate(-8, 8);
	boost::uniform_int<> dist_scale(90, 110);
	boost::variate_generator<boost::mt19937&, boost::uniform_int<> > die_flip(gen, dist_flip);
	boost::variate_generator<boost::mt19937&, boost::uniform_int<> > die_rotate(gen, dist_rotate);
	boost::variate_generator<boost::mt19937&, boost::uniform_int<> > die_scale(gen, dist_scale);

	jitter_flipping = die_flip();
	jitter_rotAngle = die_rotate();
	jitter_scale_fac = (float)die_scale() / 100.0;
}

template<typename Dtype>
Mat NetRGBDNIR<Dtype>::makeJitter(Mat img, bool noInterpolation)
{
	int interpolationMethod = INTER_LINEAR;
	int sz = std::max(img.cols, img.rows);
	Mat imgFlipped, imgRotated, imgScaled;

	//flip horizontally
	if(jitter_flipping == 1){ cv::flip(img, imgFlipped, 1); }
	else{ imgFlipped = img; }

	//rotate
	if(jitter_rotAngle != 0)
	{
		Point2f centerPt(sz/2, sz/2); //the image is assumed to have size patchSz * 2
		Mat rotMat = getRotationMatrix2D(centerPt, jitter_rotAngle, 1.0);
		interpolationMethod = noInterpolation ? INTER_NEAREST : INTER_LINEAR;
		warpAffine(imgFlipped, imgRotated, rotMat, Size(sz, sz), interpolationMethod);
	}
	else{ imgRotated = imgFlipped; }

	//rescale
	if(jitter_scale_fac != 1.0)
	{
		int newSize = sz * jitter_scale_fac + 0.5;
		if(jitter_scale_fac > 1.0) //upscaling
		{
			interpolationMethod = noInterpolation ? INTER_NEAREST : INTER_CUBIC;
			cv::resize(imgRotated, imgScaled, Size(newSize, newSize), 0, 0, interpolationMethod);
		}
		else //downscaling
		{
			interpolationMethod = noInterpolation ? INTER_NEAREST : INTER_AREA;
			cv::resize(imgRotated, imgScaled, Size(newSize, newSize), 0, 0, interpolationMethod);;
		}
	}
	else{ imgScaled = imgRotated; }

	return imgScaled;
}

template<typename Dtype>
int NetRGBDNIR<Dtype>::getNextRandomPixel()
{
	int rpx = randomPixels.at(imgCnt).at( randomPixelIndices.at(imgCnt) );
	randomPixelIndices.at(imgCnt) = (randomPixelIndices.at(imgCnt) + 1) % randomPixels.at(imgCnt).size();
//    LOG(INFO) << imgCnt << " : index: " <<  randomPixelIndices.at(imgCnt) << " of total: " << randomPixels.at(imgCnt).size() << " random px: " << rpx;
	return rpx;
}


INSTANTIATE_CLASS(NetRGBDNIR);

} //namespace caffe
