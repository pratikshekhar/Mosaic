/*
 * hw3.c
 *
 *  Created on: Mar 10, 2013
 *      Author: lou
 */

#include "hw3.h"

/**
 * Loads an image (stored as an IplImage struct) for each
 * filename provided.
 * @param numImages		The number of images to load
 * @param filenames		A list of strings of the filenames to load
 */
IplImage** loadImages(int numImages, char ** fileNames) {
	IplImage** rv; // the return result
	int i; // used for looping

	// TODO: allocate an array of numImages IplImage* s
	rv = (IplImage**)malloc(numImages * sizeof(IplImage *));

	// TODO: iterate over each filename
	for(i = 0 ; i < numImages ; i++){

		// TODO: load the image via the cvLoadImage function
	rv[i] = cvLoadImage(fileNames[i], CV_LOAD_IMAGE_COLOR);
	if(!rv[i]){

		// TODO: check if the file could not be loaded.
		
		printf("Error loading Image: %s\n", fileNames[i]);
		
		// if it could not, print an error to stderr and return NULL
		return NULL;
		}

	}

	// TODO: return results
	return rv;
}

/**
 * Computes the distance between two colors (stored as CvScalars).
 *
 * Given a CvScalar c, you can access the blue, green, and red (BGR) elements
 * via c.val[0], c.val[1], and c.val[2], respectively.
 *
 * This function computes the distance between two colors as the euclidean
 * distance between the two BGR vectors.
 *
 * @see http://en.wikipedia.org/wiki/Euclidean_distance
 *
 * @param c1	The first color
 * @param c2	The second color
 * @returns		The euclidean distance between the two 3-d vectors
 */
double colorDistance(CvScalar c1, CvScalar c2) {
	double d = 0; // the result
	int i; // an iterator
	double diff_blue = 0;
	double diff_green = 0;
	double diff_red = 0;

	double average = 0;

	diff_blue = (c1.val[0] - c2.val[0]);
	diff_green = (c1.val[1] - c2.val[1]);
	diff_red = (c1.val[2] - c2.val[2]);
	
	// TODO: return the square root of the result.
	// If d is zero, just return 0.

	d =  (pow(diff_blue,2) + pow(diff_green,2) + pow(diff_red,2));
	if (d == 0){
		return d;
		}
	else {
		return sqrt(d);
	}
}

/**
 * Splits up an image into numColumns by numRows sub-images and returns the results.
 *
 * @param src	The source image to split
 * @param numColumns	The number of columns to split into
 * @param numRows 		The number of rows to split into
 * @returns				A numColumns x numRows array of IplImages
 */
IplImage ** getSubImages(IplImage* src, int numColumns, int numRows) {
	int cellWidth, cellHeight, y, x, i;
	IplImage ** rv;
	CvSize s = cvGetSize(src);

	// Compute the cell width and the cell height
	cellWidth = s.width / numColumns;
	cellHeight = s.height / numRows;

	// Allocate an array of IplImage* s
	rv = malloc(sizeof(IplImage*) * numColumns * numRows);
	if (rv == NULL) {
		fprintf(stderr, "Could not allocate rv array\n");
	}

	// Iterate over the cells
	i = 0;
	for (y = 0; y < s.height; y += cellHeight)
		for (x = 0; x < s.width; x += cellWidth) {
			// Create a new image of size cellWidth x cellHeight and
			// store it in rv[i]. The depth and number of channels should
			// be the same as src.
			rv[i] = cvCreateImage(cvSize(cellWidth, cellHeight), src->depth,
					src->nChannels);
			if (rv[i] == NULL) {
				fprintf(stderr, "Could not allocate image %d\n", i);
			}

			// set the ROI of the src image
			cvSetImageROI(src, cvRect(x, y, cellWidth, cellHeight));

			// copy src to rv[i] using cvCopy, which obeys ROI
			cvCopy(src, rv[i], NULL);

			// reset the src image roi
			cvResetImageROI(src);

			// increment i
			i++;
		}

	// return the result
	return rv;
}

/**
 * Finds the CvScalar in colors closest to t using the colorDistance function.
 * @param t		 	The color to look for
 * @param scolors	The colors to look through
 * @param numColors	The length of scolors
 * @returns			The index of scolors that t is closest to
 * 					(i.e., colorDistance( t, scolors[result]) <=
 * 					colorDistance( t, scolors[i]) for all i != result)
 */
int findClosest(CvScalar t, CvScalar* scolors, int numColors) {
	int rv = 0, // return value
		 i; // used to iterate
	double d, // stores the result of distance
	       m = colorDistance(t, scolors[0]); // the current minimum distance
	// TODO: iterate over scolors
	for(i=0 ; i < numColors; i++){
	

		// TODO: compute the distance between t and scolors[i]
	d = colorDistance(t,scolors[i]);
		// TODO: check if the distance is less then current minimum
	if(d < m ){
			// TODO: if it is, store i as the current result and cache the minimum distance
		m = d;
		rv = i;
		}
	}
	// TODO: return result.
	return rv;
}

/**
 * For each image provided, computes the average color vector
 * (represented as a CvScalar object).
 *
 * @param images	The images
 * @param numImages	The length of images
 * @returns 		An numImages length array of CvScalars where rv[i] is the average color in images[i]
 */
CvScalar* getAvgColors(IplImage** images, int numImages) {
	CvScalar* rv;
	int i;
	// TODO: create return vector
	rv = (CvScalar *) malloc(sizeof(CvScalar)*numImages);
	// TODO: iterate over images and compute average color
	for ( i = 0 ; i < numImages ; i++){
		// TODO: for each image, compute the average color (hint: use cvAvg)
	rv[i] = cvAvg(images[i],NULL);
	}
	// TODO: return result
	return rv;
}	

/**
 * Given an ordered list of images (iclosest), creates a
 * numColumns x numRows grid in a new image, copies each image in, and returns the result.
 *
 * Thus, if numColumns is 10, numRows is 5, and each iclosest image is 64x64, the resulting image
 * would be 640x320 pixels.
 *
 * @param iclostest		A numColumns x numRows list of images in row-major order to be put into the resulting image.
 * @param numColumns  	Number of horizontal cells
 * @param numRows		Number of vertical cells
 */
IplImage* stitchImages(IplImage** iclosest, int numColumns, int numRows) {
	int j, // for iterating over the rows
	    i; // for iterating over the columns

	// TODO: using cvGetSize, get the size of the first image in iclosest.
	CvSize size1;
	size1	= cvGetSize(iclosest[0]);
	// remember all of the images should be the same size
	CvSize final_img_size;
	final_img_size	= cvSize((numColumns * size1.width), (numRows * size1.height));
	// TODO: Compute the size of the final destination image.

	// TODO: allocate the return image. This can be potentially large, so
	
	IplImage * rv = cvCreateImage( final_img_size,iclosest[0]->depth,iclosest[0]->nChannels);
	// you should make sure the result is not null

	if( rv!= NULL){
		int index_result = 0;
		
	// TODO: iterate over each cell and copy the closest image into it
		for(i = 0 ; i < numRows; i ++){
			for(j = 0 ; j < numColumns ; j++){
			// TODO: set the ROI of the result
		cvSetImageROI( rv, cvRect((j * size1.width), (i * size1.height),(size1.width),(size1.height)));

			// TODO: copy the proper image into the result

		cvCopy(iclosest[index_result] , rv , NULL);
		index_result++;
		}
	}
			// TODO: reset the ROI of the result
		cvSetImageROI(rv,cvRect(0,0,final_img_size.width,final_img_size.height));
	// TODO: return the result


	return rv;
	}

	else if(rv == NULL){
		printf("Error\n");
	}
}
