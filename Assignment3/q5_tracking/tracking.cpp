/**
 * @file tracking.cpp
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

#define THRESHOLD           (180)
using namespace std;
using namespace cv;
/**
 * @brief Cordinates for the spot
 */
typedef struct Cordinates
{
    int xleft;
    int xright;
    int ytop;
    int ybottom;
}Cordinates;
/**
 * @brief Spot Centre of mass cordinates
 */
typedef struct COM_pts
{
     int xbar;
     int ybar;
    bool spot;
}COM_pts;



/*
 * @brief Find Centre of mass
 * 
 * @param frame 
 * @param area 
 * @return COM_pts 
 */
COM_pts FindCOM(Mat frame, Cordinates *area);

/**
 * @brief Get the Xleft object
 * 
 * @param curr_left 
 * @param pt 
 * @return int 
 */

int getXleft( int curr_left ,  int pt);

/**
 * @brief Get the Xright object
 * 
 * @param curr_right 
 * @param pt 
 * @return int 
 */
 int getXright( int curr_right ,  int pt);
/**
 * @brief Get the Ytop object
 * 
 * @param curr_top 
 * @param pt 
 * @return int 
 */

 int getYtop( int curr_top ,  int pt);
/**
 * @brief Get the Ybottom object
 * 
 * @param curr_bottom 
 * @param pt 
 * @return int 
 */

 int getYbottom( int curr_bottom ,  int pt);



int main()
{
    Mat frame, bgr[3], g_band,final;
    char k;
    int framecount;
    unsigned int i = 0;
    Cordinates spot;
    COM_pts COM;
    Scalar white(255,255,255);
    // @reference https://www.learnopencv.com/read-write-and-display-a-video-using-opencv-cpp-python/
    // Create a VideoCapture object and open the input file
    VideoCapture cap("Dark-Room-Laser-Spot.mpeg");

    Size framesize = Size((int)cap.get(CV_CAP_PROP_FRAME_WIDTH), (int)cap.get(CV_CAP_PROP_FRAME_HEIGHT));
	
	//Set Output Video Parameters
    VideoWriter video;
    video.open("Output.mp4",CV_FOURCC('M','P','4','V') , (cap.get(CV_CAP_PROP_FPS)), framesize, true);
	
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
    framecount = 0;
    while(1)
    {        
     //storing th captured frame
        
        cap >> frame;
        framecount++;
        spot.xleft = frame.cols;
        spot.xright = 0;
        spot.ytop = frame.rows;
        spot.ybottom = 0;

        // Checking if we reached at the end of the video
        if(frame.empty())
            break;
        // Spliting the frame into 3 channels     
        split(frame,bgr);
        g_band = bgr[1];
        medianBlur(g_band, g_band, 3);			

        COM = FindCOM(g_band,&spot);
          
    
        merge(bgr,3,final);
        if(COM.spot == true)
        {
            line(final, Point(0, COM.ybar), Point(final.cols, COM.ybar), white, 1, 8, 0);
            line(final, Point(COM.xbar, 0), Point(COM.xbar,final.rows), white, 1, 8, 0); 
        }
       // imshow("OUTPUT",final);
        video.write(final);  
        k=waitKey(2);
	    if(k==27)
		    break;
    }
    cap.release();
    video.release();
    return 0;    

}


COM_pts FindCOM(Mat frame, Cordinates *area)
{
    COM_pts COM;
    memset(&COM, 0 , sizeof(COM));
    
    for(int row = 1; row < frame.rows; row++)
    {

        for (int col=1 ; col < frame.cols; col++)
        {
                if(frame.at<uchar>(row,col) > THRESHOLD)
                {
                   
                    area->xleft = getXleft(area->xleft, col);
                    area->xright = getXright(area->xright,col);            
                    area->ytop = getYtop(area->ytop, row);
                    area->ybottom = getYbottom(area->ybottom,row);
                    COM.spot = true;
                }
                
        }
         
    }
    //Formula referred from Professor Sam Siewert Lecture 5 Part 2
    COM.ybar = ((area->ybottom - area->ytop)/2) + area->ybottom;
    COM.xbar = ((area->xright - area->xleft)/2) + area->xleft;
 
                     
    return COM;
}

int getXleft(int curr_left ,  int pt)
{
    if(curr_left > pt)
        return pt;
    else
        return curr_left;
    
}

int getXright(int curr_right , int pt)
{
    if(curr_right < pt)
        return pt;
    else
        return curr_right;
    
}

int getYtop(int curr_top ,  int pt)
{
    if(curr_top > pt)
        return pt;
    else
        return curr_top;
    
}
int getYbottom(int curr_bottom , int pt)
{
    if(curr_bottom < pt)
        return pt;
    else
        return curr_bottom;
    
}
