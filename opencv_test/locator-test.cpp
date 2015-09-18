#include "locator.hpp"

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

        dest = Mat::zeros(frame.size(), CV_8UC3);
        vector<Point> *hull = getHull(frame);
        if (hull != NULL) {
            drawHull(*hull, Scalar(255, 255, 255), dest);
            Point center = hullCenter(*hull);
            circle(dest, center, 20, Scalar(0, 0, 255), 3);
        }
        
        imshow("videoWindow", dest);

        int key = waitKey(30);
        if(key == 27)
            break;
        delete hull;
    }

    return 0;
}
