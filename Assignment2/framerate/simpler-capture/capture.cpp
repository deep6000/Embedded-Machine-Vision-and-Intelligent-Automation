/*
 *
 *  Example by Sam Siewert 
 *
 */
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sys/time.h>
#include<syslog.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#define TIME_TO_WAIT      (60)
#define NANOSEC           (1000000000)

using namespace cv;
using namespace std;

void frame_analysis(unsigned int time);

typedef struct timespec timepec;

timespec frame_start, frame_end;
timespec start, finish;

int main( int argc, char** argv )
{
    double time_per_frame;
    unsigned int time_passed;

    cvNamedWindow("Capture Example", CV_WINDOW_AUTOSIZE);
    CvCapture* capture = cvCreateCameraCapture(0);
    IplImage* frame;

    clock_gettime(CLOCK_REALTIME, &start);
    while(1)
    {
         
            // get time before capturing frame
            clock_gettime(CLOCK_REALTIME, &frame_start);
            // get frame
            frame=cvQueryFrame(capture);
            // get time after capturing frame
            clock_gettime(CLOCK_REALTIME, &frame_end);
            // get time for each frame

            clock_gettime(CLOCK_REALTIME, &finish);
            time_passed= (double)(finish.tv_sec - start.tv_sec);
            frame_analysis(time_passed);

            // one timeout reset the start time
            if(time_passed > TIME_TO_WAIT )
            {
                time_passed = 0;
                clock_gettime(CLOCK_REALTIME, &start);
            }
        
        if(!frame) break;

        cvShowImage("Capture Example", frame);

        char c = cvWaitKey(33);
        if( c == 27 ) break;
    }

    cvReleaseCapture(&capture);
    cvDestroyWindow("Capture Example");
    
};


void frame_analysis(unsigned int time)
{
    unsigned int worst_rate_sec, frame_per_second;
    static unsigned int frame_count;
    static double worst_time;
    int worst_frame_per_second;
    static double frame_time, total_time;
    double jitter_time= 0;
    
    if(time > TIME_TO_WAIT)
    {
        frame_per_second = frame_count / TIME_TO_WAIT;
	syslog(LOG_CRIT,"Average Frame Execution time : %d nsecs\n ",(int)(total_time/frame_count));
	syslog(LOG_CRIT,"Average Frame rate : %d fps\n ",frame_per_second);
	syslog(LOG_CRIT, "worst time is %lf nsecs\n", worst_time);
	worst_frame_per_second = (NANOSEC/worst_time) ;
	syslog(LOG_CRIT, "Worst Frame Rate : %d \n", (int)worst_frame_per_second/60);
 	jitter_time = (1800 - frame_count)*(int)(total_time/frame_count);
	syslog(LOG_CRIT, "Jitter is %lf nsecs\n ", jitter_time );
	
        //reset worst time 
        worst_time = 0;
        frame_count = 0;
    }
    else
    {
        frame_time = ((double)(frame_end.tv_sec*NANOSEC+ frame_end.tv_nsec) - (frame_start.tv_sec*NANOSEC +frame_start.tv_nsec));
        total_time += frame_time ; 
       // syslog(LOG_CRIT,"Iteration time is %lf\n",frame_time);  
        frame_count++;
        
        if (worst_time < frame_time)
            {
                worst_time = frame_time;
                worst_rate_sec = time;
	
            }
    
    }
      
     

}
