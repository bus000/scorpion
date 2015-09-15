#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include "colorFilter.hpp"
#include <vector>

using namespace cv;
using namespace std;

void blob(Mat src, Mat &dest, int min, int max){
    SimpleBlobDetector::Params params;

    params.filterByArea = true;
    params.minArea = 50;
    params.maxArea = 200;

    params.filterByCircularity = true;
    params.minCircularity = 0.735;
    params.maxCircularity = 0.815;

    params.minThreshold = min;
    params.maxThreshold = max;

    SimpleBlobDetector detector(params);

    vector<KeyPoint> keypoints;

    KeyPoint k;

    detector.detect(src, keypoints);
    drawKeypoints(src, keypoints, dest, Scalar(0,0,255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
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
    int minSat = 0;
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

        cvtColor(frame, frameHSV, COLOR_BGR2HSV);
        split(frameHSV, channels);

        ColorFilter(frameHSV, redOnly, (double)mu, (double)k, (double)minSat);
        cvtColor(redOnly, redOnlyBGR, COLOR_HSV2BGR);
        blob(redOnlyBGR, withKeyPoints, min, max);

        imshow("MyVideo", withKeyPoints);

        int key = waitKey(30);
        if(key == 27)
            break;
    }
}
