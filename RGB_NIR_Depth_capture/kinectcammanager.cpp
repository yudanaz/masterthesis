#include "kinectcammanager.h"
#include<QDebug>
#include<QMessageBox>
#include<unistd.h>

KinectCamManager::KinectCamManager() :
	connected(false),
	captureOneIrFrame(false),
	queue_switchRGB2IR(false),
	queue_switchIR2RGB(false),
	capturingRGB(true)
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
	try
	{
		//if set, switch to capture IR instead of RGB frame
		if(queue_switchRGB2IR)
		{
			queue_switchRGB2IR = false;
			capturingRGB = false;
			freenectDevice->toggleIRMode(true);
		}
		else if(queue_switchIR2RGB)
		{
			queue_switchIR2RGB = false;
			capturingRGB = true;
			freenectDevice->toggleIRMode(false);
		}

		//if set, try getting one ir image (only once, because slow -> stream has to be stopped & restarted)
		if(captureOneIrFrame)
		{
			capturingRGB = false;
			freenectDevice->toggleIRMode(true);

			captureOneIrFrame = false;
			queue_switchIR2RGB = true;
		}

		bool success;

		if(capturingRGB)
		{
			//try getting RGB or IR image until successful
			Mat rgbMat(Size(640,480),CV_8UC3,Scalar(0));
			success = freenectDevice->getVideo(rgbMat);
			while(!success)
			{
				success = freenectDevice->getVideo(rgbMat);
			}
			//turn by 180° (kinect is upside down in camera rig)
			flip(rgbMat, rgbMat, 0);
			flip(rgbMat, rgbMat, 1);
			camImgs[Kinect_RGB] = rgbMat;

			//try getting depth image until successful
			Mat depthMat, depthMat_8bit;
			success =  freenectDevice->getDepth(depthMat);
			while(!success)
			{
				success = freenectDevice->getDepth(depthMat);
			}
			//scale to 8 bit and turn by 180° (kinect is upside down in camera rig)
			depthMat.convertTo(depthMat_8bit, CV_8UC1, 255.0/2047.0);
			flip(depthMat_8bit, depthMat_8bit, 0);
			flip(depthMat_8bit, depthMat_8bit, 1);
			camImgs[Kinect_Depth] = depthMat_8bit;
		}
		else
		{
			Mat irMat;
			Mat irMat8bit;

			success = freenectDevice->getIR(irMat);
			while(!success)
			{
				success = freenectDevice->getIR(irMat);
			}
//			double min, max;
//			minMaxLoc(irMat, &min, &max);

			//normalize to [0,255] and turn by 180° (kinect is upside down in camera rig)
			irMat.convertTo(irMat8bit, CV_8UC1, 255.0/16.0); //25 //max
			flip(irMat8bit, irMat8bit, 0);
			flip(irMat8bit, irMat8bit, 1);
			camImgs[Kinect_IR] = irMat8bit;
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

void KinectCamManager::switch_RGB_IR(bool captureRGB)
{
	queue_switchRGB2IR = !captureRGB;
	queue_switchIR2RGB = captureRGB;
}
