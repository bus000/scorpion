#include "DriveCtl.hpp"
#include <iostream>
#include <cmath>
#include <unistd.h>

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
}

void DriveCtl::drive(double cm){
    this->position->SetSpeed(this->speed*direction, 0.0);
    usleep(cm*DRIVE_TIME);
    this->position->SetSpeed(0.0,0.0);

    //Update x, y pos
    this->xPos += cm*cos(this->toRadians(this->yaw));
    this->yPos += cm*sin(this->toRadians(this->yaw));
}

void DriveCtl::turn(double degrees){
    double turnDirection = 1.0;
    if(degrees < 0)
        turnDirection = -1.0;
    degrees = std::abs(degrees);

    this->position->SetSpeed(0.0, this->turnSpeed*turnDirection);
    usleep(degrees*TURN_TIME);
    this->position->SetSpeed(0.0, 0.0);

    //update yaw
    this->yaw += degrees*turnDirection;
    this->overRunYaw();
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

void DriveCtl::goToPos(double x, double y){
    x = x - this->xPos;
    y = y - this->yPos;

    double vecLength = sqrt(pow(x,2)+pow(y,2));
    double gotoAngle = this->toDegrees(acos(x/vecLength));
    double angle = gotoAngle;
    std::cout << "angle: " << angle << std::endl;
    std::cout << "vecLength: " << vecLength << std::endl;

    if(y < 0.0)
        angle = 360.0 - angle;

    std::cout << "noget: " << angle << std::endl;

    if(angle > 180.0)
        this->turnRight(360.0-angle);
    else
        this->turnLeft(angle);

    this->setDirection(DRIVE_FORWARD);
    this->drive(vecLength);
}
