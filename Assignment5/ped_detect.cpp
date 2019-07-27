
/**
 * @file ped_detect.cpp
 * @author Deepesh Sonigra
 * @brief 
 * @version 0.1
 * @date 2019-07-25
 * @reference  http://www.magicandlove.com/blog/2011/08/26/people-detection-in-opencv-again/
 * 
 * @copyright Copyright (c) 2019
 * 
 */

/* This code is reffered from the link http://www.magicandlove.com/blog/2011/08/26/people-detection-in-opencv-again/
 * The code is understood and used according to my application. 
*/

#include <iostream>
#include <opencv2/opencv.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdio.h>
#include <string.h>

using namespace std;
using namespace cv;



#define SHOW        0x01
#define STORE       0x02
#define LOG         0x04


 
int main (int argc, const char * argv[])
{
    uint8_t arguments = 0x00;
    int count = 2;
    string input_video = argv[1];
    string output_video;
    Mat frame;
    timespec start, finish;
    double frame_time_secs;
    double frame_time_nsecs;
    FILE *fptr ;
    //set flags based on the arguments
    while(count < argc)
    {
        if(argv[count] == string("--show"))
        {
            arguments |= SHOW;
        }
        
        if(argv[count] == string("--store"))
        {
            arguments |= STORE;
            output_video = argv[count + 1];
        }
        
        if(argv[count] == string("--log"))
        {
            arguments |= LOG;
        }
     
     count++;
    }
      // if log file needed initialize file pointer
        if(arguments & LOG)
        {
        
            fptr = fopen("log.txt", "w");
            if(fptr == NULL)
                printf("Error Opening Log file");
            fprintf(fptr, "Pedestrian Log data\n");
            fclose(fptr);
        }

    // start capturing input video
    VideoCapture cap(input_video);
      VideoWriter video;
   
    // check if input video is opened
    if(!cap.isOpened())
    {
        cout << "Error opening input video stream or file" << endl;
        return -1;
    }
    //get frame size from the input video
    Size framesize = Size((int)cap.get(CV_CAP_PROP_FRAME_WIDTH), (int) cap.get(CV_CAP_PROP_FRAME_HEIGHT));
   
    //if to be stored initialize output video
    if(arguments & STORE)
    {	
       
        video.open(output_video,CV_FOURCC('M','P','4','V') , (cap.get(CV_CAP_PROP_FPS)), framesize, true);
        if(!video.isOpened())
        {
            cout << "Error opening output video stream or file" << endl;
            return -1;
        }    
    }

   //define HOG descriptor
    HOGDescriptor hog;
    hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());
    while(true)
    {
        if(arguments & LOG)
	    fptr = fopen("log.txt", "a");
       
       //get frame from the video
        cap >> frame;
        //check if end of video
        if (frame.empty())
             break;

        vector<Rect> found, filtered;
        // detect people and store ion vector array
        hog.detectMultiScale(frame, found, 0, Size(8,8), Size(32,32), 1.05, 2);
    
        size_t i,j;
        //to check the number of people detected 
        for ( i=0; i<found.size(); i++)
        {
            //store in a array
            Rect r = found[i];
            
            for ( j=0; j< found.size(); j++)
                if (j!=i && (r & found[j])==r) // check if this person was already detected.
                    break; // if yes break the loop
            
            if (j==found.size())
                filtered.push_back(r); // count number of people
        }
	
        for (size_t i=0; i<filtered.size(); i++)
		{
            //define rectangle parameters
		    Rect r = filtered[i];
		    r.x += cvRound(r.width*0.1);
		    r.width = cvRound(r.width*0.8);
		    r.y += cvRound(r.height*0.06);
		    r.height = cvRound(r.height*0.9);

            //draw rectangle
		    rectangle(frame, r.tl(), r.br(), cv::Scalar(0,255,0), 2);
            
            //find center of rectangle
		    Point center = (r.br() + r.tl())* 0.5;
            //log the data
            if(arguments & LOG)
            fprintf(fptr, "Pedestrian detected [x,y] : [%d,%d] \n", center.x, center.y);
		}
	    if(arguments & SHOW)
            imshow("Output", frame); // show the window
    // total people detected
        int number_people = filtered.size();
       
        fprintf(fptr, "Number of people in the frame: %d\n\n", number_people);

        fclose(fptr);

        if(arguments & STORE)
        {
            // store the video
            video << frame;
        }
	

        if (waitKey(1) >= 0)
          //  break;
	    fclose(fptr);
	    //waitKey();
    }
    return 0;
}
