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

}

void DriveCtl::drive(double cm){
    this->position->SetSpeed(this->speed*direction, 0.0);
    usleep(cm*DRIVE_TIME);
    this->position->SetSpeed(0.0,0.0);
}

void DriveCtl::turn(double degrees){
    this->position->SetSpeed(0.0, this->turnSpeed);
    usleep(degrees*TURN_TIME);
    this->position->SetSpeed(0.0, 0.0);
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
