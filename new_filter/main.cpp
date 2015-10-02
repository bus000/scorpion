#include <iostream>
#include "colorFilter.hpp"
#include <cmath>
#include <iostream>

int main(int argc, char **argv){
    int k = 200;
    int mu = 0;
    cv::VideoCapture capture(CV_CAP_ANY);
    Mat frame;
    Mat dest;
    Mat binDest;

    namedWindow("testWindow", CV_WINDOW_NORMAL);
    namedWindow("testWIndow2", CV_WINDOW_NORMAL);
    createTrackbar("k: ", "testWindow", &k, 1000);
    createTrackbar("mu: ", "testWindow", &mu, 255);

    if(!capture.isOpened()){
        std::cout << "Failed to open camera" << std::endl;
        return -1;
    }

    while(true){
        capture.read(frame);

        cv::cvtColor(frame, frame, COLOR_BGR2HSV);
        ColorFilter(frame, dest, mu, k);
        ToBin(dest, binDest, 5);

        cv::cvtColor(dest, dest, COLOR_HSV2BGR);
        imshow("testWindow", binDest);
        cv::cvtColor(frame, frame, COLOR_HSV2BGR);
        imshow("testWindow2", frame);
        
        int key = cv::waitKey(30);
        switch(key){
            case 27:
                return 0;
            default:
                std::cout << key << std::endl;
        }
    }

    return 0;
}
