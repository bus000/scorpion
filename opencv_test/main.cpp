#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include "colorFilter.hpp"

using namespace cv;
using namespace std;

int main(int argc, char **argv) {
    VideoCapture capture(0);

    if(!capture.isOpened()){
        cout << "Failed to open camera" << endl;
        return -1;
    }

    namedWindow("MyVideo", CV_WINDOW_AUTOSIZE);

    Mat channels[3];
    double strengh = 150.0;
    while(1){
        Mat frame;
        Mat frameHSV;
        Mat redOnly;
        Mat redOnlyBGR;

        if(!capture.read(frame)){
            cout << "Cannot read frame" << endl;
            break;
        }

        cvtColor(frame, frameHSV, COLOR_BGR2HSV);
        split(frameHSV, channels);

        ColorFilter(frameHSV, redOnly, 0.0, strengh);
        cvtColor(redOnly, redOnlyBGR, COLOR_HSV2BGR);

        imshow("MyVideo", redOnlyBGR);

        int key = waitKey(30);
        if(key == 27)
            break;
        else if(key == 65362)
            strengh++;
        else if(key == 65364)
            strengh--;

        cout << strengh << endl;
    }
}
