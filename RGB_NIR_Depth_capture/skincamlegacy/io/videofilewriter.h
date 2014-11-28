#ifndef VIDEOFILEWRITER_H
#define VIDEOFILEWRITER_H

#include <opencv2/opencv.hpp>

#include "../io/filewriter.h"

/*!
 \brief a class implementing the FileWriter interface, allowing to
 write a continuous stream of images to a video file.

 This class implements the virtual methods writeMatImage() and
 writeMultispectral(). On the first call of any of these methods, an openCV
 VideoWriter is created and configured appropriately. It creates a new video
 file with a timestamp added to the defined filename.
*/
class VideoFileWriter : public FileWriter
{
public:
	/*!
	 \brief a standard constructor. Will set a default filename for the target,
	 but requires the framerate for the video.

	 \param fps is the framerate for the video file.
	*/
	VideoFileWriter(quint8 fps);

	/*!
	 \brief a constructor allowing to specify the target file. Will throw an
	 exception if the path is not valid.

	 \param file denotes the full path to the target file.
	 \param fps is the framerate for the video file.
	*/
	VideoFileWriter(QString file, quint8 fps);

	/*!
	 \brief a constructor allowing to specify the target file and a list of the
	 images wavebands. The peak wavelengths of all channels will be converted
	 into text format and added to the filename of the target file.

	 Format: "(<channel_1>,<channel_2>,...,<channel_n>)", e.g. "(1060,1300,1550)"

	 \param file denotes the full path to the target file.
	 \param wavebands is an array containing the peak wavelengths of all channels.
	 \param fps is the framerate for the video file.
	*/
	VideoFileWriter(QString file, QList<qint16> wavebands, quint8 fps);

	/*!
	 \brief destructor
	*/
	virtual ~VideoFileWriter();

	/*!
	 \brief writes a new IplImage frame to the video file.

	 \param frame is the image to be written.
	*/
	void writeMatImage (Mat frame);

	/*!
	 \brief write a new MultispectralImage frame to the video file by creating
	 a side by side representation of all channels (except the dark channel).

	 \param frame is the image to be written.
	*/
	void writeMultispectral (MultispectralImage frame);

private:
	/*!
	 \brief This method creates the target video file and opens it for writing.
	 The resolution (image size) has to be given as well as a flag indicating
	 whether the images are in color (true) or grayscale (false). It returns
	 true on success.

	 \param size is the resolution (image size).
	 \param isColor flag indicating whether the images are in color (true) or grayscale (false).
	 \return bool is true if the file was openend correctly.
	*/
	bool open(Size size, bool isColor = true);

	VideoWriter myVideoWriter;	/*!< openCV VideoWriter object */

	quint8 myFPS;				/*!< specifies the framerate of the output video. */
};

#endif // VIDEOFILEWRITER_H
