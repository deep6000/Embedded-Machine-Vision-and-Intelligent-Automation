#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <syslog.h>

using namespace cv;
using namespace std;

#define TIME_TO_WAIT      (60)
#define NANOSEC           (1000000000)
#define SOBEL 		   (1)
#define CANNY		   (2)


void frame_analysis(unsigned int time);
void CannyThreshold(int, void*);

char* window_name = "Edge Detection";


Mat src, src_gray;
Mat dst, detected_edges;
Mat grad; 
int edgeThresh = 1;
int lowThreshold;
int const max_lowThreshold = 100;
int ratio = 3;
int kernel_size = 3;
int scale = 1;
int delta = 0;
int ddepth = CV_16S;
int c;


timespec frame_start, frame_end;
timespec start, finish;


 /* @function CannyThreshold
 * @brief Trackbar callback - Canny thresholds input with a ratio 1:3
 */
void CannyThreshold(int, void*)
{
  /// Reduce noise with a kernel 3x3
  blur( src_gray, detected_edges, Size(3,3) );

  /// Canny detector
  Canny( detected_edges, detected_edges, lowThreshold, lowThreshold*ratio, kernel_size );

  /// Using Canny's output as a mask, we display our result
  dst = Scalar::all(0);

  src.copyTo( dst, detected_edges);
  imshow( window_name, dst );
}

/** @function main */
int main( int argc, char** argv )
{
double time_per_frame;
    unsigned int time_passed;
    char input;
    unsigned int choice;
   
    printf("Initailizing Edge transformation\n");
    printf("Menu :\n Press (c) for Canny\n Press (s) for Sobel\n");	
     scanf(" %c",&input);

	switch(input)
	{
		case 's' :
			printf("Choice selected : SOBEL\n");
			choice = SOBEL;
		break;
	
		case 'c' :
			printf("Choice selected : CANNY\n");
			choice = CANNY;
		break;
		default:
			printf("Invalid selection\n Choice Default : SOBEL\n");
			choice = SOBEL;	
	}
		

//    cvNamedWindow("Capelse if(choice == SOBEL)    
    {  
        if( !src.data )  
            {   
            return -1;   
        }  
  
        GaussianBlur( src, src, Size(3,3), 0, 0, BORDER_DEFAULT );  
      
        /// Convert it to gray  
        cvtColor( src, src_gray, CV_RGB2GRAY );  
  
        /// Create window  
        namedWindow( window_name, CV_WINDOW_AUTOSIZE );  
  
        /// Generate grad_x and grad_y  
        Mat grad_x, grad_y;  
        Mat abs_grad_x, abs_grad_y;  
   
        /// Gradient X  
        //Scharr( src_gray, grad_x, ddepth, 1, 0, scale, delta, BORDER_DEFAULT );  
        Sobel( src_gray, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT );     
        convertScaleAbs( grad_x, abs_grad_x );  
  
        /// Gradient Y   
        //Scharr( src_gray, grad_y, ddepth, 0, 1, scale, delta, BORDER_DEFAULT );  
        Sobel( src_gray, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT );     
        convertScaleAbs( grad_y, abs_grad_y );  
  
        /// Total Gradient (approximate)  
        addWeighted( abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad );  
  
        imshow( window_name, grad );  
        char b = waitKey(33);  
  
        /*Execute Canny for 'c' or 'C'*/  
        if(b == 67 || b == 99)  
        {  
            choice = CANNY;  
            printf("choice selected : CANNY\n");  
        }  
        if(b == 27) break;  
    }  ture Example", CV_WINDOW_AUTOSIZE);
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
	    src = cvarrToMat(frame);
           
	if(choice == CANNY)
	{
		if( !src.data )
  		{ 
			return -1; 
		}

  		/// Create a matrix of the same type and size as src (for dst)
  		dst.create( src.size(), src.type() );

  		/// Convert the image to grayscale
  		cvtColor( src, src_gray, CV_BGR2GRAY );

  		/// Create a window
  		namedWindow( window_name, CV_WINDOW_AUTOSIZE );

  		/// Create a Trackbar for user to enter threshold
  		createTrackbar( "Min Threshold:", window_name, &lowThreshold, max_lowThreshold, CannyThreshold );

  		/// Show the image
  		CannyThreshold(0, 0);

  		/// Wait until user exit program by pressing a key
  		char b = waitKey(33);

		/*Execute Sobel for 's' or 'S'*/
  		if(b == 83 || b == 115)	
		{
			printf("Choice Selected : Sobel\n");
			choice = SOBEL;
		}
		if(b == 27) break;
	}
	else if(choice == SOBEL)  
	{
  		if( !src.data )
    		{ 
			return -1; 
		}else if(choice == SOBEL)    
    {  
        if( !src.data )  
            {   
            return -1;   
        }  
  
        GaussianBlur( src, src, Size(3,3), 0, 0, BORDER_DEFAULT );  
      
        /// Convert it to gray  
        cvtColor( src, src_gray, CV_RGB2GRAY );  
  
        /// Create window  
        namedWindow( window_name, CV_WINDOW_AUTOSIZE );  
  
        /// Generate grad_x and grad_y  
        Mat grad_x, grad_y;  
        Mat abs_grad_x, abs_grad_y;  
   
        /// Gradient X  
        //Scharr( src_gray, grad_x, ddepth, 1, 0, scale, delta, BORDER_DEFAULT );  
        Sobel( src_gray, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT );     
        convertScaleAbs( grad_x, abs_grad_x );  
  
        /// Gradient Y   
        //Scharr( src_gray, grad_y, ddepth, 0, 1, scale, delta, BORDER_DEFAULT );  
        Sobel( src_gray, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT );     
        convertScaleAbs( grad_y, abs_grad_y );  
  
        /// Total Gradient (approximate)  
        addWeighted( abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad );  
  
        imshow( window_name, grad );  
        char b = waitKey(33);  
  
        /*Execute Canny for 'c' or 'C'*/  
        if(b == 67 || b == 99)  
        {  
            choice = CANNY;  
            printf("choice selected : CANNY\n");  
        }  
        if(b == 27) break;  
    }  

  		GaussianBlur( src, src, Size(3,3), 0, 0, BORDER_DEFAULT );
	
 		/// Convert it to gray
  		cvtColor( src, src_gray, CV_RGB2GRAY );

  		/// Create window
  		namedWindow( window_name, CV_WINDOW_AUTOSIZE );

  		/// Generate grad_x and grad_y
  		Mat grad_x, grad_y;
  		Mat abs_grad_x, abs_grad_y;
 
  		/// Gradient X
  		//Scharr( src_gray, grad_x, ddepth, 1, 0, scale, delta, BORDER_DEFAULT );
  		Sobel( src_gray, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT );   
  		convertScaleAbs( grad_x, abs_grad_x );

  		/// Gradient Y 
  		//Scharr( src_gray, grad_y, ddepth, 0, 1, scale, delta, BORDER_DEFAULT );
  		Sobel( src_gray, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT );   
  		convertScaleAbs( grad_y, abs_grad_y );

  		/// Total Gradient (approximate)
  		addWeighted( abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad );

  		imshow( window_name, grad );
  		char b = waitKey(33);

		/*Execute Canny for 'c' or 'C'*/
  		if(b == 67 || b == 99)
		{
			choice = CANNY;
			printf("choice selected : CANNY\n");
		}
		if(b == 27) break;
	}
            clock_gettime(CLOCK_REALTIME, &finish);
            time_passed= (double)(finish.tv_sec - start.tv_sec);
            frame_analysis(time_passed);
	// one timeout reset the start time
            if(time_passed > TIME_TO_WAIT )
            {
                time_passed = 0;
                clock_gettime(CLOCK_REALTIME, &start);
            }
}
return 0;
}

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
        total_time += frame_time; 
       // syslog(LOG_CRIT,"Iteration time is %lf\n",frame_time);  
        frame_count++;
        
        if (worst_time < frame_time)
            {
                worst_time = frame_time;
                worst_rate_sec = time;
	
            }
    
    }
      
     
}

