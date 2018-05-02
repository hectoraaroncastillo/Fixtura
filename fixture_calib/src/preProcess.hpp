// Pre-proccess image.h;
#include <ctype.h>
#include <math.h>
#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/core/core.hpp>
#include <stdlib.h>
#include <iostream>
#include "opencv2/calib3d/calib3d.hpp"
#include <fstream>
#include <time.h>
#include <stdio.h>

using namespace cv;
using namespace std;


typedef enum FixtureMessages{
	SUCCESS			= 	0,
	FAILED			=	1,
	CANT_OPEN_CAMERA	=	2,
	EMPTY_STRING_OPTION	=	3,
	CALIBRATION_NOT_FOUND	=	4,
	NO_LEDS_FOUND		= 	5,
	MORE_THAN_ONE_LED_FOUND =	6,
}FixtureMessages;



#ifndef THRESHOLD_VALUE
#define	THRESHOLD_VALUE	250
#endif//THRESHOLD_VALUE

#ifndef THRESHOLD_MAX_VALUE
#define	THRESHOLD_MAX_VALUE	255
#endif//THRESHOLD_MAX_VALUE

#ifndef THRESHOLD_VALUE_CHANNELS
#define	THRESHOLD_VALUE_CHANNELS	200
#endif//THRESHOLD_VALUE_CHANNELS

#ifndef LOW_IMAGE_LIMIT
#define	LOW_IMAGE_LIMIT	190
#endif//LOW_IMAGE_LIMIT

#ifndef UP_IMAGE_LIMIT
#define	UP_IMAGE_LIMIT	50
#endif//UP_IMAGE_LIMIT

#ifndef	AREA_THRESHOLD
#define AREA_THRESHOLD	25
#endif//AREA_THRESHOLD

#ifndef AREA_UPPER_THRESHOLD
#define AREA_UPPER_THRESHOLD  60
#endif//AREA_UPPER_THRESHOLD

#ifndef EPSILON
#define EPSILON  8
#endif//EPSILON


Mat imagePreProcessing(Mat  image);
vector<Vec3f> houghTransform(Mat image);
Mat createImageMask(Mat image);
vector<Point2f>  getMoments(Mat image);
uint8_t checkRegionExist(Mat image);
FixtureMessages euclidean(vector<Point2f> vectorIn, string target);
vector<Point2f> getTargetPoints(string target);
FixtureMessages writeTargetPoints(vector<Point2f> Points, string target);

