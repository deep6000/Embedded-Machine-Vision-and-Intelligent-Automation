    /**
 * @file Test.cpp
 * @author Deepesh Sonigra
 * @brief 
 * @version 0.1
 * @date 2019-06-26
 * 
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


/**
 * @brief  Find minimum value of the given array
 * 
 * @param arr 
 * @param size 
 * @return uint8_t 
 */
uint8_t findMin(uint8_t arr[], uint8_t size);

/**
 * @brief  Find Maximum value of given array
 * 
 * @param arr 
 * @param size 
 * @return uint8_t 
 */
uint8_t findMax(uint8_t arr[], uint8_t size);




int main()
{
    uint8_t pixel_val[8];
    uint8_t pixel_val_ij, MinVal, MaxVal;
    system("ffmpeg -i Dark-Room-Laser-Spot.mpeg -ss 00:00:07.000 -vframes 1  lazer_frame.png");
    Mat src_image = imread("lazer_frame.png",CV_LOAD_IMAGE_COLOR); //load  image
    Mat frame;
    Mat bgr[3];   //destination array

    split(src_image,bgr);//split source  

    // OpenCV uses BGR color order
    imwrite("green.png",bgr[1]); //green channel

    Mat g_band  = imread("green.png",CV_LOAD_IMAGE_GRAYSCALE);
    frame = g_band;
    for(int i = 1; i < g_band.rows; i++)
    {
        for (int j=1 ; j < g_band.cols; j++)
        {
            // 1 channel Mat Image  
            pixel_val[0] = g_band.at<uchar>(i-1,j);
            pixel_val[1] = g_band.at<uchar>(i+1,j);
            pixel_val[2] = g_band.at<uchar>(i-1,j+1);
            pixel_val[3] = g_band.at<uchar>(i+1,j+1);
            pixel_val[4] = g_band.at<uchar>(i-1,j-1);
            pixel_val[5] = g_band.at<uchar>(i+1,j-1);
            pixel_val[6] = g_band.at<uchar>(i,j+1);
            pixel_val[7] = g_band.at<uchar>(i,j-1);
            pixel_val_ij = g_band.at<uchar>(i,j); 

            MinVal = findMin(pixel_val,8);
            MaxVal = findMax(pixel_val,8);

            if(pixel_val_ij < MinVal)
            {
                printf("Value changed to Min at i= %d j =%d\n",i,j);
                frame.at<uchar>(i,j) = MinVal;
            }
            else if(pixel_val_ij > MaxVal)
            {
                frame.at<uchar>(i,j) = MaxVal;
                printf("Value changed to Max at i= %d j =%d\n",i,j);
            }
            //else
                //   frame.at<uchar>(i,j) = pixel_val_ij;           
        } 

    }
	imwrite("Median.png",frame);

}

uint8_t findMin(uint8_t arr[], uint8_t size)
{
    int min_val = arr[0];
    for(int i = 0; i< size; i++)
    {
        if(arr[i] < min_val)
            min_val = arr[i];
    }
            return min_val;
}


uint8_t findMax(uint8_t arr[], uint8_t size)
{
    int max_val = arr[0];
    for(int i = 0; i< size; i++)
    {
        if(arr[i] > max_val)
            max_val = arr[i];
    }
            return max_val;
}
