#include "hog_crossspectralstereomatcher.h"
#include <QDebug>

#define CV_SSE2 true

HOG_crossSpectralStereoMatcher::HOG_crossSpectralStereoMatcher():
	disparityOptimimizationStragey(dispOpt_WTA),
	minDisp(4), maxDisp(20),
	wtaThreshold(1.0),
	SADwindow(3), preFilterCAP(11), uniquenessRatio(1), speckleWindow(7), speckleRange(4),
	bestDescrRasterDiv(16)
{
	//fill best descriptor candidates vector with as many positions as the raster dictates
	for(int i = 0; i< bestDescrRasterDiv*bestDescrRasterDiv; ++i)
	{
		BestDescriptorCandidate candidate(0,0,0,0,9999.0);
		bestDescrCanditates.push_back(candidate);
	}
}

void HOG_crossSpectralStereoMatcher::process(Mat img_L, Mat img_R, Mat& out_disp)
{
	int orig_w = img_L.cols;
	int orig_h = img_L.rows;

	//multispectral stereo matching with HOG descriptors like in
	//"On cross-spectral stereo matching using dense gradient features" [Pinggera]
	HOGDescriptor d1;
//	int blockSize = 18;
//	int cellSize = 6;
	int nCells = blockSize/cellSize * blockSize/cellSize;
	d1.blockSize = Size(blockSize, blockSize); //param from paper
	d1.cellSize = Size(cellSize, cellSize);// n x n subcells, n = 3, nCells = 3*3 = 9, param from paper

	//define a dynamic window size used for HOG descriptors computation. Resize to lower resolution if set.
	//must be aligned with blocksize and should be close to original size
	int win_w = ((int)(orig_w / blockSize)) * blockSize;
	int win_h = ((int)(orig_h / blockSize)) * blockSize;
	d1.winSize = Size(win_w, win_h);

	d1.blockStride = Size(1, 1); // -> stride = 1 means every goddamn pixel, less blocksize-1 border, i.e.
							   // nrOfGradients = blocks * cellsPerBlock * Bins
							   // and blocks = (width - blockSize + 1) * (height - blockSize + 1) => 613 * 487 * 9 * 9 = 24181011 in this case
	d1.nbins = 9;
	vector<float> descriptorsValues_L;
	vector<Point> locations_RGB;
	vector<float> descriptorsValues_R;
	vector<Point> locations_NIR;

	//resize images to match HOG window size, inter are is best method for down-sampling an image, as it doesn't cause moire-effects (http://docs.opencv.org/modules/imgproc/doc/geometric_transformations.html#resize)
	Mat rgb_resized, nir_resized;
	resize(img_L, rgb_resized, d1.winSize, INTER_AREA);
	resize(img_R, nir_resized, d1.winSize, INTER_AREA);

	//compute the HOG descriptors
	d1.compute(rgb_resized, descriptorsValues_L, Size(0,0), Size(0,0), locations_RGB);
	d1.compute(nir_resized, descriptorsValues_R, Size(0,0), Size(0,0), locations_NIR);

	//make disparity map according to chosen optimization strategy
	Mat disp;

	if(disparityOptimimizationStragey == dispOpt_WTA)
	{
		//make disparity map with winner-take-all strategy
		disp = makeDisparity_WTA(descriptorsValues_L, descriptorsValues_R, d1.winSize,
								 blockSize, nCells, d1.nbins);
	}
	else if(disparityOptimimizationStragey == dispOpt_SGBM)
	{
		StereoSGBM sgbm; //use the original opencv sgbm as parameter to our modified version
		sgbm.SADWindowSize = SADwindow;
		sgbm.minDisparity = minDisp;
		int nrOfDisp = maxDisp-minDisp;
		while(nrOfDisp % 16 != 0){ nrOfDisp++; }//for sgbm the nr of disparities must be divisible by 16
		sgbm.numberOfDisparities = nrOfDisp;
		sgbm.preFilterCap = preFilterCAP;
		sgbm.uniquenessRatio = uniquenessRatio;
		sgbm.speckleWindowSize = speckleWindow;
		sgbm.speckleRange = speckleRange;
		sgbm.fullDP = true;
		sgbm.P1 = 8 * SADwindow * SADwindow;
		sgbm.P2 = 32 * SADwindow * SADwindow;
		sgbm.disp12MaxDiff = 1;

		Mat disp16;
		compute_SGBM_HOG(rgb_resized, nir_resized, disp16, sgbm, descriptorsValues_L, descriptorsValues_R, blockSize, nCells, d1.nbins);
		normalize(disp16, disp, 0, 255, CV_MINMAX, CV_8U);
	}

	//resize back to original size
	resize(disp, disp, Size(orig_w, orig_h), INTER_AREA);

	//improve disparity map using superpixels


	out_disp = disp;
}

Mat HOG_crossSpectralStereoMatcher::makeDisparity_WTA(vector<float> &values_L, vector<float> &values_R,
													  Size imgSize, int blockSize, int nCells, int nBins)
{
	int w = imgSize.width;
	int h = imgSize.height;

	//get actual width of descriptor matrix - smaller than image, because of border caused by blocks
	int w_descr = (w - blockSize + 1);
	int h_descr = (h - blockSize + 1);

	Mat disp_border(h, w, CV_8UC1, Scalar(0));

	int range = maxDisp - minDisp;
	int binsInDescriptor = nCells * nBins; //every step is one block centered around one pixel in original images
	float smallestDistance = FLT_MAX; //here the smallest distance is stored when searching for best match for every pixel
	int bestMatch = 0;

//    vector<float> descr_L(binsInDescriptor);
//    vector<float> descr_R(binsInDescriptor);

	//go through all descriptors of left image and compare with <range> of descriptors in right image
	//(every descriptor stands for one image pixel)
	for(int y = 0; y < h_descr; ++y)
	{
		for(int x = maxDisp; x < w_descr; ++x) //respect horizontal border caused by disparity-range (we're looking for pixels to the left)
		{
			ulong i = x * h_descr + y;// the real index into descriptor vector

			//step through a range of descriptors for right image and find the best match
			smallestDistance = FLT_MAX; //reset
//            for(int j = 0; j <= range; j++)
			for(int j = minDisp; j <= maxDisp; j++)
			{
				//index in right image, not straight-forward because HOG computes descriptors column-wise, instead of row-wise
				int indexR = i - j * h_descr;

				//get L1 (Manhattan) distance between descriptor vectors. is it the smallest yet?
				float dist = getL1Distance(values_L, i, values_R, indexR, binsInDescriptor);
				if(dist < smallestDistance)
				{
					bestMatch = j;
					smallestDistance = dist;
				}
			}

			//cast to 8bit range, 255 = closest, 1 = farest, 0 = undefined
			uchar dispValue;
//            qDebug() << smallestDistance;
			if(smallestDistance < wtaThreshold)
			{
				dispValue = (uchar)(bestMatch/(float)range * 254.0 + 1.0 + 0.5);
			}
			else //undefined because distance is too big
			{
				dispValue = 0;
			}

			//write the disparity for the best match to disparity image
			disp_border.at<uchar>(y + blockSize/2, x + blockSize/2) = dispValue;
		}//x
	}//y

	return disp_border;
}

float HOG_crossSpectralStereoMatcher::getL1Distance(vector<float>& a, int index_a, vector<float>& b, int index_b, int binsInDescriptor)
{
	int start_a = index_a * binsInDescriptor;
	int start_b = index_b * binsInDescriptor;
	float dist = 0;
	for (int i = 0; i < binsInDescriptor; ++i)
	{
		dist += fabs(a[start_a + i] - b[start_b + i]);
	}
	return dist;
}

void HOG_crossSpectralStereoMatcher::getBestDescriptors(vector<KeyPoint> &outDescr_L, vector<KeyPoint> &outDescr_R, vector<DMatch> &dmatches, float threshold)
{
	//select those descriptors which are "better" then the threshold
	int i = 0;
	foreach (BestDescriptorCandidate c, bestDescrCanditates)
	{
		if(c.dist < threshold)
		{
			outDescr_L.push_back(KeyPoint(c.x1, c.y1, 1));
			outDescr_R.push_back(KeyPoint(c.x2, c.y2, 1));
			dmatches.push_back(DMatch(i,i,c.dist));
			i++;
		}
	}
}

void HOG_crossSpectralStereoMatcher::setBestDescriptors(int w, int h, int x1, int x2, int y, float dist)
{
	//check in which raster cell of the image we're in
	int cellX = x1 / (w / bestDescrRasterDiv);
	int cellY = y / (h / bestDescrRasterDiv);
	int cellIndex = cellY * bestDescrRasterDiv + cellX;

	//if this cost is lower than the one stored for this cell, replace candidate
	float currentDist= bestDescrCanditates[cellIndex].dist;
	if(dist < currentDist)
	{
		bestDescrCanditates[cellIndex].dist = dist;
		bestDescrCanditates[cellIndex].x1 = x1;
		bestDescrCanditates[cellIndex].y1 = y;
		bestDescrCanditates[cellIndex].x2 = x2;
		bestDescrCanditates[cellIndex].y2 = y;
	}
}


/******************************************************************************
 * SET HOG PARAMETERS IN STATIC FUNCTION
 *****************************************************************************/
void HOG_crossSpectralStereoMatcher::setParams(std::vector<float> params)
{
	if(params.size() > 0) disparityOptimimizationStragey = (int)params[0];
	if(params.size() > 1) minDisp = (int)params[1];
	if(params.size() > 2) maxDisp = (float)params[2];
	if(params.size() > 3) wtaThreshold = (float)params[3];
	if(params.size() > 4) SADwindow = (float)params[4];
	if(params.size() > 5) preFilterCAP = (float)params[5];
	if(params.size() > 6) uniquenessRatio = (float)params[6];
	if(params.size() > 7) speckleWindow = (float)params[7];
	if(params.size() > 8) speckleRange = (float)params[8];

	if(params.size() > 9) blockSize = (float)params[9];
	if(params.size() > 10) cellSize = (float)params[10];
	//make sure blockSize is multiple of cellSize
	while(blockSize % cellSize != 0){ blockSize++; }
}


/******************************************************************************
 * SGBM WITH HOG COST FUNCTION, STOLEN & MODIFIED FROM OPENCV
 *****************************************************************************/
void HOG_crossSpectralStereoMatcher::compute_SGBM_HOG(InputArray leftarr, InputArray rightarr, OutputArray disparr, StereoSGBM params,
													  vector<float>& HOGdesriptors_L, vector<float>& HOGdescriptors_R,
													  int blockSize, int nCells, int nBins)
{
	Mat left = leftarr.getMat(), right = rightarr.getMat();
	CV_Assert( left.size() == right.size() && left.type() == right.type() &&
			   left.depth() == CV_8U );

	disparr.create( left.size(), CV_16S );
	Mat disp = disparr.getMat();

	int binsInDescriptor = nCells * nBins;
	computeDisparitySGBM( left, right, disp, params, SGBM_buffer , HOGdesriptors_L, HOGdescriptors_R, blockSize, binsInDescriptor);
	medianBlur(disp, disp, 3);

	if( params.speckleWindowSize > 0 )
		filterSpeckles(disp, (params.minDisparity - 1)* params.DISP_SCALE, params.speckleWindowSize,
					   params.DISP_SCALE*params.speckleRange, SGBM_buffer);
}


//computes the cost for disparity matches for one row based on HOG desriptors
void HOG_crossSpectralStereoMatcher::calcPixelCostHOG(vector<float> &descr_L, vector<float> &descr_R, int blockSize, int y,
													  int imgWidth, int imgHeight, int minDisp, int maxDisp, int binsInDescriptor,
													  CostType *cost)
{
	int w_descr = (imgWidth - blockSize + 1);
	int h_descr = (imgHeight - blockSize + 1);

	//prepare the data
	int dispRange = maxDisp - minDisp;
	memset( cost, 0, imgWidth*dispRange*sizeof(cost[0]) );
//    qDebug() << "row cost values size: " << imgWidth*dispRange;

	//make sure only valid indices in the descriptor vector are used
	//discard y values that go over the "blocksize-induces border"
	if(y < blockSize/2){ return; }
	int yy = (y - blockSize/2);
	if(yy >= h_descr){ return; }

	int cnt = (blockSize/2 + maxDisp-2) * dispRange;
	for(int x = maxDisp-1; x < w_descr; ++x) //respect horizontal border caused by disparity-range (we're looking for pixels to the left)
	{
		uint i = x * h_descr + yy;// the real index into descriptor vector

		//get distance for every descriptor in range
		for(int j = minDisp; j < maxDisp; j++)
		{
			//index in right image, not straight-forward because HOG computes descriptors column-wise, instead of row-wise
			uint indexR = i - j * h_descr;

			//get L1 (Manhattan) distance between descriptor vectors. is it the smallest yet?
			float dist = getL1Distance(descr_L, i, descr_R, indexR, binsInDescriptor);
			cost[cnt++] = (CostType)(dist * 100.0 + 0.5); //*100 to cast float distance to short interval

			//while doing all this also remember the best descriptors
			setBestDescriptors(imgWidth, imgHeight, x, x-j, y, dist);
		}
	}//x
}


/*
 computes disparity for "roi" in img1 w.r.t. img2 and write it to disp1buf.
 that is, disp1buf(x, y)=d means that img1(x+roi.x, y+roi.y) ~ img2(x+roi.x-d, y+roi.y).
 minD <= d < maxD.
 disp2full is the reverse disparity map, that is:
 disp2full(x+roi.x,y+roi.y)=d means that img2(x+roi.x, y+roi.y) ~ img1(x+roi.x+d, y+roi.y)

 note that disp1buf will have the same size as the roi and
 disp2full will have the same size as img1 (or img2).
 On exit disp2buf is not the final disparity, it is an intermediate result that becomes
 final after all the tiles are processed.

 the disparity in disp1buf is written with sub-pixel accuracy
 (4 fractional bits, see StereoSGBM::DISP_SCALE),
 using quadratic interpolation, while the disparity in disp2buf
 is written as is, without interpolation.

 disp2cost also has the same size as img1 (or img2).
 It contains the minimum current cost, used to find the best disparity, corresponding to the minimal cost.
 */
void HOG_crossSpectralStereoMatcher::computeDisparitySGBM(const Mat& img1, const Mat& img2,
														  Mat& disp1, const StereoSGBM &params,
														  Mat& buffer,
														  vector<float>& HOGdescr_L, vector<float>& HOGdescr_R,
														  int blockSize, int binsInDescriptor)
{

//	qDebug() << cv::useOptimized();
#if CV_SSE2
	static const uchar LSBTab[] =
	{
		0, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
		5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
		6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
		5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
		7, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
		5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
		6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
		5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0
	};

	volatile bool useSIMD = checkHardwareSupport(CV_CPU_SSE2);
#endif

	/***** HOG MODE *****/
	if(disp1.rows != img1.rows || disp1.cols != img1.cols)
	{ disp1.create(img1.rows, img1.cols, CV_16S); }
	/***** HOG MODE *****/


	const int ALIGN = 16;
	const int DISP_SHIFT = params.DISP_SHIFT;
	const int DISP_SCALE = (1 << DISP_SHIFT);
	const CostType MAX_COST = SHRT_MAX;

	int minD = params.minDisparity, maxD = minD + params.numberOfDisparities;
	Size SADWindowSize;
	SADWindowSize.width = SADWindowSize.height = params.SADWindowSize > 0 ? params.SADWindowSize : 5;
	int ftzero = std::max(params.preFilterCap, 15) | 1;
	int uniquenessRatio = params.uniquenessRatio >= 0 ? params.uniquenessRatio : 10;
	int disp12MaxDiff = params.disp12MaxDiff > 0 ? params.disp12MaxDiff : 1;
	int P1 = params.P1 > 0 ? params.P1 : 2, P2 = std::max(params.P2 > 0 ? params.P2 : 5, P1+1);
	int k, width = disp1.cols, height = disp1.rows;
	int minX1 = std::max(-maxD, 0), maxX1 = width + std::min(minD, 0);
	int D = maxD - minD, width1 = maxX1 - minX1;
	int INVALID_DISP = minD - 1, INVALID_DISP_SCALED = INVALID_DISP*DISP_SCALE;
	int SW2 = SADWindowSize.width/2, SH2 = SADWindowSize.height/2;
	bool fullDP = params.fullDP;
	int npasses = fullDP ? 2 : 1;
	const int TAB_OFS = 256*4, TAB_SIZE = 256 + TAB_OFS*2;
	PixType clipTab[TAB_SIZE];

	for( k = 0; k < TAB_SIZE; k++ )
		clipTab[k] = (PixType)(std::min(std::max(k - TAB_OFS, -ftzero), ftzero) + ftzero);

	if( minX1 >= maxX1 )
	{
		disp1 = Scalar::all(INVALID_DISP_SCALED);
		return;
	}

	CV_Assert( D % 16 == 0 );

	// NR - the number of directions. the loop on x below that computes Lr assumes that NR == 8.
	// if you change NR, please, modify the loop as well.
	int D2 = D+16, NRD2 = NR2*D2;

	// the number of L_r(.,.) and min_k L_r(.,.) lines in the buffer:
	// for 8-way dynamic programming we need the current row and
	// the previous row, i.e. 2 rows in total
	const int NLR = 2;
	const int LrBorder = NLR - 1;

	// for each possible stereo match (img1(x,y) <=> img2(x-d,y))
	// we keep pixel difference cost (C) and the summary cost over NR directions (S).
	// we also keep all the partial costs for the previous line L_r(x,d) and also min_k L_r(x, k)
	size_t costBufSize = width1*D;
	size_t CSBufSize = costBufSize*(fullDP ? height : 1);
	size_t minLrSize = (width1 + LrBorder*2)*NR2, LrSize = minLrSize*D2;
	int hsumBufNRows = SH2*2 + 2;
	size_t totalBufSize = (LrSize + minLrSize)*NLR*sizeof(CostType) + // minLr[] and Lr[]
	costBufSize*(hsumBufNRows + 1)*sizeof(CostType) + // hsumBuf, pixdiff
	CSBufSize*2*sizeof(CostType) + // C, S
	width*16*img1.channels()*sizeof(PixType) + // temp buffer for computing per-pixel cost
	width*(sizeof(CostType) + sizeof(DispType)) + 1024; // disp2cost + disp2

	if( buffer.empty() || !buffer.isContinuous() ||
		buffer.cols*buffer.rows*buffer.elemSize() < totalBufSize )
		buffer.create(1, (int)totalBufSize, CV_8U);

	// summary cost over different (nDirs) directions
	CostType* Cbuf = (CostType*)alignPtr(buffer.ptr(), ALIGN);
	CostType* Sbuf = Cbuf + CSBufSize;
	CostType* hsumBuf = Sbuf + CSBufSize;
	CostType* pixDiff = hsumBuf + costBufSize*hsumBufNRows;

	CostType* disp2cost = pixDiff + costBufSize + (LrSize + minLrSize)*NLR;
	DispType* disp2ptr = (DispType*)(disp2cost + width);
	PixType* tempBuf = (PixType*)(disp2ptr + width);

	// add P2 to every C(x,y). it saves a few operations in the inner loops
	for( k = 0; k < width1*D; k++ )
		Cbuf[k] = (CostType)P2;

	for( int pass = 1; pass <= npasses; pass++ )
	{
		int x1, y1, x2, y2, dx, dy;

		if( pass == 1 )
		{
			y1 = 0; y2 = height; dy = 1;
			x1 = 0; x2 = width1; dx = 1;
		}
		else
		{
			y1 = height-1; y2 = -1; dy = -1;
			x1 = width1-1; x2 = -1; dx = -1;
		}

		CostType *Lr[NLR]={0}, *minLr[NLR]={0};

		for( k = 0; k < NLR; k++ )
		{
			// shift Lr[k] and minLr[k] pointers, because we allocated them with the borders,
			// and will occasionally use negative indices with the arrays
			// we need to shift Lr[k] pointers by 1, to give the space for d=-1.
			// however, then the alignment will be imperfect, i.e. bad for SSE,
			// thus we shift the pointers by 8 (8*sizeof(short) == 16 - ideal alignment)
			Lr[k] = pixDiff + costBufSize + LrSize*k + NRD2*LrBorder + 8;
			memset( Lr[k] - LrBorder*NRD2 - 8, 0, LrSize*sizeof(CostType) );
			minLr[k] = pixDiff + costBufSize + LrSize*NLR + minLrSize*k + NR2*LrBorder;
			memset( minLr[k] - LrBorder*NR2, 0, minLrSize*sizeof(CostType) );
		}

		for( int y = y1; y != y2; y += dy )
		{
			int x, d;
			DispType* disp1ptr = disp1.ptr<DispType>(y);
			CostType* C = Cbuf + (!fullDP ? 0 : y*costBufSize);
			CostType* S = Sbuf + (!fullDP ? 0 : y*costBufSize);

			if( pass == 1 ) // compute C on the first pass, and reuse it on the second pass, if any.
			{
				int dy1 = y == 0 ? 0 : y + SH2, dy2 = y == 0 ? SH2 : dy1;

				for( k = dy1; k <= dy2; k++ )
				{
					CostType* hsumAdd = hsumBuf + (std::min(k, height-1) % hsumBufNRows)*costBufSize;

					if( k < height )
					{
//                        calcPixelCostBT( img1, img2, k, minD, maxD, pixDiff, tempBuf, clipTab, TAB_OFS, ftzero );
						  calcPixelCostHOG( HOGdescr_L, HOGdescr_R, blockSize, k,
											img1.cols, img1.rows, minD, maxD, binsInDescriptor, pixDiff);

						/** DEBUG **/
//                        int costVals = img1.cols * (maxD - minD);
//                        qDebug() << "cost vals per row: " << QString::number(costVals);
//                        for (int i = 0; i < costVals ; ++i)
//                        {
//                            qDebug() << QString::number(pixDiff[i]);
//                        }
//                        qDebug() << "=======================";
						/** DEBUG **/


						memset(hsumAdd, 0, D*sizeof(CostType));
						for( x = 0; x <= SW2*D; x += D )
						{
							int scale = x == 0 ? SW2 + 1 : 1;
							for( d = 0; d < D; d++ )
								hsumAdd[d] = (CostType)(hsumAdd[d] + pixDiff[x + d]*scale);
						}

						if( y > 0 )
						{
							const CostType* hsumSub = hsumBuf + (std::max(y - SH2 - 1, 0) % hsumBufNRows)*costBufSize;
							const CostType* Cprev = !fullDP || y == 0 ? C : C - costBufSize;

							for( x = D; x < width1*D; x += D )
							{
								const CostType* pixAdd = pixDiff + std::min(x + SW2*D, (width1-1)*D);
								const CostType* pixSub = pixDiff + std::max(x - (SW2+1)*D, 0);

							#if CV_SSE2
								if( useSIMD )
								{
									for( d = 0; d < D; d += 8 )
									{
										__m128i hv = _mm_load_si128((const __m128i*)(hsumAdd + x - D + d));
										__m128i Cx = _mm_load_si128((__m128i*)(Cprev + x + d));
										hv = _mm_adds_epi16(_mm_subs_epi16(hv,
																		   _mm_load_si128((const __m128i*)(pixSub + d))),
															_mm_load_si128((const __m128i*)(pixAdd + d)));
										Cx = _mm_adds_epi16(_mm_subs_epi16(Cx,
																		   _mm_load_si128((const __m128i*)(hsumSub + x + d))),
															hv);
										_mm_store_si128((__m128i*)(hsumAdd + x + d), hv);
										_mm_store_si128((__m128i*)(C + x + d), Cx);
									}
								}
								else
							#endif
								{
									for( d = 0; d < D; d++ )
									{
										int hv = hsumAdd[x + d] = (CostType)(hsumAdd[x - D + d] + pixAdd[d] - pixSub[d]);
										C[x + d] = (CostType)(Cprev[x + d] + hv - hsumSub[x + d]);
									}
								}
							}
						}
						else
						{
							for( x = D; x < width1*D; x += D )
							{
								const CostType* pixAdd = pixDiff + std::min(x + SW2*D, (width1-1)*D);
								const CostType* pixSub = pixDiff + std::max(x - (SW2+1)*D, 0);

								for( d = 0; d < D; d++ )
									hsumAdd[x + d] = (CostType)(hsumAdd[x - D + d] + pixAdd[d] - pixSub[d]);
							}
						}
					}

					if( y == 0 )
					{
						int scale = k == 0 ? SH2 + 1 : 1;
						for( x = 0; x < width1*D; x++ )
							C[x] = (CostType)(C[x] + hsumAdd[x]*scale);
					}
				}

				// also, clear the S buffer
				for( k = 0; k < width1*D; k++ )
					S[k] = 0;
			}

			// clear the left and the right borders
			memset( Lr[0] - NRD2*LrBorder - 8, 0, NRD2*LrBorder*sizeof(CostType) );
			memset( Lr[0] + width1*NRD2 - 8, 0, NRD2*LrBorder*sizeof(CostType) );
			memset( minLr[0] - NR2*LrBorder, 0, NR2*LrBorder*sizeof(CostType) );
			memset( minLr[0] + width1*NR2, 0, NR2*LrBorder*sizeof(CostType) );

			/*
			 [formula 13 in the paper]
			 compute L_r(p, d) = C(p, d) +
			 min(L_r(p-r, d),
			 L_r(p-r, d-1) + P1,
			 L_r(p-r, d+1) + P1,
			 min_k L_r(p-r, k) + P2) - min_k L_r(p-r, k)
			 where p = (x,y), r is one of the directions.
			 we process all the directions at once:
			 0: r=(-dx, 0)
			 1: r=(-1, -dy)
			 2: r=(0, -dy)
			 3: r=(1, -dy)
			 4: r=(-2, -dy)
			 5: r=(-1, -dy*2)
			 6: r=(1, -dy*2)
			 7: r=(2, -dy)
			 */
			for( x = x1; x != x2; x += dx )
			{
				int xm = x*NR2, xd = xm*D2;

				int delta0 = minLr[0][xm - dx*NR2] + P2, delta1 = minLr[1][xm - NR2 + 1] + P2;
				int delta2 = minLr[1][xm + 2] + P2, delta3 = minLr[1][xm + NR2 + 3] + P2;

				CostType* Lr_p0 = Lr[0] + xd - dx*NRD2;
				CostType* Lr_p1 = Lr[1] + xd - NRD2 + D2;
				CostType* Lr_p2 = Lr[1] + xd + D2*2;
				CostType* Lr_p3 = Lr[1] + xd + NRD2 + D2*3;

				Lr_p0[-1] = Lr_p0[D] = Lr_p1[-1] = Lr_p1[D] =
				Lr_p2[-1] = Lr_p2[D] = Lr_p3[-1] = Lr_p3[D] = MAX_COST;

				CostType* Lr_p = Lr[0] + xd;
				const CostType* Cp = C + x*D;
				CostType* Sp = S + x*D;

			#if CV_SSE2
				if( useSIMD )
				{
					__m128i _P1 = _mm_set1_epi16((short)P1);

					__m128i _delta0 = _mm_set1_epi16((short)delta0);
					__m128i _delta1 = _mm_set1_epi16((short)delta1);
					__m128i _delta2 = _mm_set1_epi16((short)delta2);
					__m128i _delta3 = _mm_set1_epi16((short)delta3);
					__m128i _minL0 = _mm_set1_epi16((short)MAX_COST);

					for( d = 0; d < D; d += 8 )
					{
						__m128i Cpd = _mm_load_si128((const __m128i*)(Cp + d));
						__m128i L0, L1, L2, L3;

						L0 = _mm_load_si128((const __m128i*)(Lr_p0 + d));
						L1 = _mm_load_si128((const __m128i*)(Lr_p1 + d));
						L2 = _mm_load_si128((const __m128i*)(Lr_p2 + d));
						L3 = _mm_load_si128((const __m128i*)(Lr_p3 + d));

						L0 = _mm_min_epi16(L0, _mm_adds_epi16(_mm_loadu_si128((const __m128i*)(Lr_p0 + d - 1)), _P1));
						L0 = _mm_min_epi16(L0, _mm_adds_epi16(_mm_loadu_si128((const __m128i*)(Lr_p0 + d + 1)), _P1));

						L1 = _mm_min_epi16(L1, _mm_adds_epi16(_mm_loadu_si128((const __m128i*)(Lr_p1 + d - 1)), _P1));
						L1 = _mm_min_epi16(L1, _mm_adds_epi16(_mm_loadu_si128((const __m128i*)(Lr_p1 + d + 1)), _P1));

						L2 = _mm_min_epi16(L2, _mm_adds_epi16(_mm_loadu_si128((const __m128i*)(Lr_p2 + d - 1)), _P1));
						L2 = _mm_min_epi16(L2, _mm_adds_epi16(_mm_loadu_si128((const __m128i*)(Lr_p2 + d + 1)), _P1));

						L3 = _mm_min_epi16(L3, _mm_adds_epi16(_mm_loadu_si128((const __m128i*)(Lr_p3 + d - 1)), _P1));
						L3 = _mm_min_epi16(L3, _mm_adds_epi16(_mm_loadu_si128((const __m128i*)(Lr_p3 + d + 1)), _P1));

						L0 = _mm_min_epi16(L0, _delta0);
						L0 = _mm_adds_epi16(_mm_subs_epi16(L0, _delta0), Cpd);

						L1 = _mm_min_epi16(L1, _delta1);
						L1 = _mm_adds_epi16(_mm_subs_epi16(L1, _delta1), Cpd);

						L2 = _mm_min_epi16(L2, _delta2);
						L2 = _mm_adds_epi16(_mm_subs_epi16(L2, _delta2), Cpd);

						L3 = _mm_min_epi16(L3, _delta3);
						L3 = _mm_adds_epi16(_mm_subs_epi16(L3, _delta3), Cpd);

						_mm_store_si128( (__m128i*)(Lr_p + d), L0);
						_mm_store_si128( (__m128i*)(Lr_p + d + D2), L1);
						_mm_store_si128( (__m128i*)(Lr_p + d + D2*2), L2);
						_mm_store_si128( (__m128i*)(Lr_p + d + D2*3), L3);

						__m128i t0 = _mm_min_epi16(_mm_unpacklo_epi16(L0, L2), _mm_unpackhi_epi16(L0, L2));
						__m128i t1 = _mm_min_epi16(_mm_unpacklo_epi16(L1, L3), _mm_unpackhi_epi16(L1, L3));
						t0 = _mm_min_epi16(_mm_unpacklo_epi16(t0, t1), _mm_unpackhi_epi16(t0, t1));
						_minL0 = _mm_min_epi16(_minL0, t0);

						__m128i Sval = _mm_load_si128((const __m128i*)(Sp + d));

						L0 = _mm_adds_epi16(L0, L1);
						L2 = _mm_adds_epi16(L2, L3);
						Sval = _mm_adds_epi16(Sval, L0);
						Sval = _mm_adds_epi16(Sval, L2);

						_mm_store_si128((__m128i*)(Sp + d), Sval);
					}

					_minL0 = _mm_min_epi16(_minL0, _mm_srli_si128(_minL0, 8));
					_mm_storel_epi64((__m128i*)&minLr[0][xm], _minL0);
				}
				else
			#endif
				{
					int minL0 = MAX_COST, minL1 = MAX_COST, minL2 = MAX_COST, minL3 = MAX_COST;

					for( d = 0; d < D; d++ )
					{
						int Cpd = Cp[d], L0, L1, L2, L3;

						L0 = Cpd + std::min((int)Lr_p0[d], std::min(Lr_p0[d-1] + P1, std::min(Lr_p0[d+1] + P1, delta0))) - delta0;
						L1 = Cpd + std::min((int)Lr_p1[d], std::min(Lr_p1[d-1] + P1, std::min(Lr_p1[d+1] + P1, delta1))) - delta1;
						L2 = Cpd + std::min((int)Lr_p2[d], std::min(Lr_p2[d-1] + P1, std::min(Lr_p2[d+1] + P1, delta2))) - delta2;
						L3 = Cpd + std::min((int)Lr_p3[d], std::min(Lr_p3[d-1] + P1, std::min(Lr_p3[d+1] + P1, delta3))) - delta3;

						Lr_p[d] = (CostType)L0;
						minL0 = std::min(minL0, L0);

						Lr_p[d + D2] = (CostType)L1;
						minL1 = std::min(minL1, L1);

						Lr_p[d + D2*2] = (CostType)L2;
						minL2 = std::min(minL2, L2);

						Lr_p[d + D2*3] = (CostType)L3;
						minL3 = std::min(minL3, L3);

						Sp[d] = saturate_cast<CostType>(Sp[d] + L0 + L1 + L2 + L3);
					}
					minLr[0][xm] = (CostType)minL0;
					minLr[0][xm+1] = (CostType)minL1;
					minLr[0][xm+2] = (CostType)minL2;
					minLr[0][xm+3] = (CostType)minL3;
				}
			}

			if( pass == npasses )
			{
				for( x = 0; x < width; x++ )
				{
					disp1ptr[x] = disp2ptr[x] = (DispType)INVALID_DISP_SCALED;
					disp2cost[x] = MAX_COST;
				}

				for( x = width1 - 1; x >= 0; x-- )
				{
					CostType* Sp = S + x*D;
					int minS = MAX_COST, bestDisp = -1;

					if( npasses == 1 )
					{
						int xm = x*NR2, xd = xm*D2;

						int minL0 = MAX_COST;
						int delta0 = minLr[0][xm + NR2] + P2;
						CostType* Lr_p0 = Lr[0] + xd + NRD2;
						Lr_p0[-1] = Lr_p0[D] = MAX_COST;
						CostType* Lr_p = Lr[0] + xd;

						const CostType* Cp = C + x*D;

					#if CV_SSE2
						if( useSIMD )
						{
							__m128i _P1 = _mm_set1_epi16((short)P1);
							__m128i _delta0 = _mm_set1_epi16((short)delta0);

							__m128i _minL0 = _mm_set1_epi16((short)minL0);
							__m128i _minS = _mm_set1_epi16(MAX_COST), _bestDisp = _mm_set1_epi16(-1);
							__m128i _d8 = _mm_setr_epi16(0, 1, 2, 3, 4, 5, 6, 7), _8 = _mm_set1_epi16(8);

							for( d = 0; d < D; d += 8 )
							{
								__m128i Cpd = _mm_load_si128((const __m128i*)(Cp + d)), L0;

								L0 = _mm_load_si128((const __m128i*)(Lr_p0 + d));
								L0 = _mm_min_epi16(L0, _mm_adds_epi16(_mm_loadu_si128((const __m128i*)(Lr_p0 + d - 1)), _P1));
								L0 = _mm_min_epi16(L0, _mm_adds_epi16(_mm_loadu_si128((const __m128i*)(Lr_p0 + d + 1)), _P1));
								L0 = _mm_min_epi16(L0, _delta0);
								L0 = _mm_adds_epi16(_mm_subs_epi16(L0, _delta0), Cpd);

								_mm_store_si128((__m128i*)(Lr_p + d), L0);
								_minL0 = _mm_min_epi16(_minL0, L0);
								L0 = _mm_adds_epi16(L0, *(__m128i*)(Sp + d));
								_mm_store_si128((__m128i*)(Sp + d), L0);

								__m128i mask = _mm_cmpgt_epi16(_minS, L0);
								_minS = _mm_min_epi16(_minS, L0);
								_bestDisp = _mm_xor_si128(_bestDisp, _mm_and_si128(_mm_xor_si128(_bestDisp,_d8), mask));
								_d8 = _mm_adds_epi16(_d8, _8);
							}

							short /*CV_DECL_ALIGNED(16)*/ bestDispBuf[8];
							_mm_store_si128((__m128i*)bestDispBuf, _bestDisp);

							_minL0 = _mm_min_epi16(_minL0, _mm_srli_si128(_minL0, 8));
							_minL0 = _mm_min_epi16(_minL0, _mm_srli_si128(_minL0, 4));
							_minL0 = _mm_min_epi16(_minL0, _mm_srli_si128(_minL0, 2));

							__m128i qS = _mm_min_epi16(_minS, _mm_srli_si128(_minS, 8));
							qS = _mm_min_epi16(qS, _mm_srli_si128(qS, 4));
							qS = _mm_min_epi16(qS, _mm_srli_si128(qS, 2));

							minLr[0][xm] = (CostType)_mm_cvtsi128_si32(_minL0);
							minS = (CostType)_mm_cvtsi128_si32(qS);

							qS = _mm_shuffle_epi32(_mm_unpacklo_epi16(qS, qS), 0);
							qS = _mm_cmpeq_epi16(_minS, qS);
							int idx = _mm_movemask_epi8(_mm_packs_epi16(qS, qS)) & 255;

							bestDisp = bestDispBuf[LSBTab[idx]];
						}
						else
					#endif
						{
							for( d = 0; d < D; d++ )
							{
								int L0 = Cp[d] + std::min((int)Lr_p0[d], std::min(Lr_p0[d-1] + P1, std::min(Lr_p0[d+1] + P1, delta0))) - delta0;

								Lr_p[d] = (CostType)L0;
								minL0 = std::min(minL0, L0);

								int Sval = Sp[d] = saturate_cast<CostType>(Sp[d] + L0);
								if( Sval < minS )
								{
									minS = Sval;
									bestDisp = d;
								}
							}
							minLr[0][xm] = (CostType)minL0;
						}
					}
					else
					{
						for( d = 0; d < D; d++ )
						{
							int Sval = Sp[d];
							if( Sval < minS )
							{
								minS = Sval;
								bestDisp = d;
							}
						}
					}

					for( d = 0; d < D; d++ )
					{
						if( Sp[d]*(100 - uniquenessRatio) < minS*100 && std::abs(bestDisp - d) > 1 )
							break;
					}
					if( d < D )
						continue;
					d = bestDisp;
					int _x2 = x + minX1 - d - minD;
					if( disp2cost[_x2] > minS )
					{
						disp2cost[_x2] = (CostType)minS;
						disp2ptr[_x2] = (DispType)(d + minD);
					}

					if( 0 < d && d < D-1 )
					{
						// do subpixel quadratic interpolation:
						//   fit parabola into (x1=d-1, y1=Sp[d-1]), (x2=d, y2=Sp[d]), (x3=d+1, y3=Sp[d+1])
						//   then find minimum of the parabola.
						int denom2 = std::max(Sp[d-1] + Sp[d+1] - 2*Sp[d], 1);
						d = d*DISP_SCALE + ((Sp[d-1] - Sp[d+1])*DISP_SCALE + denom2)/(denom2*2);
					}
					else
						d *= DISP_SCALE;
					disp1ptr[x + minX1] = (DispType)(d + minD*DISP_SCALE);
				}

				for( x = minX1; x < maxX1; x++ )
				{
					// we round the computed disparity both towards -inf and +inf and check
					// if either of the corresponding disparities in disp2 is consistent.
					// This is to give the computed disparity a chance to look valid if it is.
					int d1 = disp1ptr[x];
					if( d1 == INVALID_DISP_SCALED )
						continue;
					int _d = d1 >> DISP_SHIFT;
					int d_ = (d1 + DISP_SCALE-1) >> DISP_SHIFT;
					int _x = x - _d, x_ = x - d_;
					if( 0 <= _x && _x < width && disp2ptr[_x] >= minD && std::abs(disp2ptr[_x] - _d) > disp12MaxDiff &&
					   0 <= x_ && x_ < width && disp2ptr[x_] >= minD && std::abs(disp2ptr[x_] - d_) > disp12MaxDiff )
						disp1ptr[x] = (DispType)INVALID_DISP_SCALED;
				}
			}

			// now shift the cyclic buffers
			std::swap( Lr[0], Lr[1] );
			std::swap( minLr[0], minLr[1] );
		}
	}
}

/*
 For each pixel row1[x], max(-maxD, 0) <= minX <= x < maxX <= width - max(0, -minD),
 and for each disparity minD<=d<maxD the function
 computes the cost (cost[(x-minX)*(maxD - minD) + (d - minD)]), depending on the difference between
 row1[x] and row2[x-d]. The subpixel algorithm from
 "Depth Discontinuities by Pixel-to-Pixel Stereo" by Stan Birchfield and C. Tomasi
 is used, hence the suffix BT.

 the temporary buffer should contain width2*2 elements
 */
void HOG_crossSpectralStereoMatcher::calcPixelCostBT( const Mat& img1, const Mat& img2, int y,
							int minD, int maxD, CostType* cost,
							PixType* buffer, const PixType* tab,
							int tabOfs, int )
{
	int x, c, width = img1.cols, cn = img1.channels();
	int minX1 = std::max(-maxD, 0), maxX1 = width + std::min(minD, 0);
	int minX2 = std::max(minX1 - maxD, 0), maxX2 = std::min(maxX1 - minD, width);
	int D = maxD - minD, width1 = maxX1 - minX1, width2 = maxX2 - minX2;
	const PixType *row1 = img1.ptr<PixType>(y), *row2 = img2.ptr<PixType>(y);
	PixType *prow1 = buffer + width2*2, *prow2 = prow1 + width*cn*2;

	tab += tabOfs;

	for( c = 0; c < cn*2; c++ )
	{
		prow1[width*c] = prow1[width*c + width-1] =
		prow2[width*c] = prow2[width*c + width-1] = tab[0];
	}

	int n1 = y > 0 ? -(int)img1.step : 0, s1 = y < img1.rows-1 ? (int)img1.step : 0;
	int n2 = y > 0 ? -(int)img2.step : 0, s2 = y < img2.rows-1 ? (int)img2.step : 0;

	if( cn == 1 )
	{
		for( x = 1; x < width-1; x++ )
		{
			prow1[x] = tab[(row1[x+1] - row1[x-1])*2 + row1[x+n1+1] - row1[x+n1-1] + row1[x+s1+1] - row1[x+s1-1]];
			prow2[width-1-x] = tab[(row2[x+1] - row2[x-1])*2 + row2[x+n2+1] - row2[x+n2-1] + row2[x+s2+1] - row2[x+s2-1]];

			prow1[x+width] = row1[x];
			prow2[width-1-x+width] = row2[x];
		}
	}
	else
	{
		for( x = 1; x < width-1; x++ )
		{
			prow1[x] = tab[(row1[x*3+3] - row1[x*3-3])*2 + row1[x*3+n1+3] - row1[x*3+n1-3] + row1[x*3+s1+3] - row1[x*3+s1-3]];
			prow1[x+width] = tab[(row1[x*3+4] - row1[x*3-2])*2 + row1[x*3+n1+4] - row1[x*3+n1-2] + row1[x*3+s1+4] - row1[x*3+s1-2]];
			prow1[x+width*2] = tab[(row1[x*3+5] - row1[x*3-1])*2 + row1[x*3+n1+5] - row1[x*3+n1-1] + row1[x*3+s1+5] - row1[x*3+s1-1]];

			prow2[width-1-x] = tab[(row2[x*3+3] - row2[x*3-3])*2 + row2[x*3+n2+3] - row2[x*3+n2-3] + row2[x*3+s2+3] - row2[x*3+s2-3]];
			prow2[width-1-x+width] = tab[(row2[x*3+4] - row2[x*3-2])*2 + row2[x*3+n2+4] - row2[x*3+n2-2] + row2[x*3+s2+4] - row2[x*3+s2-2]];
			prow2[width-1-x+width*2] = tab[(row2[x*3+5] - row2[x*3-1])*2 + row2[x*3+n2+5] - row2[x*3+n2-1] + row2[x*3+s2+5] - row2[x*3+s2-1]];

			prow1[x+width*3] = row1[x*3];
			prow1[x+width*4] = row1[x*3+1];
			prow1[x+width*5] = row1[x*3+2];

			prow2[width-1-x+width*3] = row2[x*3];
			prow2[width-1-x+width*4] = row2[x*3+1];
			prow2[width-1-x+width*5] = row2[x*3+2];
		}
	}

	memset( cost, 0, width1*D*sizeof(cost[0]) );
//    qDebug() << QString::number(width1*D);

	buffer -= minX2;
	cost -= minX1*D + minD; // simplify the cost indices inside the loop

#if CV_SSE2
	volatile bool useSIMD = checkHardwareSupport(CV_CPU_SSE2);
#endif

#if 1
	for( c = 0; c < cn*2; c++, prow1 += width, prow2 += width )
	{
		int diff_scale = c < cn ? 0 : 2;

		// precompute
		//   v0 = min(row2[x-1/2], row2[x], row2[x+1/2]) and
		//   v1 = max(row2[x-1/2], row2[x], row2[x+1/2]) and
		for( x = minX2; x < maxX2; x++ )
		{
			int v = prow2[x];
			int vl = x > 0 ? (v + prow2[x-1])/2 : v;
			int vr = x < width-1 ? (v + prow2[x+1])/2 : v;
			int v0 = std::min(vl, vr); v0 = std::min(v0, v);
			int v1 = std::max(vl, vr); v1 = std::max(v1, v);
			buffer[x] = (PixType)v0;
			buffer[x + width2] = (PixType)v1;
		}

		for( x = minX1; x < maxX1; x++ )
		{
			int u = prow1[x];
			int ul = x > 0 ? (u + prow1[x-1])/2 : u;
			int ur = x < width-1 ? (u + prow1[x+1])/2 : u;
			int u0 = std::min(ul, ur); u0 = std::min(u0, u);
			int u1 = std::max(ul, ur); u1 = std::max(u1, u);

		#if CV_SSE2
			if( useSIMD )
			{
				__m128i _u = _mm_set1_epi8((char)u), _u0 = _mm_set1_epi8((char)u0);
				__m128i _u1 = _mm_set1_epi8((char)u1), z = _mm_setzero_si128();
				__m128i ds = _mm_cvtsi32_si128(diff_scale);

				for( int d = minD; d < maxD; d += 16 )
				{
					__m128i _v = _mm_loadu_si128((const __m128i*)(prow2 + width-x-1 + d));
					__m128i _v0 = _mm_loadu_si128((const __m128i*)(buffer + width-x-1 + d));
					__m128i _v1 = _mm_loadu_si128((const __m128i*)(buffer + width-x-1 + d + width2));
					__m128i c0 = _mm_max_epu8(_mm_subs_epu8(_u, _v1), _mm_subs_epu8(_v0, _u));
					__m128i c1 = _mm_max_epu8(_mm_subs_epu8(_v, _u1), _mm_subs_epu8(_u0, _v));
					__m128i diff = _mm_min_epu8(c0, c1);

					c0 = _mm_load_si128((__m128i*)(cost + x*D + d));
					c1 = _mm_load_si128((__m128i*)(cost + x*D + d + 8));

					_mm_store_si128((__m128i*)(cost + x*D + d), _mm_adds_epi16(c0, _mm_srl_epi16(_mm_unpacklo_epi8(diff,z), ds)));
					_mm_store_si128((__m128i*)(cost + x*D + d + 8), _mm_adds_epi16(c1, _mm_srl_epi16(_mm_unpackhi_epi8(diff,z), ds)));
				}
			}
			else
		#endif
			{
				for( int d = minD; d < maxD; d++ )
				{
					int v = prow2[width-x-1 + d];
					int v0 = buffer[width-x-1 + d];
					int v1 = buffer[width-x-1 + d + width2];
					int c0 = std::max(0, u - v1); c0 = std::max(c0, v0 - u);
					int c1 = std::max(0, v - u1); c1 = std::max(c1, u0 - v);

					cost[x*D + d] = (CostType)(cost[x*D+d] + (std::min(c0, c1) >> diff_scale));
				}
			}
		}
	}
#else
	for( c = 0; c < cn*2; c++, prow1 += width, prow2 += width )
	{
		for( x = minX1; x < maxX1; x++ )
		{
			int u = prow1[x];
		#if CV_SSE2
			if( useSIMD )
			{
				__m128i _u = _mm_set1_epi8(u), z = _mm_setzero_si128();

				for( int d = minD; d < maxD; d += 16 )
				{
					__m128i _v = _mm_loadu_si128((const __m128i*)(prow2 + width-1-x + d));
					__m128i diff = _mm_adds_epu8(_mm_subs_epu8(_u,_v), _mm_subs_epu8(_v,_u));
					__m128i c0 = _mm_load_si128((__m128i*)(cost + x*D + d));
					__m128i c1 = _mm_load_si128((__m128i*)(cost + x*D + d + 8));

					_mm_store_si128((__m128i*)(cost + x*D + d), _mm_adds_epi16(c0, _mm_unpacklo_epi8(diff,z)));
					_mm_store_si128((__m128i*)(cost + x*D + d + 8), _mm_adds_epi16(c1, _mm_unpackhi_epi8(diff,z)));
				}
			}
			else
		#endif
			{
				for( int d = minD; d < maxD; d++ )
				{
					int v = prow2[width-1-x + d];
					cost[x*D + d] = (CostType)(cost[x*D + d] + (CostType)std::abs(u - v));
				}
			}
		}
	}
#endif
}
