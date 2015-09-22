#include "locator.hpp"
#include <libplayerc++/playerc++.h>
#include <iostream>
#include <cmath>
#include <stdio.h>

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

#define MAX_TURN_SPEED 0.9
#define MAX_FRONT_SPEED 0.3

PlayerCc::PlayerClient *robot;
PlayerCc::Position2dProxy *position;

VideoCapture *capture;
Size frameSize;
double searchFrame = 0.2;

double toRadian(double degree) {
    return degree*(M_PI/180.0);
}

void turnInPlace(double degree) {
    double motorSpeed = 0.1;

    if(degree < 0){
        motorSpeed *= -1.0;
        degree *= -1.0;
    }
        
    position->SetSpeed(0.0, motorSpeed);

    double radian = toRadian(degree);

    double radiansTurned = 0.0;
    cout << "Radians: " << radian << endl;
    while(true){
        position->ResetOdometry();
        robot->Read();
        
        radiansTurned += abs(position->GetYaw());

        if(radiansTurned >= radian)
            break;
    }

    position->SetSpeed(0.0, 0.0);
}

void drawWindow(vector<Point> *hull, vector<Point> *lines){
    Mat dest = Mat::zeros(frameSize, CV_8UC3);

    if(hull != NULL) {
        drawHull(*hull, Scalar(255, 255, 255), dest);
        Point center = hullCenter(hull);
        circle(dest, center, 20, Scalar(0, 0, 255), 3);
    }

    if(lines != NULL) {
        for(int i = 0; i < lines->size(); i += 2){
            line(dest, lines->at(i), lines->at(i+1), Scalar(0,0,255), 2);
        }
    }

    imshow("videoWindow", dest);
}

int isInCenter(Point point, double centerProcent) {
    double max = frameSize.width/2+((frameSize.width/2)*centerProcent);
    double min = frameSize.width/2-((frameSize.width/2)*centerProcent);

    if(point.x < min)
        return -1;
    else if(point.x > max)
        return 1;
    else
        return 0;
}

void findRedBox(){
    Mat frame;
    vector<Point> lines;

    double rightX = frameSize.width/2+((frameSize.width/2)*searchFrame);
    double leftX = frameSize.width/2-((frameSize.width/2)*searchFrame);
    Point rightTop(rightX, 0);
    Point rightBottom(rightX, frameSize.height-1);
    Point leftTop(leftX, 0);
    Point leftBottom(leftX, frameSize.height-1);

    lines.push_back(rightTop);
    lines.push_back(rightBottom);
    lines.push_back(leftTop);
    lines.push_back(leftBottom);

    while(true){
        waitKey(30);
        //read frame, check for convex hull
        capture->read(frame);
        vector<Point> *hull = getHull(frame);
        drawWindow(hull, &lines);

        if(hull == NULL){
            cout << "Nothing found... Turning." << endl;
            //Turn 6 degrees
            turnInPlace(6.0);
            continue;
        }

        Point center = hullCenter(hull);
        delete hull;

        int decision = isInCenter(center, searchFrame);
        switch(decision){
            case 0:{
                cout << "FOUND in middle !!!" << endl;
                return;
            }
            default: {
                cout << "JUST FOUND!!!" << endl;
                turnInPlace(2.0*decision);
            }
        }
    }
}

bool goToBox(){
    Mat frame;
    double xCenter = frameSize.width/2;

    while(true){
        waitKey(30);
        capture->read(frame);
        vector<Point> *hull = getHull(frame);

        if(hull == NULL){
            position->SetSpeed(0.0, 0.0);
            return false;
        }
        drawWindow(hull, NULL);
        
        Point center = hullCenter(hull);
        delete hull;
        hull = NULL;

        double turn = MAX_TURN_SPEED*((center.x - xCenter)/frameSize.width)*2;
        cout << "Turning: " << turn << endl;
        position->SetSpeed(MAX_FRONT_SPEED, turn);
    }
}

int main(int argc, char **argv){
    //robot = new PlayerCc::PlayerClient("192.168.240.129");
    robot = new PlayerCc::PlayerClient("localhost");
    position = new PlayerCc::Position2dProxy(robot);
    capture = new VideoCapture(0);

    Mat frame;
    
    if(!capture->isOpened()){
        cout << "Failed to open camera" << endl;
        return -1;
    }
    
    initHullWindow("videoWindow");
    if(!capture->read(frame)){
        cout << "Cannot read frame" << endl;
        return 0;
    }

    frameSize = frame.size();

    while(true){
        findRedBox();
        if(!goToBox())
            continue;

    }

    return 0;
}
