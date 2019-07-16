#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>

using namespace cv;
using namespace std;


void help()
{
cout << "\nThis program demonstrates a skeletal transform on the Live Camera\n"
        "Usage:\n"
        "./skeletal \n" << endl;
}

int main(int argc, char** argv)
{
   int framecount;
   char image [15];
   Mat gray, binary, mfblur, frame, hist;
   VideoCapture cap(0); // open the default camera
   //get frame size to encode output video of same size
   Size framesize = Size((int)cap.get(CV_CAP_PROP_FRAME_WIDTH), (int)cap.get(CV_CAP_PROP_FRAME_HEIGHT));
  
    //Set Output Video Parameters
   VideoWriter video;
   video.open("Output.avi", cap.get(CV_CAP_PROP_FOURCC), (cap.get(CV_CAP_PROP_FPS)), framesize, true);
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
     framecount ++;
     cap >> frame;
     if(frame.empty())
     {
         break;
     }
     //convert frame to grayscale
     cvtColor(frame, gray, CV_BGR2GRAY);
     // Use 70 negative for Moose, 150 positive for hand
     // To improve, compute a histogram here and set threshold to first peak
     // For now, histogram analysis was done with GIMP
     threshold(gray, binary, 150, 255, CV_THRESH_BINARY);
     binary = 255 - binary;

     // To remove median filter, just replace blurr value with 1
     medianBlur(binary, mfblur, 1);

     // This section of code was adapted from the following post, which was
     // based in turn on the Wikipedia description of a morphological skeleton
     //
     // http://felix.abecassis.me/2011/09/opencv-morphological-skeleton/
     //
     Mat skel(mfblur.size(), CV_8UC1, Scalar(0));
     Mat temp;
     Mat eroded;
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
     snprintf(image,15,"frame%04d.jpg",framecount);
     if(framecount <= 3000)
     {
   //  imwrite(image,skel);
     }
    
     imshow("skeleton", skel);
     video.write(skel); 
     int k=waitKey(2);
     if(k==27)
       break;
   }
}








