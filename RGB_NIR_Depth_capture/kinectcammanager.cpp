#include "kinectcammanager.h"
#include<QDebug>
#include<unistd.h>

KinectCamManager::KinectCamManager() :
	freenectDevice(freenect.createDevice<MyFreenectOpenCVDevice>(0))
{
}

void KinectCamManager::connectCameras()
{
	freenectDevice.startDepth();
	freenectDevice.startVideo();
}

void KinectCamManager::closeCameras()
{
	freenectDevice.stopDepth();
	freenectDevice.stopVideo();
}

void KinectCamManager::getImages(QMap<RGBDNIR_captureType, Mat> &camImgs)
{
	bool success;

	//add Kinect Images
	Mat depthMat(Size(640,480),CV_16UC1);
	Mat depthMap8bit (Size(640,480),CV_8UC1);
	Mat rgbMat(Size(640,480),CV_8UC3,Scalar(0));

	success = freenectDevice.getDepth(depthMat);
	if(success)
	{
		depthMat.convertTo(depthMap8bit, CV_8UC1, 255.0/2048.0);
		camImgs[Kinect_Depth] = depthMap8bit;
	}

	success = freenectDevice.getVideo(rgbMat);
	if(success)
	{
		//cvReleaseImage( camImgs[Kinect_RGB].data ); //free memory
		camImgs[Kinect_RGB] = rgbMat;
	}

	//sleep 5 milliseconds in order not to overpower kinect
	usleep(5000);//33333);
}
