#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include "colorFilter.hpp"
#include <vector>

using namespace cv;
using namespace std;

double hullCircum(vector<Point> hull) {
  double circum = 0.0;

  for (int i = 1; i < hull.size(); i++) {
    Point a = hull[i-1];
    Point b = hull[i];

    circum += norm(b - a); 
  }

  return abs(circum);
}

Point findGeometry(Mat src, Mat &dst) {
  RNG rng(12345);

  Mat denoized;
  GaussianBlur(src, denoized, Size(5, 5), 0);
  GaussianBlur(denoized, denoized, Size(5, 5), 0);

  // Convert to lab
  Mat imgLab;
  cvtColor(denoized, imgLab, CV_BGR2Lab);

  // Split channels
  vector<Mat> channels;
  split(imgLab, channels);

  Mat threshed;
  threshold(channels[0], threshed, 40, 255, THRESH_BINARY);
  
  vector<vector<Point> > contours;
  vector<Vec4i> hierarchy;
  findContours(threshed, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

  vector<vector<Point> > hull(contours.size());
  for( int i = 0; i < contours.size(); i++ ) {
    convexHull( Mat(contours[i]), hull[i], false );
  }


  vector<Point> biggestHull;
  double biggetCirc = 0.0;

  for (int i = 0; i < hull.size(); i++) {
    double circ = hullCircum(hull[i]);

    if (circ > biggetCirc) {
      biggetCirc = circ;
      biggestHull = hull[i];
    }
  }

  printf("Biggest circ: %f\n", biggetCirc);
  
  dst = Mat::zeros(threshed.size(), CV_8UC3);
  for( int i = 1; i< biggestHull.size(); i++ )
  {
    Scalar color = Scalar(255, 255, 255);
    Point a = biggestHull[i-1];
    Point b = biggestHull[i];

    line(dst, a, b, color);
  }

  Point sum(0.0, 0.0);
  int count = 0;

  for (int j = 0; j < biggestHull.size(); j++) {
    sum += biggestHull[j];
    count += 1;
  }

  Point center = sum * (1.0/(double)count);
  
  return center;
}

int main(int argc, char **argv) {
    VideoCapture capture(0);

    if(!capture.isOpened()){
        cout << "Failed to open camera" << endl;
        return -1;
    }


    Mat channels[3];
    int mu = 0;
    int k = 150;
    int satWeight = 100;
    int min = 10;
    int max = 100;
    int minSat = 50;
    namedWindow("MyVideo", CV_WINDOW_NORMAL);
    createTrackbar("k:", "MyVideo", &k, 1000);
    createTrackbar("mu:", "MyVideo", &mu, 255);
    createTrackbar("blobMin:", "MyVideo", &min, 800);
    createTrackbar("blobMax:", "MyVideo", &max, 800);
    createTrackbar("minSat:", "MyVideo", &minSat, 255);
    while(1){
        Mat frame;
        Mat frameHSV;
        Mat redOnly;
        Mat redOnlyBGR;
        Mat withKeyPoints;

        if(!capture.read(frame)){
            cout << "Cannot read frame" << endl;
            break;
        }

        resize(frame, frame, Size(), 0.5, 0.5);

        cvtColor(frame, frameHSV, COLOR_BGR2HSV);
        split(frameHSV, channels);

        ColorFilter(frameHSV, redOnly, (double)mu, (double)k, (double)minSat);
        cvtColor(redOnly, redOnlyBGR, COLOR_HSV2BGR);

        Mat geom;
        Point center = findGeometry(redOnly, geom);
 
        circle(geom, center, 20, Scalar(0, 0, 255), 3);

        imshow("MyVideo", geom);

        int key = waitKey(30);
        if(key == 27)
            break;
    }
}
