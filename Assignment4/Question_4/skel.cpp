#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <string>
#include <stdio.h>

#include <iostream>
using namespace cv;
using namespace std;




int main(int argc,char** argv)
{
	cvNamedWindow("Skeleton Example", CV_WINDOW_AUTOSIZE);
        CvCapture* capture = cvCreateCameraCapture(0);
	IplImage* frame_cap;
	Mat original, grayscale, binary, previous,delta;
 	unsigned int time_passed;
    	timespec start, finish;

	  //Set Output Video Parameters
    	VideoWriter video;
    	video.open("Output.avi",CV_FOURCC('M','P','4','V') ,15, Size(640,480), true);
   

  // check if the output video is opened to write
    if(!video.isOpened())
	  {
      cout << "Error opening output video stream or file" << endl;
		  return -1;
    }
	//original = imread(argv[1]);
	//VideoCapture cap(0);	
	frame_cap=cvQueryFrame(capture);
	previous =  cvarrToMat(frame_cap);
	//cap >> previous;	
	int count = 0;
  	clock_gettime(CLOCK_REALTIME, &start);
	while(1)
	{	
		clock_gettime(CLOCK_REALTIME, &finish);
         	time_passed= (double)(finish.tv_sec - start.tv_sec);
	
      		if(time_passed > 100)
      			break;
		frame_cap=cvQueryFrame(capture);
		original =  cvarrToMat(frame_cap);
	//	cap >> original;		
		if(original.empty())
		{
			cout<<" Can not open camera\n";
			return -1;
		}
		delta = original - previous;
		previous = original.clone();	
		
		cvtColor(delta, grayscale, CV_BGR2GRAY);
		
		threshold(grayscale, binary,40,255,CV_THRESH_BINARY);
		
		int i,j;
		medianBlur(binary, binary, 1);

		int chi, sigma, chi1;
		int A0, A1,A2, A3, A4, A5, A6, A7, A8;	//pixels
		bool done;
     	int iterations=0;
		do
		{
			done = true;
			for(i = 1; i < binary.rows-1; i++)
			{
				for(j = 1; j < binary.cols-1; j++)
				{
					sigma=chi=chi1 =0;
					
					//get scaled values of pixels
					A0 = binary.at<uchar>(i,j)>>7;
					A1 = binary.at<uchar>(i,j+1)>>7;
					A2 = binary.at<uchar>(i-1,j+1)>>7;
					A3 = binary.at<uchar>(i-1,j)>>7;
					A4 = binary.at<uchar>(i-1,j-1)>>7;
					A5 = binary.at<uchar>(i,j-1)>>7;
					A6 = binary.at<uchar>(i+1,j-1)>>7;
					A7 = binary.at<uchar>(i+1,j)>>7;
					A8 = binary.at<uchar>(i+1,j+1)>>7;

					sigma = A1 + A2 + A3+ A4+ A5+ A6+ A7+ A8;
					sigma = sigma>>7;

					chi = (int)(A1!=A3) + (int)(A3!=A5) + (int)(A5!=A7) + (int)(A7!=A1) + 2*((int)((A2>A1) && (A2> A3))+ (int)((A4>A3) && (A4> A5))+  (int)((A6>A5) && (A6> A7))+(int)((A8>A7) && (A8> A1)));

				if((A0==1) && (A3==0) && (A7 ==1) && (chi == 2) && (sigma != 1))
					{
						binary.at<uchar>(i,j) = 0;
						done = false;
					} 
				  else if((A0==1) && (A3==1) && (A7 ==0) && (chi == 2) && (sigma != 1)) 
					{
						binary.at<uchar>(i,j) = 0;
						done = false;
					}
				else if((A0==1) && (A1==0) && (A5 ==1) && (chi == 2) && (sigma != 1)) 
					{
						binary.at<uchar>(i,j) = 0;
						done = false;
					}
				else if((A0==1) && (A1==1) && (A5 ==0) && (chi == 2) && (sigma != 1))
					{
						binary.at<uchar>(i,j) = 0;
						done = false;
					}
				}
			}
			
        iterations++;
      
      } while (!done && (iterations < 100));
	  cout<<"iterations "<<iterations<<endl;
			
		imshow("thining", binary);
		cvtColor(binary, binary, CV_GRAY2BGR);
		
		video.write(binary);  
      	int k=waitKey(1);
      	if(k==27)
       	 break;
	
	}
	
}
