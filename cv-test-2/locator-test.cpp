#include "locator.hpp"
#include "cloud.hpp"
#include "poly.hpp"
#include <math.h>

double distance(double height, double realHeight) {
  return 1034.246244 * (realHeight / height);
}

int main(int argc, char **argv) {
    VideoCapture capture(0);
    Mat frame;
    Mat dest;
    double fx = 1058.051100;
    double fy = 1068.394230;
    
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
          vector<Line> hlines = horizontalLines(filtered);
          vector<Line> vlines = verticalLines(filtered);

          drawHull(*hull, Scalar(200, 255, 200), frame);

          for (int i = 0; i < hlines.size(); i++) {
            line(frame, hlines[i].a, hlines[i].b, Scalar(150, 0, 0), 4);
          }

          for (int i = 0; i < vlines.size(); i++) {
            line(frame, vlines[i].a, vlines[i].b, Scalar(0, 150, 0), 4);
          }
          
          if (hlines.size() > 1 && vlines.size() > 1) {
            double width1   = hlines[0].length();
            double width2   = hlines[1].length();
            double width    = (width1 + width2) / 2.0;

            double height1  = vlines[0].length();
            double height2  = vlines[1].length();
            double height   = (width1 + width2) / 2.0;

            double angSign  = height1 > height2 ? -1.0 : 1.0;

            double distance = distanceToPoly(0.218, vlines[0].height(), fy);
            double angle    = angSign * angleToPoly(distance, 0.297, hlines[0].width(), fx);
            printf("distance: (%f m, %f px); angle (%f deg, %f px)\n", distance, height, (180.0/M_PI) * angle, width);
          }
        }

        resize(frame, frame, Size(), 0.5, 0.5); 
        imshow("videoWindow", frame);

        int key = waitKey(30);
        if(key == 27)
        delete hull;
    }

    return 0;
}
