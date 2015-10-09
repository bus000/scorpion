#include "locator.hpp"
#include "kmeans.hpp"
#include <math.h>

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
 
    Point z = Point(0, 0);
    vector<Point> prev;
    prev.push_back(z);
    prev.push_back(z);
    prev.push_back(z);
    prev.push_back(z);

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
           vector<Point> filtered = findPoly(4, *hull);
           
           for (int i = 0; i < filtered.size(); i++) {
             circle(frame, filtered[i], 10, Scalar(255, 255, 0), 2);
           }

            drawHull(*hull, Scalar(0, 255, 255), frame);
            //Point center = hullCenter(*hull);
            // circle(dest, center, 20, Scalar(0, 0, 255), 3);

            height = hullHeight(filtered);
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
