#ifndef TASKDETECTSKINBYQUOTIENT_H
#define TASKDETECTSKINBYQUOTIENT_H

#include <QReadWriteLock>

#include "../core/processingtask.h"
#include "../core/skincamconfig.h"

/*!
 \brief This class represents the task of detecting skin in a multispectral
 image by calculating quotients between all wavebands for each pixel and doing
 a simple thresholding-based classification. The input can be either a 3-channel
 (false color) Mat image or a MultispectralImage with at least 2 channels. In
 addition, it requires a filter definition specifying the channels to use and
 upper and lower thresholds for their quotient. The result is returned via
 signal as a new image representing a binary mask to remove non-skin regions.
*/
class TaskDetectSkinByQuotient : public ProcessingTask
{
	Q_OBJECT

public:
	/*!
	 \brief a simple constructor.
	*/
	TaskDetectSkinByQuotient();

	/*!
	 \brief a constructor allowing to specify a quotient filter which will be
	 used for classification.

	 \param filter is the filter which will be used for classification.
	*/
	TaskDetectSkinByQuotient(struct quotientFilter filter);

	/*!
	 \brief this method sets a quotient filter which will be used for
	 classification.

	 \param filter is the filter which will be used for classification.
	*/
	void setFilter(struct quotientFilter filter);

public slots:
	/*!
	 \brief this slot will start skin detection with the current quotient filter
	 on a given openCV matrix image.

	 \param matSource is the image to be processed.
	*/
	void process(Mat matSource);

	/*!
	 \brief this slot will start processing on a given multispectral image.

	 \param msSource is the image to be processed.
	*/
	void process(MultispectralImage msSource);

private:
	/*!
	 \brief this method performs the filter operation (thresholding) based on
	 the current filter using two openCV matrix images in float format.

	 \param image1F is the first channel image as a float matrix.
	 \param image2F is the second channel image as a float matrix.
	 \return Mat is the resulting binary mask image representing the
	 thresholding result.
	*/
	Mat applyFilter(Mat image1F, Mat image2F);

	struct quotientFilter myFilter;	/*!< holds the quotient filter settings for this instance. */
	QReadWriteLock lockFilter;		/*!< thread-safe lock for the filter settings. */
};

#endif // TASKDETECTSKINBYQUOTIENT_H
