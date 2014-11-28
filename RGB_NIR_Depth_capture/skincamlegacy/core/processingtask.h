#ifndef PROCESSINGTASK_H
#define PROCESSINGTASK_H

#include <QObject>

#include <opencv2/opencv.hpp>

#include "../core/multispectralimage.h"

using namespace cv;

	/*!
	 \brief this class defines the structure of a generic processing task for
	 the SkinCam. It is derived from QObject and provides constructors requiring
	 a source image in either Mat (openCV) or MultispectralImage format, as well
	 as getResult() functions for both image formats or plain integer values.
	 It should be moved to a thread and the slots and signals should be connected
	 appropriately.
	*/
class ProcessingTask : public QObject
{
	Q_OBJECT

public:
	/*!
	 \brief a simple constructor.
	*/
	ProcessingTask();

signals:
	/*!
	 \brief signal emitted when the processing task is finished, handing over
	 the result as an openCV image matrix.
	*/
	void finishedProcessing(Mat matResult);

	/*!
	 \brief signal emitted when the processing task is finished, handing over
	 the result as a multispectral image.
	*/
	void finishedProcessing(MultispectralImage msResult);

	/*!
	 \brief signal emitted when the processing task is finished, handing over
	 the result as an integer value.
	*/
	void finishedProcessing(quint16 intResult);

	/*!
	 \brief signal emitted when an error occured during the processing task.
	*/
	void errorProcessing();

public slots:
	/*!
	 \brief this slot will start processing on a given openCV matrix image.

	 \param matSource is the image to be processed.
	*/
	virtual void process(Mat matSource) = 0;

	/*!
	 \brief this slot will start processing on a given multispectral image.

	 \param msSource is the image to be processed.
	*/
	virtual void process(MultispectralImage msSource) = 0;

};

#endif // PROCESSINGTASK_H
