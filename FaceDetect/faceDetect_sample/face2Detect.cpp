 #include "opencv2/objdetect/objdetect.hpp"
 #include "opencv2/highgui/highgui.hpp"
 #include "opencv2/imgproc/imgproc.hpp"
 #include <iostream>
 #include <stdio.h>

 using namespace std;
 using namespace cv;

 int main( )
   {
      Mat image;
      Mat image_gray;
      image = imread("lena.jpg", CV_LOAD_IMAGE_COLOR);  
      //   namedWindow( "window1", 1 ); //  imshow( "window1", image );

       CascadeClassifier face_cascade;
       CascadeClassifier eyes_cascade;
       CascadeClassifier smile_cascade;

       face_cascade.load( "/home/veena/Downloads/opencv-3.2.0/data/haarcascades /haarcascade_frontalface_alt.xml" );
       eyes_cascade.load( "/home/veena/Downloads/opencv-3.2.0/data/haarcascades/haarcascade_eye_tree_eyeglasses.xml"); 
       smile_cascade.load("/home/veena/Downloads/opencv-3.2.0/data/haarcascades/haarcascade_smile.xml");
// Detect faces
       std::vector<Rect> faces;
       cvtColor(image, image_gray, COLOR_BGR2GRAY);
       equalizeHist(image_gray, image_gray);
       face_cascade.detectMultiScale( image_gray, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(512, 512) );
       for( int i = 0; i < faces.size(); i++ )
         {
            Point center( faces[i].x + faces[i].width/2, faces[i].y + faces[i].height/2 );
            ellipse( image, center, Size( faces[i].width/2, faces[i].height/2), 0, 0, 360, Scalar( 255, 0, 255 ), 4, 8, 0 );
            // rectangle(image,Point (faces[i].x,faces[i].y),Point (faces[i].x+faces[i].width, faces[i].y+faces[i].height),Scalar(255,0,255),4,8,0);

             Mat faceROI = image_gray(faces[i]);
             std::vector<Rect> eyes;
             eyes_cascade.detectMultiScale(faceROI, eyes, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(30,30));
             for(int j=0;j < eyes.size(); j++)
               {
                 Point eye_center( faces[i].x + eyes[j].x + eyes[j].width/2, faces[i].y  +eyes[j].y + eyes[j].height/2 );
                 int radius = cvRound((eyes[j].width+eyes[j].height)*0.25);        
                 circle( image,eye_center,radius,Scalar(255, 0, 0),4,8,0 );
                }

               std::vector<Rect> smile;
               smile_cascade.detectMultiScale(faceROI, smile, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(80,80));
               for(int j = 0; j < smile.size(); j++)
                 {
                   Point smile_center(faces[i].x + smile[j].x + smile[j].width/2,  faces[i].y + smile[j].y + smile[j].height/2);
                   int radius = cvRound((smile[j].width+smile[j].height)*0.25);        
                   circle( image,smile_center,radius,Scalar(0, 255, 0),4,8,0 );

                   }
             }
           imshow( "Detected Face", image );

           waitKey(0);                   
           return 0;
    }
