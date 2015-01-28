#include "kinectcammanager.h"
#include<QDebug>
#include<QMessageBox>
#include<unistd.h>

KinectCamManager::KinectCamManager() :
	connected(false),
	captureOneIrFrame(false)
{
	try
	{
		freenectDevice = &(freenect.createDevice<MyFreenectOpenCVDevice>(0));
		connected = true;
	}
	catch(std::runtime_error e)
	{
		QMessageBox::information(NULL, "Kinect Error", QString::fromStdString(e.what()), QMessageBox::Ok);
		connected = false;
	}
}

void KinectCamManager::connectCameras()
{
	if(!connected){ return; }
	try
	{
		freenectDevice->startDepth();
		freenectDevice->startVideo();
	}
	catch(std::runtime_error e)
	{
		QMessageBox::information(NULL, "Kinect Error", QString::fromStdString(e.what()), QMessageBox::Ok);
	}
}

void KinectCamManager::closeCameras()
{
	if(!connected){ return; }
	try
	{
		freenectDevice->stopDepth();
		freenectDevice->stopVideo();
	}
	catch(std::runtime_error e)
	{
		QMessageBox::information(NULL, "Kinect Error", QString::fromStdString(e.what()), QMessageBox::Ok);
	}
}

void KinectCamManager::getImages(QMap<RGBDNIR_captureType, Mat> &camImgs)
{
	if(!connected){ return; }

	//add Kinect Images
	Mat depthMat(Size(640,480),CV_16UC1);
	Mat depthMap8bit (Size(640,480),CV_8UC1);
	Mat rgbMat(Size(640,480),CV_8UC3,Scalar(0));

	try
	{
		bool success;

		//try getting depth image until successful
		success =  freenectDevice->getDepth(depthMat);
		while(!success)
		{
			success = freenectDevice->getDepth(depthMat);
		}
		depthMat.convertTo(depthMap8bit, CV_8UC1, 255.0/2048.0);
		camImgs[Kinect_Depth] = depthMap8bit;

		//try getting rgb image until successful
		success = freenectDevice->getVideo(rgbMat);
		while(!success)
		{
			success = freenectDevice->getVideo(rgbMat);
		}
		camImgs[Kinect_RGB] = rgbMat;

		//if set, try getting one ir image (only once, because slow -> stream has to be stopped & restarted)
		if(captureOneIrFrame)
		{
			freenectDevice->toggleIRMode(true);

			Mat irMat(Size(640,480),CV_8UC1, Scalar(0));
			Mat irMap8bit (Size(640,480),CV_8UC1);

			success = freenectDevice->getIR(irMat);
			while(!success)
			{
				success = freenectDevice->getIR(irMat);
			}
//			double min, max;
			//normalize to [0,255]
//			minMaxLoc(irMat, &min, &max);
			irMat.convertTo(irMap8bit, CV_8UC1, 255.0/25.0);//max);
			camImgs[Kinect_IR] = irMap8bit;

			freenectDevice->toggleIRMode(false);
			captureOneIrFrame = false;
		}
	}
	catch(std::runtime_error e)
	{
		QMessageBox::information(NULL, "Kinect Error", QString::fromStdString(e.what()), QMessageBox::Ok);
	}

	//sleep 5 milliseconds in order not to overpower kinect
	usleep(5000);//33333);
}

void KinectCamManager::triggerIRcapture()
{
	captureOneIrFrame = true;
}

bool KinectCamManager::isConnected()
{
	return connected;
}
