#ifndef DRIVE_CTL_H
#define DRIVE_CTL_H

#define DRIVE_TIME 10000000/96.5
#define TURN_TIME 61100000/(2*360)
#define DRIVE_FORWARD 1
#define DRIVE_BACKWARD -1
#include <libplayerc++/playerc++.h>


class DriveCtl{
public:
    DriveCtl(PlayerCc::PlayerClient *robot, PlayerCc::Position2dProxy *position);

    void drive(double cm);
    void turn(double degrees);
    void turnLeft(double degrees);
    void turnRight(double degrees);
    void setDirection(int direction);
    void setXPos(double x);
    void setYPos(double y);
    void setYaw(double yaw);
    double getXPos();
    double getYPos();
    double getYaw();
    void goToPos(double x, double y);
    void resetCounters();
    void resetYawed();
    void resetDriven();
    double getYawed();
    double getDriven();
    double toRadians(double degrees);
    double toDegrees(double radians);

private:
    PlayerCc::PlayerClient *robot;
    PlayerCc::Position2dProxy *position;
    double direction;
    double speed;
    double turnSpeed;
    double xPos, yPos, yaw;
    double yawed, driven;
    void overRunYaw();
};

#endif
