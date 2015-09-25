#include "locator.hpp"

double distance(double height, double realHeight) {
  return 1034.246244 * (realHeight / height);
}

int main(int argc, char **argv) {
    VideoCapture capture(0);
    Mat frame;
    Mat dest;
    
    if(!capture.isOpened()){
        cout << "Failed to open camera" << endl;
        return -1;
    }

    initHullWindow("videoWindow");

    while (true) {
        if(!capture.read(frame)){
            cout << "Cannot read frame" << endl;
            break;
        }

        double height = 0;
        double dist = 0;

        dest = Mat::zeros(frame.size(), CV_8UC3);
        vector<Point> *hull = getHull(frame);
        if (hull != NULL) {
            drawHull(*hull, Scalar(255, 255, 0), frame);
            Point center = hullCenter(*hull);
            // circle(dest, center, 20, Scalar(0, 0, 255), 3);

            height = hullHeight(*hull);
            dist   = distance(height, 0.21);
            printf("Height (px): %f\tDistance (m): %f\n", height, dist);
        }
        
        resize(frame, frame, Size(), 0.5, 0.5); 
        imshow("videoWindow", frame);

        int key = waitKey(30);
        if(key == 27)
        delete hull;
    }

    return 0;
}
