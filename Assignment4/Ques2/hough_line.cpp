/**
 * @file hough_line.cpp
 * @author Deepesh 
 * @brief Draw Hough lines (Probabilistic and non probabilistic)
 * @version 0.1
 * @date 2019-07-17
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgcodecs/imgcodecs.hpp"
#include <iostream>

using namespace cv;
using namespace std;

/**
 * @brief helper function
 * 
 */
void help();

int main(int argc, char** argv)
{
 const char* filename = argc >= 2 ? argv[1] : "pic1.jpg";

 Mat src = imread(filename, 0);
 if(src.empty())
 {
     help();
     cout << "can not open " << filename << endl;
     return -1;
 }

 Mat dst, cdst, cdstP;
 Canny(src, dst, 50, 200, 3);
 cvtColor(dst, cdst, CV_GRAY2BGR);

 cdstP = cdst.clone();
 vector<Vec2f> lines;
 HoughLines(dst, lines, 1, CV_PI/180, 150, 0, 0 );

  for( size_t i = 0; i < lines.size(); i++ )
  {
     float rho = lines[i][0], theta = lines[i][1];
     Point pt1, pt2;
     double a = cos(theta), b = sin(theta);
     double x0 = a*rho, y0 = b*rho;
     pt1.x = cvRound(x0 + 1000*(-b));
     pt1.y = cvRound(y0 + 1000*(a));
     pt2.x = cvRound(x0 - 1000*(-b));
     pt2.y = cvRound(y0 - 1000*(a));
     line( cdst, pt1, pt2, Scalar(0,0,255), 3, LINE_AA);
  }

  vector<Vec4i> linesP;
  HoughLinesP(dst, linesP, 1, CV_PI/180, 50, 50, 10 );
  for( size_t i = 0; i < linesP.size(); i++ )
  {
    Vec4i l = linesP[i];
    line( cdstP, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 3, CV_AA);
  }
 imshow("source", src);
 imshow("detected lines Standard Hough transform", cdst);
 imshow("detected lines Probabilistic Hough transform", cdstP);
 imwrite("Standard Hough transform.jpg", cdst);
 imwrite("Probabilistic Hough transform.jpg", cdstP);
 waitKey();

 return 0;
}


void help()
{
 cout << "\nThis program demonstrates line finding with the Hough transform.\n"
         "Usage:\n"
         "./houghlines <image_name>" << endl;
}
