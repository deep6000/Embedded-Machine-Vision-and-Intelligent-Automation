/**
 * @file Diff_test.cpp
 * @author Deepesh Sonigra
 * @brief 
 * @version 0.1
 * @date 2019-06-26
 * @reference https://www.learnopencv.com/read-write-and-display-a-video-using-opencv-cpp-python/
 * @copyright Copyright (c) 2019
 * 
 */

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/background_segm.hpp>
#include "opencv2/opencv.hpp"
#include <iostream>
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/core/core.hpp>
#include <string>
#include <stdio.h>
#include <opencv/highgui.h>
#include <opencv2/video/background_segm.hpp>

using namespace std;
using namespace cv;


int main()
{

    Mat previous , current, difference;
    Mat bgr_prev[3],bgr_curr[3], bgr_diff[3];
    Mat final(Size(1920, 1280), CV_8UC3);
    char k;
    unsigned int framecount = 0;
    vector<Mat> channels;
    // @reference https://www.learnopencv.com/read-write-and-display-a-video-using-opencv-cpp-python/
    // Create a VideoCapture object and open the input file
    VideoCapture cap("Dark-Room-Laser-Spot-with-Clutter.mpeg");

    Size framesize = Size((int)cap.get(CV_CAP_PROP_FRAME_WIDTH), (int)cap.get(CV_CAP_PROP_FRAME_HEIGHT));
	
	//Set Output Video Parameters
    VideoWriter video;
    video.open("Output.mp4",CV_FOURCC('M','P','4','2') , (cap.get(CV_CAP_PROP_FPS))/2, framesize, true);
	
    if(!video.isOpened())
	{
        cout << "Error opening output video stream or file" << endl;
		return -1;
    }
   
    // Check if camera opened successfully
    if(!cap.isOpened())
    {
        cout << "Error opening video stream or file" << endl;
        return -1;
    }
    while(1)
    {
        cap >> previous;
        
        if(previous.empty())
            break;
        
        split(previous,bgr_prev);
        cap >> current;
        split(current,bgr_curr);
        
        absdiff(bgr_prev[0],bgr_curr[0],bgr_diff[0]);
        absdiff(bgr_prev[1],bgr_curr[1],bgr_diff[1]);
        absdiff(bgr_prev[2],bgr_curr[2],bgr_diff[2]);

        merge(bgr_diff,3,final);
     
        framecount++;   
       video.write(final);
       
        k=waitKey(2);
		if(k==27)
		break;


    }
    cap.release();
    video.release();
                
    return 0;    


}
