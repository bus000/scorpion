#ifndef DRIVE_CTL_H
#define DRIVE_CTL_H

#include <libplayerc++/playerc++.h>
#include "particle.hpp"

#define GOTO_OFFSET 3.0
#define THETA_THRES 0.05

class DriveCtl{
public:
    DriveCtl(PlayerCc::PlayerClient *robot, PlayerCc::Position2dProxy *position);

    Particle reset();
    Particle pose();
    void gotoPose(Particle position);
    void turn(double rads);
    void drive(double dist);

private:
    PlayerCc::PlayerClient *robot;
    PlayerCc::Position2dProxy *positionProxy;
    Particle odometryOffset;
    void overRunYaw();
    Particle odometryParticle();
};

#endif
