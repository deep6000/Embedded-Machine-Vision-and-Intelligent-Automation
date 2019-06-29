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
    Mat frame, bgr[3];
    char k;
    unsigned int i = 0;
  
    vector<Mat> channels;
    // @reference https://www.learnopencv.com/read-write-and-display-a-video-using-opencv-cpp-python/
    // Create a VideoCapture object and open the input file
    VideoCapture cap("Dark-Room-Laser-Spot.mpeg");

    // Check if camera opened successfully
    if(!cap.isOpened())
    {
        cout << "Error opening video stream or file" << endl;
        return -1;
    }
    while(1)
    {
        cap >> frame;
        if(frame.empty())
            break;
        split(frame,bgr);
        vector<int> params;
        params.push_back(CV_IMWRITE_PXM_BINARY);
        params.push_back(0); // 1 for binary format, 0 for ascii format
        string filename = "frame" + to_string(++i) + ".pgm";
        imwrite(filename,bgr[1],params);   
        
    }
    cap.release();
  
    // Combine all Images into a video
    system("ffmpeg -start_number 1 -i frame%d.pgm -vcodec mpeg4 Output.avi");
    return 0;    


}