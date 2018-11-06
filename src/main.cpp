#include <opencv2/opencv.hpp>

#include "SGMStereo.h"
#include "GroundEstimator.h"
#include "RANSACPlane.h"
#include "Visualizer.h"

#include <iostream>
#include <time.h>
#include <iomanip>
#include <sstream>
#include "Config.h"

using namespace cv;
using namespace std;

int main()
{
	SGMStereo::Parameter param;
	param.mMaxDisp = 128;
	SGMStereo stereo(param);
	GroundEstimator::Parameter groundParam;
	RANSACPlane::Parameter RANSACParam;
	GroundEstimator groundEstimator(&groundParam, &param, &RANSACParam);
	Visualizer visualizer(&groundEstimator); 

	for (int i = Config::frameBegin; i < /*Config::frameEnd*/1100; i++)
	{
		stringstream leftFile, rightFile;
        leftFile << "/";
		leftFile << Config::imageDir << "/" << Config::imageBaseName[0] << setfill('0') << setw(Config::idxWidth) << i << Config::imageFormat;
		rightFile << Config::imageDir << "/" << Config::imageBaseName[1] << setfill('0') << setw(Config::idxWidth) << i << Config::imageFormat;

		char left[256];
		char right[256];
		sprintf(left,"E:\\WorkDirectory\\CLionProjects\\kitti\\01\\left\\%06d.png",i);
        sprintf(right,"E:\\WorkDirectory\\CLionProjects\\kitti\\01\\right\\%06d.png",i);

		Mat leftImg = imread(/*leftFile.str()*/left, CV_LOAD_IMAGE_GRAYSCALE);
		Mat rightImg = imread(/*rightFile.str()*/right, CV_LOAD_IMAGE_GRAYSCALE);
		//cv::imshow("leftimage",leftImg);
		//cv::waitKey(0);

		clock_t beginTime, endTime;
		beginTime = clock();
		stereo.process(leftImg, rightImg);//对图像进行处理，得到视差图。输入的图像已经经过预处理--畸变矫正，共面行对准等？？存疑，or输入的是原始图像
		endTime = clock();
		cout << float(endTime - beginTime) / (float)CLOCKS_PER_SEC << endl;
		
		// in linux(gcc) this line is necessary in case of the "temporal address error"
		Mat disparity = stereo.getDisparity();
		groundEstimator.compute(&leftImg, &rightImg, &disparity);

		imshow("left", leftImg);
		imshow("disp", stereo.getDisparity());
		imshow("ground mask",visualizer.showGroundWithImage());
		
		waitKey(1);
	}

	system("pause");

	return 0;
}