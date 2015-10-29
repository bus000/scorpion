#include "driveCtl.hpp"
#include <iostream>
#include <cmath>
#include <unistd.h>
#include "util.hpp"

DriveCtl::DriveCtl(PlayerCc::PlayerClient *robot,
        PlayerCc::Position2dProxy *position){

    this->robot = robot;
    this->position = position;
    this->speed = 0.1;
    this->turnSpeed = 0.2;
    this->direction = 1;
    this->xPos = 0.0;
    this->yPos = 0.0;
    this->yaw = 0.0;
    this->yawed = 0.0;
    this->driven = 0.0;
}

void DriveCtl::resetYawed(){
    this->yawed = 0.0;
}

void DriveCtl::resetDriven(){
    this->driven = 0.0;
}

void DriveCtl::resetCounters(){
    this->resetDriven();
    this->resetYawed();
}

double DriveCtl::getYawed(){
    return this->yawed;
}

double DriveCtl::getDriven(){
    return this->driven;
}

void DriveCtl::drive(double cm){
    this->position->SetSpeed(this->speed*direction, 0.0);
    usleep(cm*DRIVE_TIME);
    this->position->SetSpeed(0.0,0.0);

    //Update x, y pos
    this->xPos += cm*cos(this->toRadians(this->yaw));
    this->yPos += cm*sin(this->toRadians(this->yaw));
    //update driven
    this->driven += cm;
}

void DriveCtl::turn(double degrees){
    if (areSame(0.0, degrees))
        return;

    double turnDirection = 1.0;
    if(degrees < 0)
        turnDirection = -1.0;
    degrees = std::abs(degrees);

    this->position->SetSpeed(0.0, this->turnSpeed*turnDirection);
    usleep(degrees*TURN_TIME);
    this->position->SetSpeed(0.0, 0.0);

    //update yaw
    this->yaw += degrees*turnDirection*-1;
    this->overRunYaw();
    //update yawed
    this->yawed += std::abs(degrees);
}

void DriveCtl::turnLeft(double degrees){
    this->turn(degrees);
}

void DriveCtl::turnRight(double degrees){
    this->turn(degrees*-1.0);
}

double DriveCtl::toRadians(double degrees){
    return degrees * (M_PI/180.0);
}

double DriveCtl::toDegrees(double radians){
    return radians * (180.0/M_PI);
}

void DriveCtl::setDirection(int direction){
    this->direction = direction;
}

void DriveCtl::setXPos(double x){
    this->xPos = x;
}

void DriveCtl::setYPos(double y){
    this->yPos = y;
}

void DriveCtl::setYaw(double yaw){
    this->yaw = yaw;
    this->overRunYaw();
}

double DriveCtl::getXPos(){
    return this->xPos;
}

double DriveCtl::getYPos(){
    return this->yPos;
}

double DriveCtl::getYaw(){
    return this->yaw;
}

void DriveCtl::overRunYaw(){
    if(this->yaw > 360.0){
        this->yaw -= 360.0;
    }else if(this->yaw < -360.0){
        this->yaw += 360.0;
    }
}

void DriveCtl::goToPos(double xTo, double yTo){
    double theta = this->toRadians(this->getYaw());
    double yDiff = xTo - this->xPos;
    double xDiff = yTo - this->yPos;

    double thetaDiff = atan2(yDiff, xDiff);

    double cs = cos(theta);
    double sn = sin(theta);

    double xRot = xDiff * cs - yDiff * sn;
    double yRot = xDiff * sn + yDiff * cs;

    double finTheta = 0.5 * M_PI - atan2(yRot, xRot);
    if (finTheta < 0) finTheta += 2*M_PI;

    if (finTheta > M_PI){
        finTheta = 2*M_PI - finTheta;
        printf("Turning left %f degrees\n", finTheta * 57.2958);
        turnLeft(this->toDegrees(finTheta));
    }
    else {
        printf("Turning right %f degrees\n", finTheta * 57.2958);
        turnRight(this->toDegrees(finTheta));
    }

    double dist = sqrt(pow(xRot, 2.0) + pow(yRot, 2.0));
    this->setDirection(DRIVE_FORWARD);
    this->drive(dist);

    printf("Driving %f cm\n", dist);
    printf("posX: %f\nPosY: %f\nYaw: %f\n", this->xPos, this->yPos, this->yaw);
}

//void DriveCtl::goToPos(double x, double y) {
//    x = x - this->xPos;
//    y = y - this->yPos;
//
//    double angleD = atan2(y, x);
//    if(angleD < 0.0)
//        angleD += 2.0*M_PI;
//    double angleR = this->toRadians(this->yaw);
//    double angleT = angleR - angleD;
//    double vecLength = sqrt(pow(x,2)+pow(y,2));
//
//    angleT = this->toDegrees(angleT);
//    this->turn(angleT);
//
//    this->setDirection(DRIVE_FORWARD);
//    this->drive(vecLength);
//}

//void DriveCtl::goToPos(double x, double y) {
//    x = x - this->xPos;
//    y = y - this->yPos;
//
//    double tAngle = atan2(y, x) - atan2(sin(this->toRadians(this->yaw)),
//            cos(this->toRadians(this->yaw)));
//    if(tAngle < 0.0)
//        tAngle += 2.0*M_PI;
//    tAngle = this->toDegrees(tAngle);
//
//    if(tAngle > 180.0)
//        this->turnLeft(360.0-tAngle);
//    else
//        this->turnRight(tAngle);
//
//    std::cout << "drive angle: " << tAngle << std::endl;
//    double vecLength = sqrt(pow(x,2)+pow(y,2));
//    this->setDirection(DRIVE_FORWARD);
//    this->drive(vecLength);
//}
