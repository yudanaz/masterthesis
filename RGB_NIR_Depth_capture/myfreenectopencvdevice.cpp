#include "myfreenectopencvdevice.h"
#include "libfreenect.h"
#include "libfreenect.hpp"

MyFreenectOpenCVDevice::MyFreenectOpenCVDevice(freenect_context *_ctx, int _index):
	Freenect::FreenectDevice(_ctx, _index),
	in_IR_mode(false), modeSetOnce_IR(false), modeSetOnce_RGB(false),
	m_buffer_depth(FREENECT_DEPTH_11BIT),
	m_buffer_rgb(FREENECT_VIDEO_RGB), m_gamma(2048), m_new_rgb_frame(false),
	m_new_depth_frame(false), depthMat(Size(640,480),CV_16UC1),
	rgbMat(Size(640,480), CV_8UC3, Scalar(0)),
	irMat(Size(640,480),CV_8UC1),
	ownMat(Size(640,480),CV_8UC3,Scalar(0))
{
	for( unsigned int i = 0 ; i < 2048 ; i++) {
		float v = i/2048.0;
		v = std::pow(v, 3)* 6;
		m_gamma[i] = v*6*256;
	}
}

void MyFreenectOpenCVDevice::VideoCallback(void* _rgb, uint32_t timestamp)
{
//    std::cout << "RGB callback" << std::endl;
	m_rgb_mutex.lock();

//	freenect_video_format format = getVideoFormat();
	if(!in_IR_mode)//RGB -> 3 channel image
	{
		uint8_t* rgb = static_cast<uint8_t*>(_rgb);
		rgbMat.data = rgb;
		m_new_rgb_frame = true;
	}
	else//IR -> 1 channel image
	{
		uint8_t* ir = static_cast<uint8_t*>(_rgb);
		irMat.data = (uchar*) ir;
		m_new_ir_frame = true;
	}
	m_rgb_mutex.unlock();
}

void MyFreenectOpenCVDevice::DepthCallback(void* _depth, uint32_t timestamp)
{
//    std::cout << "Depth callback" << std::endl;
	m_depth_mutex.lock();
	uint16_t* depth = static_cast<uint16_t*>(_depth);
	depthMat.data = (uchar*) depth;
	m_new_depth_frame = true;
	m_depth_mutex.unlock();
}

bool MyFreenectOpenCVDevice::getVideo(Mat& output)
{
	m_rgb_mutex.lock();
	if(m_new_rgb_frame) {
		cv::cvtColor(rgbMat, output, CV_RGB2BGR);
		m_new_rgb_frame = false;
		m_rgb_mutex.unlock();
		return true;
	} else {
		m_rgb_mutex.unlock();
		return false;
	}
}

bool MyFreenectOpenCVDevice::getDepth(Mat& output)
{
	m_depth_mutex.lock();
	if(m_new_depth_frame) {
		depthMat.copyTo(output);
		m_new_depth_frame = false;
		m_depth_mutex.unlock();
		return true;
	} else {
		m_depth_mutex.unlock();
		return false;
	}
}


bool MyFreenectOpenCVDevice::getIR(Mat& output)
{
	m_depth_mutex.lock();
	if(m_new_ir_frame) {
		irMat.copyTo(output);
		m_new_ir_frame = false;
		m_depth_mutex.unlock();
		return true;
	} else {
		m_depth_mutex.unlock();
		return false;
	}
}


void MyFreenectOpenCVDevice::toggleIRMode(bool ir_on)
{
	//this method was supposed to make the time gaps between rgb and ir faster but that apparently doesn't work...

	if(in_IR_mode == ir_on) { return; } //nothing changed
	in_IR_mode = ir_on;

	freenect_stop_video((_freenect_device*)getDevice());
	if(in_IR_mode)
	{
		if(!modeSetOnce_IR)
		{
			mode_IR = freenect_find_video_mode(FREENECT_RESOLUTION_MEDIUM, FREENECT_VIDEO_IR_8BIT);
			modeSetOnce_IR = true;
		}
		freenect_set_video_mode((_freenect_device*)getDevice(), mode_IR);
	}
	else //in RGB mode
	{
		if(!modeSetOnce_RGB)
		{
			mode_RGB = freenect_find_video_mode(FREENECT_RESOLUTION_MEDIUM, FREENECT_VIDEO_RGB);
			modeSetOnce_RGB = true;
		}
		freenect_set_video_mode((_freenect_device*)getDevice(), mode_RGB);
	}
	freenect_start_video((_freenect_device*)getDevice());

}

