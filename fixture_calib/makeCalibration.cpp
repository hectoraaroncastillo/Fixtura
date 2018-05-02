#include "src/preProcess.hpp"

int main(int argc, char** argv)
{
	uint8_t regionRecognized = 0, counter = 0;
	FixtureMessages result;
    	VideoCapture camera(0);   ///*** Open the camera, index indicates the number of device.
	camera.set(CV_CAP_PROP_FRAME_WIDTH,320);
	camera.set(CV_CAP_PROP_FRAME_HEIGHT,240);
	Mat cameraFrame, originalFrame;
	vector<Point2f>  centers;
	if (!camera.isOpened())
	{
		cout<< "Can't open the camera"<<endl;
		return CANT_OPEN_CAMERA;
	}

	while (!regionRecognized && counter <= 5)
	{
		//cout<< "Processing ...."<<endl;
		camera.read(cameraFrame);
		cameraFrame = imagePreProcessing(cameraFrame);
		regionRecognized = checkRegionExist(cameraFrame);
		if (regionRecognized)
		{
			centers = getMoments(cameraFrame);
			//cout<<centers[0]<<endl;
			//imwrite("Image.jpg", originalFrame);
			result = writeTargetPoints(centers, argv[1]);
			//cout<<"program finished"<<endl;
			cout<<result<<endl;
		}
		counter++;
		if (counter > 5)
		{
			result = NO_LEDS_FOUND;
		}
	}
	return result;

}


