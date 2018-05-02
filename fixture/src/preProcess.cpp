#include "preProcess.hpp"

uint8_t checkRegionExist(Mat image)
{
	for(int i = 0 ;i < image.rows; i++)
	{
        	for(int j = 0; j< image.cols; j++)
		{
                	Scalar value = image.at<uchar>(i,j);
                    	if(value.val[0] > 0)
			{
				//imwrite("whiteRegions.jpg",image);
                    		return 1;
                    	}
                }

        }
	return 0;
}


vector<Point2f> getTargetPoints(string target)
{
        char * pEnd;
        string line;
	vector <Point2f> pointTarget(1);
        size_t position;
        char * targetFound =  NULL;
        ifstream infile("calibration.txt");
        while( getline(infile, line))
        {
                position = line.find(target);
                if( position < line.length())
                {
                        string cxValue = line.substr(position + target.length()+3, line.length());
			char * pEnd;
  			float f1, f2;
  			//f1 = strtof (cxValue.c_str(), &pEnd);
  			//f2 = strtof (pEnd, NULL);
			pointTarget[0] = Point2f(strtof(cxValue.c_str(), &pEnd), strtof(pEnd, NULL));
			return pointTarget;
                }
        }
	pointTarget[0] = Point2f(0.0,0.0);
        return pointTarget;
}

FixtureMessages euclidean(vector<Point2f> vectorIn, string target)
{
	float dist;
	uint8_t nonZeroFlag = 0, nonZero = 0;
	vector<Point2f> vectorTarget;
	if (target.empty())
		return EMPTY_STRING_OPTION;
	vectorTarget = getTargetPoints(target);

	if (vectorTarget[0].x == 0 && vectorTarget[0].y == 0)
		return CALIBRATION_NOT_FOUND;
	for (int i = 0; i<vectorIn.size();i++)
	{
		if (vectorIn[i].x > 0 && vectorIn[i].y > 0)
		{
			nonZeroFlag++;
			nonZero = i;
		}
	}

	if (nonZeroFlag == 1)
	{
		dist = sqrt(pow(vectorIn[nonZero].x - vectorTarget[0].x, 2) + pow(vectorIn[nonZero].y - vectorTarget[0].y, 2));
	}
	else if(nonZeroFlag == 0)
	{
		return NO_LEDS_FOUND;
	}
	else if (nonZeroFlag > 1)
	{
		return MORE_THAN_ONE_LED_FOUND;
	}
	if (dist < EPSILON)
		return SUCCESS;
	return FAILED;
}


Mat imagePreProcessing(Mat  image)
{
	Mat element = getStructuringElement( MORPH_ELLIPSE, Size(7, 7), Point( 1, 1) ), element2 = getStructuringElement( MORPH_ELLIPSE, Size(2,2), Point( 1, 1) ), element3 = getStructuringElement( MORPH_ELLIPSE, Size(12, 8), Point( 1, 1) );
	Mat grayImage, Binary, binaryInverted, finalImage, whiteImage = image;
	vector<Mat> channels(3), channels2(3);
	Mat Geq;
	//image = createImageMask(image);
	split(image, channels);
	//imshow("O_Image", whiteImage);
	//if (waitKey(30) >= 0)
	//	return element;


	equalizeHist(channels[0], channels[0]);
	equalizeHist(channels[1], channels[1]);
	equalizeHist(channels[2], channels[2]);

	threshold( channels[0], channels[0], THRESHOLD_VALUE_CHANNELS, THRESHOLD_MAX_VALUE, 1);
	threshold( channels[1], channels[1], THRESHOLD_VALUE, THRESHOLD_MAX_VALUE, 0);
	threshold( channels[2], channels[2], THRESHOLD_VALUE_CHANNELS, THRESHOLD_MAX_VALUE, 1);
	merge(channels, whiteImage);
	imwrite("Leds_on.jpg", whiteImage);

	morphologyEx( channels[1], channels[1], 2, element);
	/*channels[1] = channels[1] & channels[0];
	morphologyEx( channels[1], channels[1], 2, element2);
	morphologyEx( channels[1], channels[1], 1, element);*/


	//imshow("R_Image", channels[0]);
	//if (waitKey(30) >= 0)
	//return element;
	//imshow("G_Image", channels[1]);
	//if (waitKey(30) >= 0)
		//return element;
	return channels[1];
}



vector<Vec3f> houghTransform(Mat image)
{

	vector<Vec3f> circles;
	HoughCircles(image, circles, CV_HOUGH_GRADIENT,1,1,10,10,0,0);
	return circles;
}


Mat createImageMask(Mat image)
{
	Mat C = Mat::ones(image.rows, image.cols, CV_8UC1);
	C = C*255;
	vector<Mat> channels(3);
	split(image, channels);

	for (int i = 0; i < image.rows;i++)
	{
			if (i<UP_IMAGE_LIMIT || i>LOW_IMAGE_LIMIT)
			{
	//				C.row(i) << 0;
			}
	}
	channels[0] = channels[0] & C;
	channels[1] = channels[1] & C;
	channels[2] = channels[2] & C;
	///merge(channels, image);
	return image;
}

vector<Point2f>  getMoments(Mat image)
{
	vector<vector<Point> > contours;
	/// Detect edges using canny
  	Canny( image, image, 100, 100*2, 3 );
  	/// Find contours
  	findContours(image, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
  	/// Get the moments
  	vector<Moments> mu(contours.size() );
  	for( int i = 0; i < contours.size(); i++ )
     	{
		mu[i] = moments( contours[i], false );
	}

  	///  Get the mass centers:
  	vector<Point2f> mc( contours.size() );
	vector<Point2f> centers(contours.size());
  	for( int i = 0; i < contours.size(); i++ )
     	{
		mc[i] = Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 );
	}

  	/// Draw contours
  	Mat drawing = Mat::zeros( image.size(), CV_8UC3 );
	int j = 0;
	cout<<"SIZE:  "<< contours.size()<<endl;
  	for( int i = 0; i< contours.size(); i++ )
     	{
       		Point center(cvRound(mc[i].x), cvRound(mc[i].y));
                int radius = cvRound(5);
		//cout<<contourArea(contours[i])<<endl;
		if((contourArea(contours[i]) >= AREA_THRESHOLD) && (contourArea(contours[i]) <= AREA_UPPER_THRESHOLD))
		{
		//	cout<<contourArea(contours[i])<<endl;
                	circle(drawing, center, radius, Scalar(255,255,255),1,8,0);
			centers[j] = Point2f(mc[i].x, mc[i].y);
			j++;
		}
		else
		{
			centers[j] = Point2f(0,0);
			j++;
		}
	}
	imwrite("imageContours.jpg",drawing);
	return centers;
}
