// Very simple example found on Stackoverflow
//

#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <vector>

using namespace std;
using namespace cv;

int main(int argc, char *argv[])
{        
  Mat image = imread(argv[1]);

  // Create smart pointer for SIFT feature detector.
  Ptr<FeatureDetector> featureDetector = FeatureDetector::create("SIFT");
  vector<KeyPoint> keypoints;

  // Detect the keypoints
  featureDetector->detect(image, keypoints); // NOTE: featureDetector is a pointer hence the '->'.

  //Similarly, we create a smart pointer to the SIFT extractor.
  Ptr<DescriptorExtractor> featureExtractor = DescriptorExtractor::create("SIFT");

  // Compute the 128 dimension SIFT descriptor at each keypoint.
  // Each row in "descriptors" correspond to the SIFT descriptor for each keypoint
  Mat descriptors;
  featureExtractor->compute(image, keypoints, descriptors);

  // If you would like to draw the detected keypoint just to check
  Mat outputImage;
  Scalar keypointColor = Scalar(255, 0, 0);     // Blue keypoints.
  drawKeypoints(image, keypoints, outputImage, keypointColor, DrawMatchesFlags::DEFAULT);

  namedWindow("Output");
  imshow("Output", outputImage);

  char c = ' ';
  while ((c = waitKey(0)) != 'q');  // Keep window there until user presses 'q' to quit.

  return 0;

}
