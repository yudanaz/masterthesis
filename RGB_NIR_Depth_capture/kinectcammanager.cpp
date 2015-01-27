#include "kinectcammanager.h"
#include<QDebug>
#include<QMessageBox>
#include<unistd.h>

KinectCamManager::KinectCamManager() :
	connected(false)
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
	Mat irMat(Size(640,480),CV_8UC1, Scalar(0));

	try
	{
		//try getting depth and rgb images until successful

		bool success;

		//		success =  freenectDevice->getDepth(depthMat);
		//		while(!success)
		//		{
		//			success = freenectDevice->getDepth(depthMat);
		//		}
		//		depthMat.convertTo(depthMap8bit, CV_8UC1, 255.0/2048.0);
		//		camImgs[Kinect_Depth] = depthMap8bit;


		freenectDevice->setVideoFormat(FREENECT_VIDEO_IR_8BIT);
		success = freenectDevice->getIR(irMat);
		while(!success)
		{
			success = freenectDevice->getIR(irMat);
		}
		double min, max;
		minMaxLoc(irMat, &min, &max);
		irMat.convertTo(depthMap8bit, CV_8UC1, 255.0/max);
		camImgs[Kinect_Depth] = depthMap8bit;



		freenectDevice->setVideoFormat(FREENECT_VIDEO_RGB);
		success = freenectDevice->getVideo(rgbMat);
		while(!success)
		{
			success = freenectDevice->getVideo(rgbMat);
		}
		camImgs[Kinect_RGB] = rgbMat;
	}
	catch(std::runtime_error e)
	{
		QMessageBox::information(NULL, "Kinect Error", QString::fromStdString(e.what()), QMessageBox::Ok);
	}

	//sleep 5 milliseconds in order not to overpower kinect
	usleep(5000);//33333);
}

bool KinectCamManager::isConnected()
{
	return connected;
}
