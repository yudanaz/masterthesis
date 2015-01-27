#ifndef MYFREENECTOPENCVDEVICE_H
#define MYFREENECTOPENCVDEVICE_H

#include "libfreenect.hpp"
#include <iostream>
#include <vector>
#include <cmath>
#include <pthread.h>
#include <opencv/cv.h>
#include <opencv/cxcore.h>
#include <opencv/highgui.h>


using namespace cv;
using namespace std;

class myMutex {
	public:
		myMutex() {
			pthread_mutex_init( &m_mutex, NULL );
		}
		void lock() {
			pthread_mutex_lock( &m_mutex );
		}
		void unlock() {
			pthread_mutex_unlock( &m_mutex );
		}
	private:
		pthread_mutex_t m_mutex;
};

class MyFreenectOpenCVDevice : public Freenect::FreenectDevice
{
public:
		MyFreenectOpenCVDevice(freenect_context *_ctx, int _index);


		// Do not call directly even in child
		void VideoCallback(void* _rgb, uint32_t timestamp);

		// Do not call directly even in child
		void DepthCallback(void* _depth, uint32_t timestamp);

		bool getVideo(Mat& output);

		bool getDepth(Mat& output);

		bool getIR(Mat& output);

	private:
		std::vector<uint8_t> m_buffer_depth;
		std::vector<uint8_t> m_buffer_rgb;
		std::vector<uint16_t> m_gamma;
		uint16_t* depth;
		Mat depthMat;
		Mat rgbMat;
		Mat irMat;
		Mat ownMat;
		myMutex m_rgb_mutex;
		myMutex m_depth_mutex;
		bool m_new_rgb_frame;
		bool m_new_depth_frame;
		bool m_new_ir_frame;
};

#endif // MYFREENECTOPENCVDEVICE_H
