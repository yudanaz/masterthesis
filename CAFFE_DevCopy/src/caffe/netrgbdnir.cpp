#include "../../include/caffe/netrgbdnir.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include "../../include/caffe/data_layers.hpp"
#include "../../include/caffe/util/io.hpp"

using namespace cv;

namespace caffe {

template<typename Dtype>
void NetRGBDNIR<Dtype>::setup(std::string imgsListURL, int patchsize, int batchSize, int batchesPerImage,
							  bool RGB, bool NIR, bool depth, bool isMultiscale, std::string imageType, std::string labelImageSuffix)
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

		vector<int> randomImgPixels;
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
	multiscale = isMultiscale;
	imgType = imageType;
	labelImgSuffix = labelImageSuffix;
	scaleCnt = 0;
	if(multiscale)
	{
		batchesPerImg = batchesPerImage*3; //because of 3 scales
		scaleCntMax = 3;
	}
	else
	{
		batchesPerImg = batchesPerImage;
		scaleCntMax = 1;
	}
	currentLabel = 0; x = 0; y = 0;
	jitter_flipping = false;
	jitter_rotAngle = 0;
	jitter_scale_fac = 0;

	//read first image
	readNextImage();

	//define random patches that should be read from this image
//    setRandomPatches();
//    setUniformPatches();

	//debug
	iteration = 0;
}

template<typename Dtype>
void NetRGBDNIR<Dtype>::feedNextPatchesToInputLayers()
{
//    LOG(INFO) << "Feeding new patches to memory data layers";
	std::vector<Mat> mats_rgb_Y;
	std::vector<Mat> mats_rgb_UV;

	std::vector<Mat> mats_nir_Y;
	std::vector<Mat> mats_nir_UV;

	std::vector<Mat> mats_depth;
	std::vector<int> labels;

	int batchCnt = 0;
	for(batchCnt = 0; batchCnt < batchSz; )
	{
		//LOG(INFO) << "Starting nr " << batchCnt << " / " << batchSz << " in current batch";
		//if all patches in current image have been read, load next image
//        if(patchCnt >= batchesPerImg)//patchMax)
//        {
////            LOG(INFO) << "Next image";
//            readNextImage();
////            setRandomPatches();
//            setUniformPatches();
//            patchCnt = 0;
//        }

		//load next image if enough batches have been read from the current image
		if(batchNr == batchesPerImg)
		{
			readNextImage();
			batchNr = 0;
		}
		else{ batchNr++; }

		//get the correct pixel indices
		int w = img_labels.cols;

		//get the next random pixel, but only once for each current patch, use same label for all scales of one patch position
		//if the label is "unknown", go to next pixel
//		int currentLabel, x, y;
		if(scaleCnt == 0)
		{
			do
			{
				int randomPixel = getNextRandomPixel();
				x = randomPixel % w;
				y = randomPixel / w;

				//get label for this patch
				currentLabel = img_labels.at<uchar>(y,x);
	//            LOG(INFO) << "label: " << currentLabel;
			}
			while(currentLabel == 255);
		}

		labels.push_back(currentLabel);

		//jump the "unknown" label, training with this label is useless
		batchCnt++;

		///////////////////////////////////////////////////////////////////////
		/// READ PATCHES
		///////////////////////////////////////////////////////////////////////

		//set random variables for jitter (scaling, rotation and flip)
		setJitterRandomVars();

		//LOG(INFO) << "Get Patch for Scale 0";
		//read the current patch from the image (orig. scale 0)
		if(hasRGB)
		{
			if(scaleCnt == 0) //get patch for scale 0 on this pixel position
			{
	//            LOG(INFO) << "has RGB \n";
				Mat patch_rgb0 = getImgPatch(img_rgb0, x, y);
				std::vector<Mat> patch_rgb0_vect;
				split(patch_rgb0, patch_rgb0_vect);
				Mat rgb0y = patch_rgb0_vect[0];
				Mat rgb0uv;
				std::vector<Mat> rgb0uv_vec;
				rgb0uv_vec.push_back(patch_rgb0_vect[1]);
				rgb0uv_vec.push_back(patch_rgb0_vect[2]);
				merge(rgb0uv_vec, rgb0uv);
				mats_rgb_Y.push_back(rgb0y);
				mats_rgb_UV.push_back(rgb0uv);
	//            imshow("rgb0y", rgb0y); cvWaitKey();
	//            imwrite("/home/maurice/rgb0y.png", rgb0y);
			}
			else if(scaleCnt == 1) //get patch for scale 1 on this pixel position
			{
//                LOG(INFO) << "is multiscale\n";
				//level 1
				Mat patch_rgb1 = getImgPatch(img_rgb1, x/2, y/2);
				std::vector<Mat> patch_rgb1_vect;
				split(patch_rgb1, patch_rgb1_vect);
				Mat rgb1y = patch_rgb1_vect[0];
				Mat rgb1uv;
				std::vector<Mat> rgb1uv_vec;
				rgb1uv_vec.push_back(patch_rgb1_vect[1]);
				rgb1uv_vec.push_back(patch_rgb1_vect[2]);
				merge(rgb1uv_vec, rgb1uv);
				mats_rgb_Y.push_back(rgb1y);
				mats_rgb_UV.push_back(rgb1uv);
//                imshow("rgb1y", rgb1y); cvWaitKey();
//                imwrite("/home/maurice/rgb1y.png", rgb1y);
			}
			else //get patch for scale 2 on this pixel position
			{
				//level 2
				Mat patch_rgb2 = getImgPatch(img_rgb2, x/4, y/4);
				std::vector<Mat> patch_rgb2_vect;
				split(patch_rgb2, patch_rgb2_vect);
				Mat rgb2y = patch_rgb2_vect[0];
				Mat rgb2uv;
				std::vector<Mat> rgb2uv_vec;
				rgb2uv_vec.push_back(patch_rgb2_vect[1]);
				rgb2uv_vec.push_back(patch_rgb2_vect[2]);
				merge(rgb2uv_vec, rgb2uv);
				mats_rgb_Y.push_back(rgb2y);
				mats_rgb_UV.push_back(rgb2uv);
//                imshow("rgb2y", rgb2y); cvWaitKey();
//                imwrite("/home/maurice/rgb2y.png", rgb2y);
			}
		}

		if(hasNIR)
		{
			if(scaleCnt == 0) //get patch for scale 0 on this pixel position
			{
	//            LOG(INFO) << "has NIR \n";
				Mat patch_nir0 = getImgPatch(img_nir0, x, y);
				std::vector<Mat> patch_nir0_vect;
				split(patch_nir0, patch_nir0_vect);
				Mat nir0y = patch_nir0_vect[0];
				Mat nir0uv;
				std::vector<Mat> nir0uv_vec;
				nir0uv_vec.push_back(patch_nir0_vect[1]);
				nir0uv_vec.push_back(patch_nir0_vect[2]);
				merge(nir0uv_vec, nir0uv);
				mats_nir_Y.push_back(nir0y);
				mats_nir_UV.push_back(nir0uv);
	//            imshow("nir0y", nir0y); cvWaitKey();
	//            imwrite("/home/maurice/nir0y.png", nir0y);
			}
			else if(scaleCnt == 1) //get patch for scale 1 on this pixel position
			{
//                LOG(INFO) << "is multiscale\n";
				//level 1
				Mat patch_nir1 = getImgPatch(img_nir1, x/2, y/2);
				std::vector<Mat> patch_nir1_vect;
				split(patch_nir1, patch_nir1_vect);
				Mat nir1y = patch_nir1_vect[0];
				Mat nir1uv;
				std::vector<Mat> nir1uv_vec;
				nir1uv_vec.push_back(patch_nir1_vect[1]);
				nir1uv_vec.push_back(patch_nir1_vect[2]);
				merge(nir1uv_vec, nir1uv);
				mats_nir_Y.push_back(nir1y);
				mats_nir_UV.push_back(nir1uv);
//                imshow("nir1y", nir1y); cvWaitKey();
//                imwrite("/home/maurice/nir1y.png", nir1y);
			}
			else //get patch for scale 2 on this pixel position
			{
				//level 2
				Mat patch_nir2 = getImgPatch(img_nir2, x/4, y/4);
				std::vector<Mat> patch_nir2_vect;
				split(patch_nir2, patch_nir2_vect);
				Mat nir2y = patch_nir2_vect[0];
				Mat nir2uv;
				std::vector<Mat> nir2uv_vec;
				nir2uv_vec.push_back(patch_nir2_vect[1]);
				nir2uv_vec.push_back(patch_nir2_vect[2]);
				merge(nir2uv_vec, nir2uv);
				mats_nir_Y.push_back(nir2y);
				mats_nir_UV.push_back(nir2uv);
//                imshow("nir2y", nir2y); cvWaitKey();
//                imwrite("/home/maurice/nir2y.png", nir2y);
			}
		}

		if(hasDepth)
		{
//            LOG(INFO) << "has Depth \n";
			if(scaleCnt == 0) //get patch for scale 0 on this pixel position
			{
				mats_depth.push_back(getImgPatch(img_depth0, x, y, true));
			}
			else if(scaleCnt == 1) //get patch for scale 1 on this pixel position
			{
				mats_depth.push_back(getImgPatch(img_depth1, x/2, y/2, true));
			}
			else //get patch for scale 2 on this pixel position
			{
				mats_depth.push_back(getImgPatch(img_depth2, x/4, y/4, true));
			}
		}

		patchCnt++;
		scaleCnt = (scaleCnt + 1) % scaleCntMax;


		///DEBUG///
		imshow("current nir patch", mats_nir_Y.at(0)); cvWaitKey();
		///DEBUG///
	}

	//LOG(INFO) << "Feed Datum vectors to Memory Data Layers";
	//feed images to corresponding memory_data_layers. We have different input layers for
	//RGB, NIR and Depth, so convolution won't mix those channels, plus 3 different scales
	//I.e. in total there are 3 * 3 = 9 input layers
	for(int layer_id = 0; layer_id < this->layers_.size(); ++layer_id)
	{
		Layer<Dtype>* layer = this->layers_[layer_id].get(); //get stored pointer from shared pointer

		//if memory data layer, add correct datum vector to layer
		if( strcmp( layer->type(), "MemoryData") == 0 )
		{
			std::string nm = layer->layer_param().name();

			//NIR//////////////////////////////////////////////////////////////
			if(nm == "nir_Y"){ if(hasNIR)
				{ ((MemoryDataLayer<Dtype>*)layer)->AddMatVector(mats_nir_Y, labels); } }
			else if(nm == "nir_UV"){ if(hasNIR)
				{ ((MemoryDataLayer<Dtype>*)layer)->AddMatVector(mats_nir_UV, labels); } }

			//DEPTH////////////////////////////////////////////////////////////
			else if(nm == "depth"){ if(hasDepth)
				{((MemoryDataLayer<Dtype>*)layer)->AddMatVector(mats_depth, labels); } }

			//RGB//////////////////////////////////////////////////////////////
			else if(nm == "rgb_Y"){ if(hasRGB)
				{ ((MemoryDataLayer<Dtype>*)layer)->AddMatVector(mats_rgb_Y, labels); } }
			else if(nm == "rgb_UV"){ if(hasRGB)
				{ ((MemoryDataLayer<Dtype>*)layer)->AddMatVector(mats_rgb_UV, labels); } }

		}
	}

	//debug
	iteration++;
}


template<typename Dtype>
void NetRGBDNIR<Dtype>::readNextImage()
{
	//increment image counter
	imgCnt  = (imgCnt + 1) % imgMax;

	//get next image URL, also circle through images (if iterations are > all available patches)
	std::string imageURL = imgs[imgCnt];
//    LOG(INFO) << "Read next image: " << imageURL;

	//load all image types (RGB, NIR and Depth) if available, create scales (image pyramid) pad images (make borders)
	std::string labelsNm = imageURL + labelImgSuffix + std::string(".png"); //labels lossless, always png
//    std::string rgbNm = imageURL + std::string(".") + imgType; //for stanford
	std::string rgbNm = imageURL + std::string("_rgb.") + imgType;
	std::string nirNm = imageURL + std::string("_nir.") + imgType;
	std::string depthNm = imageURL + std::string("_depth.png"); //depth lossless, always png

//	LOG(INFO) << "Reading image " << imageURL;
	img_labels = cv::imread(labelsNm, cv::IMREAD_GRAYSCALE); //label img isn't downsampled nor padded

	//make a shuffled list of pixel indices for each image
	if(randomPixels.at(imgCnt).size() == 0) //vector for current image not initialized yet
	{
		//fill vector holding all image pixels for current image, then shuffle it
		int totalNrOfPixels = img_labels.cols * img_labels.rows;
		srand(time(0)); //set seed for random generator using current time
		for (int i = 0; i < totalNrOfPixels; ++i)
		{
			randomPixels.at(imgCnt).push_back(i);
		}
		std::random_shuffle(randomPixels.at(imgCnt).begin(), randomPixels.at(imgCnt).end());
	}

	cv::Mat temp1;//, temp2, temp3;

	//Make the downsized images for the image pyramid and add the padding according to patch size
//    LOG(INFO) << "Making scales for RGB";
	if(hasRGB)
	{
		//read image and convert ot YUV color space
		temp1 = cv::imread(rgbNm, cv::IMREAD_COLOR);
		cvtColor(temp1, temp1, CV_BGR2YCrCb);

		if(!multiscale)
		{
			preproc.normalizeEachChannelLocally(temp1, 15);
			cv::copyMakeBorder(temp1, img_rgb0, borderSz, borderSz-1, borderSz, borderSz-1, cv::BORDER_CONSTANT, cv::Scalar(0)); //scale 0
		}
		else //make Laplacian pyramid
		{
			std::vector<Mat> pyramid = preproc.makePyramid(temp1, 3);

			//compute zero mean and unit variance for each channel
			for(int i = 0; i < 3; ++i)
			{
				Mat pyrLevel = pyramid[i];
				preproc.normalizeEachChannelLocally(pyrLevel, 15);
			}

			//make border padding around image for each pyramid level, border is whole patchsize to allow
			//for artificial jitter (rotation & scale), then the patches are cutout after applying jitter
			cv::copyMakeBorder(pyramid[0], img_rgb0, patchSz, patchSz-1, patchSz, patchSz-1, cv::BORDER_CONSTANT, cv::Scalar(0)); //scale 0
			cv::copyMakeBorder(pyramid[1], img_rgb1, patchSz, patchSz-1, patchSz, patchSz-1, cv::BORDER_CONSTANT, cv::Scalar(0)); //scale 1 (half the size)
			cv::copyMakeBorder(pyramid[2], img_rgb2, patchSz, patchSz-1, patchSz, patchSz-1, cv::BORDER_CONSTANT, cv::Scalar(0)); //scale 2 (1/4 the size)
		}
	}

//    LOG(INFO) << "Making scales for NIR";
	if(hasNIR)
	{
		temp1 = cv::imread(nirNm, cv::IMREAD_COLOR);
		cvtColor(temp1, temp1, CV_BGR2YCrCb);

		if(!multiscale)
		{
			preproc.normalizeEachChannelLocally(temp1, 15);
			cv::copyMakeBorder(temp1, img_nir0, borderSz, borderSz-1, borderSz, borderSz-1, cv::BORDER_CONSTANT, cv::Scalar(0)); //scale 0
		}
		else //make Laplacian pyramid
		{
			std::vector<Mat> pyramid = preproc.makePyramid(temp1, 3);

			//compute zero mean and unit variance for each channel
			for(int i = 0; i < 3; ++i)
			{
				Mat pyrLevel = pyramid[i];
				preproc.normalizeEachChannelLocally(pyrLevel, 15);
			}

			//make border padding around image for each pyramid level, border is whole patchsize to allow
			//for artificial jitter (rotation & scale), then the patches are cutout after applying jitter
			cv::copyMakeBorder(pyramid[0], img_nir0, patchSz, patchSz-1, patchSz, patchSz-1, cv::BORDER_CONSTANT, cv::Scalar(0)); //scale 0
			cv::copyMakeBorder(pyramid[1], img_nir1, patchSz, patchSz-1, patchSz, patchSz-1, cv::BORDER_CONSTANT, cv::Scalar(0)); //scale 1 (half the size)
			cv::copyMakeBorder(pyramid[2], img_nir2, patchSz, patchSz-1, patchSz, patchSz-1, cv::BORDER_CONSTANT, cv::Scalar(0)); //scale 2 (1/4 the size)
		}
	}

//    LOG(INFO) << "Making scales for Depth";
	if(hasDepth)
	{
		temp1 = cv::imread(depthNm, cv::IMREAD_GRAYSCALE);

		if(!multiscale)
		{
//            preproc.normalizeEachChannelLocally(temp1, 15);
			cv::copyMakeBorder(temp1, img_depth0, borderSz, borderSz-1, borderSz, borderSz-1, cv::BORDER_CONSTANT, cv::Scalar(0)); //scale 0
		}
		else //make Laplacian pyramid
		{
			std::vector<Mat> pyramid = preproc.makePyramid(temp1, 3, INTER_NEAREST); //for depth, no interpolation when resizing

			//make border padding around image for each pyramid level, border is whole patchsize to allow
			//for artificial jitter (rotation & scale), then the patches are cutout after applying jitter
			cv::copyMakeBorder(pyramid[0], img_depth0, patchSz, patchSz-1, patchSz, patchSz-1, cv::BORDER_CONSTANT, cv::Scalar(0)); //scale 0
			cv::copyMakeBorder(pyramid[1], img_depth1, patchSz, patchSz-1, patchSz, patchSz-1, cv::BORDER_CONSTANT, cv::Scalar(0)); //scale 1 (half the size)
			cv::copyMakeBorder(pyramid[2], img_depth2, patchSz, patchSz-1, patchSz, patchSz-1, cv::BORDER_CONSTANT, cv::Scalar(0)); //scale 2 (1/4 the size)
		}
	}
//    LOG(INFO) << "done";
}



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
//	Mat patchrgb;
//	cvtColor(patchJitter, patchrgb, CV_YCrCb2BGR);
//	imshow("patch with jitter", patchrgb); cvWaitKey();
//    imwrite("/home/maurice/"+s+"patch_jitter.png", patchJitter);

	int offset = borderSz - (patchSz2x - patchJitter.cols) / 2; //cols and rows should be equal, it's a square after all
	cv::Rect roi2(offset, offset, patchSz, patchSz);
	patchJitter(roi2).copyTo(patch);
//	cvtColor(patch, patchrgb, CV_YCrCb2BGR);
//	imshow("patch with jitter resized", patchrgb); cvWaitKey();
//    imwrite("/home/maurice/"+s+"patch_jitter_resized.png", patch);
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
//    LOG(INFO) << imgCnt << " : " <<  randomPixelIndices.at(imgCnt);
	return rpx;
}


INSTANTIATE_CLASS(NetRGBDNIR);

} //namespace caffe
