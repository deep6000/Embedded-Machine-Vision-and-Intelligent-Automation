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
#include  "opencv2/core/core.hpp"
#include <iostream>
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <sched.h>

using namespace cv;
using namespace std;
#define NUM_OF_THREADS 			(2)

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
 * @brief 
 * 
 * @param threadptr 
 * @return void* 
 */
void *skeletal(void * threadptr);

int write_frame;
pthread_t threads[NUM_OF_THREADS];

typedef struct threadarg
{
  Mat frame;
  int frameNumber;
}threadarg;

threadarg arg_thread[NUM_OF_THREADS];
/**
 * @brief Main Function
 */
int main()
{
    char image [15];
    int framecount;
    unsigned int time_passed;
    timespec start, finish;
    
    void *status;
    /// Establish the number of bins
    Mat frame[NUM_OF_THREADS];
    cvNamedWindow("Skeleton Example", CV_WINDOW_AUTOSIZE);
    CvCapture* capture = cvCreateCameraCapture(0);
    IplImage* frame_cap[NUM_OF_THREADS];
    framecount = 0;
 // check if camera is opened 
  
    clock_gettime(CLOCK_REALTIME, &start);
    while(1)
    {
      clock_gettime(CLOCK_REALTIME, &finish);
      time_passed= (double)(finish.tv_sec - start.tv_sec);
	
      if(time_passed > 100 || framecount > 3000)
      break;

      for(int i=0;i<NUM_OF_THREADS;i++)
      {
      	frame_cap[i]=cvQueryFrame(capture);
	framecount++;	
	frame[i] = cvarrToMat(frame_cap[i]);
      	arg_thread[i].frameNumber = framecount;    
      	arg_thread[i].frame = frame[i];	
      }


      for(int i=0; i<NUM_OF_THREADS; i++)
      {
     	 pthread_create(&threads[i], NULL , skeletal, (void *)&arg_thread[i]);
      }
     for(int i=0; i<NUM_OF_THREADS; i++)
     {

            //printf("join thread_idx=%d\n", thread_idx);    

            if((pthread_join(threads[i], &status)) < 0);	
     }
	
    }
    cout<<"framecount "<<framecount<<endl;

system("ffmpeg -i frame%04d.jpg -vcodec mpeg4 Output.mp4");
   
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

void *skeletal(void * threadptr)
{
 
    Point hist_count;
     char image [15];
     int frameNumber;
    Mat frame, hist,gray,binary,mfblur;
    threadarg arg = *((threadarg*)threadptr);
    
    frame = arg.frame;
    frameNumber = arg.frameNumber;
    cvtColor(frame, gray, CV_BGR2GRAY);
      // Calculate Histogram
     // hist_count = CalculateHistogram(gray);
      // Thresholding the frame
      threshold(gray, binary, 100, 255, CV_THRESH_BINARY);
      binary = 255 - binary;
      
      // To remove median filter, just replace blurr value with 1
      medianBlur(binary, mfblur, 1);
     
    Mat element = getStructuringElement(MORPH_CROSS, Size(3, 3));
    Mat temp, eroded;
    Mat skel(frame.size(),CV_8UC1,Scalar(0));
   
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

       snprintf(image,15,"frame%04d.jpg",frameNumber);
     
      imwrite(image,skel);
      pthread_exit((void **)0);

}
