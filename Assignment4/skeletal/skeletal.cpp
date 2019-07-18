/**
 * @file skeletal.cpp
 * @author Deepesh Sonigra
 * @brief Displaying the skeletal view of the live camera
 * @version 0.1
 * @date 2019-07-16
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>

using namespace cv;
using namespace std;

/**
 * @brief Helper function
 * 
 */
void help();

/**
 * @brief Calculate histogram and return max count point
 * 
 * @param frame 
 * @return Point 
 */
Point CalculateHistogram(Mat frame);


/**
 * @brief Main Function
 */
int main()
{
    char image [15];
    int framecount;
    /// Establish the number of bins
    Mat frame, hist,gray,binary,mfblur;
    Point hist_count;

    VideoCapture cap(0); // open the default camera
    //get frame size to encode output video of same size
    Size framesize = Size((int)cap.get(CV_CAP_PROP_FRAME_WIDTH), (int)cap.get(CV_CAP_PROP_FRAME_HEIGHT));
	
	  //Set Output Video Parameters
    VideoWriter video;
    video.open("Output.mp4",CV_FOURCC('M','P','4','V') , (cap.get(CV_CAP_PROP_FPS)), framesize, true);
    
    // check if camera is opened 
    if(!cap.isOpened())
    {
      cout << "Error opening output video stream or file" << endl;  
      return -1;
    }
    
    // check if the output video is opened to write
    if(!video.isOpened())
	  {
      cout << "Error opening output video stream or file" << endl;
		  return -1;
    }
    
    framecount = 0;

    while(1)
    {
      //Capture the Frame
      
      cap >> frame;
      if(frame.empty())
      {
          break;
      }
      framecount ++;
      Mat skel(frame.size(), CV_8UC1, Scalar(0));
      Mat Output;
  
      //convert frame to grayscale
      cvtColor(frame, gray, CV_BGR2GRAY);
      // Calculate Histogram
      hist_count = CalculateHistogram(gray);
      // Thresholding the frame
      threshold(gray, binary, hist_count.y, 255, CV_THRESH_BINARY);
      binary = 255 - binary;

      // To remove median filter, just replace blurr value with 1
      medianBlur(binary, mfblur, 1);

      // This section of code was adapted from the following post, which was based in turn on the Wikipedia description of a morphological skeleton
      // http://felix.abecassis.me/2011/09/opencv-morphological-skeleton/
      
      Mat temp, eroded;
      Mat element = getStructuringElement(MORPH_CROSS, Size(3, 3));
      bool done;
      int iterations=0;
      do
      {
        erode(mfblur, eroded, element);
        dilate(eroded, temp, element);
        subtract(mfblur, temp, temp);
        bitwise_or(skel, temp, skel);
        eroded.copyTo(mfblur);
        done = (countNonZero(mfblur) == 0);
        iterations++;
      
      } while (!done && (iterations < 100));
      
      // get image name every frame
      snprintf(image,15,"frame%04d.jpg",framecount);
      if(framecount <= 3000)
      {
    //  imwrite(image,skel);
      }
      cvtColor(skel, Output, CV_GRAY2BGR);
      imshow("skeleton", Output);
      video.write(Output);  
      int k=waitKey(2);
      if(k==27)
        break;
    }
} 


void help()
{
 cout << "\nThis program demonstrates a skeletal transform on the Live Camera\n"
         "Usage:\n"
         "./skeletal \n" << endl;
}


Point CalculateHistogram(Mat frame)
{
    const int histSize = 256;
    double maxVal=0;
    const float range[] = { 0, 256 } ;
    const float* histRange = { range };
    bool uniform = true; bool accumulate = false;
    Point max;
    Mat hist;
    calcHist( &frame, 1, 0, Mat(), hist, 1, &histSize, &histRange, uniform, accumulate );
    minMaxLoc(hist, 0, &maxVal, 0, &max);
    return max;
}
