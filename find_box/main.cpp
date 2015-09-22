#include "locator.hpp"
#include <libplayerc++/playerc++.h>
#include <iostream>
#include <cmath>

#define IR_bn_ene 0
#define IR_bn_wnw 1
#define IR_bn_n 2
#define IR_bn_ne 3
#define IR_bn_nw 4
#define IR_te_nnw 5
#define IR_te_nne 6
#define IR_tw_nnw 7
#define IR_tw_nne 8
#define IR_bs_w 9
#define IR_bs_e 10
#define IR_bw_s 11
#define IR_be_s 12

PlayerCc::PlayerClient *robot;
PlayerCc::Position2dProxy *position;

VideoCapture *capture;
Size frameSize;
double searchFrame = 0.1;

double toRadian(double degree) {
    double radian = degree*(180.0/M_PI);

    if(radian < 0.0)
        radian = (2.0*M_PI)-radian;
    return radian;
}

void turnInPlace(double degree) {
    double motorSpeed = 0.1;
    position->ResetOdometry();

    if(degree < 0)
        motorSpeed *= -1;
        
    position->SetSpeed(0.0, 0.1);

    double radian = toRadian(degree);
    if(degree > 0.0)
        while (position->GetYaw() < radian) ;
    else if(degree < 0.0)
        while (position->GetYaw() > radian) ;
}

void drawWindow(vector<Point> *hull){
    Mat dest = Mat::zeros(frameSize, CV_8UC3);

    drawHull(*hull, Scalar(255, 255, 255), dest);
    Point center = hullCenter(hull);
    circle(dest, center, 20, Scalar(0, 0, 255), 3);
    imshow("videoWindow", dest);
}

int isInCenter(Point point) {
    double max = frameSize.width/2+((frameSize.width/2)*searchFrame);
    double min = frameSize.width/2-((frameSize.width/2)*searchFrame);

    if(point.x < min)
        return -1;
    else if(point.x > max)
        return 1;
    else
        return 0;
}

void findRedBox(){
    Mat frame;

    while(true){
        //read frame, check for convex hull
        capture->read(frame);
        vector<Point> *hull = getHull(frame);
        if(hull == NULL){
            cout << "Nothing found... Turning." << endl;
            //Turn 6 degrees
            turnInPlace(6.0);
            continue;
        }
        drawWindow(hull);

        Point center = hullCenter(hull);
        delete hull;

        int decision = isInCenter(center);
        switch(decision){
            case 0:
                return;
            default:
                turnInPlace(6.0);
        }
    }
}

int main(int argc, char **argv){
    robot = new PlayerCc::PlayerClient("192.168.240.129");
    position = new PlayerCc::Position2dProxy(robot);
    capture = new VideoCapture(0);

    Mat frame;
    
    if(!capture->isOpened()){
        cout << "Failed to open camera" << endl;
        return -1;
    }
    
    if(!capture->read(frame)){
        cout << "Cannot read frame" << endl;
        return 0;
    }

    frameSize = frame.size();

    initHullWindow("videoWindow");

    while (true) {
        if(!capture->read(frame)){
            cout << "Cannot read frame" << endl;
            break;
        }

        vector<Point> *hull = getHull(frame);
        drawWindow(hull);

        int key = waitKey(30);
        if(key == 27)
            break;
        delete hull;
    }

    return 0;
}
