#ifndef VIDEOFILEREADER_H
#define VIDEOFILEREADER_H

#include <QTime>
#include <QCoreApplication>

#include "../io/filereader.h"
#include "../core/multispectralimage.h"

using namespace std;

/*!
 \brief an implementation of FileReader, allowing to read frames from
 video (AVI) files sequentially.
*/
class VideoFileReader : public FileReader
{
public:
	/*!
	 \brief standard constructor, initializing class variables.
	*/
	VideoFileReader();

	/*!
	 \brief a constructor allowing to specify calibration settings for the
	 image source / file reader.

	 \param settings are the calibration settings to be used.
	*/
	VideoFileReader(struct calibrationSettings settings);

	/*!
	 \brief standard destructor.
	*/
	~VideoFileReader();

	/*!
	 \brief opens the image source, which is a video file in this case, and
	 reads framerate and total frame count. Also starts the "fps keeper",
	 which is a timer making sure that images are not read faster than they
	 should be.

	 \param source is a string representing the filename.
	*/
	virtual void openSource (QString source);

	/*!
	 \brief acquires a new frame from the video file and decodes the contained
	 multispectral channels. Finally, it waits until the "fps keeper" timer
	 runs out to maintain the correct framerate.
	*/
	virtual MultispectralImage acquireNewFrame();

	/*!
	 \brief returns the total number of frames in the video file.

	 \return double the number of frames.
	*/
	double getFrameCount();

	/*!
	 \brief changes the position in the video file to the given frame number.

	 \param frame is the number of the frame which will be acquired next.
	*/
	void setPosition(double frame);

private:
	bool isImageSequence;		/*!< marks if the source is a video file (false)
									 or an image sequence (true) */

	VideoCapture mySourceFile;	/*!< the video file handler. */

	QString mySourcePath;		/*!< path to the image sequence. */
	quint32 myPosition;		/*!< holds the current position within the sequence. */

	double myFrameCount;		/*!< the number of frames in the video file. */
};

#endif // VIDEOFILEREADER_H
