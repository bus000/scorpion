#ifndef DRIVE_CTL_H
#define DRIVE_CTL_H

#include <libplayerc++/playerc++.h>
#include "particle.hpp"
#include <stdio.h>

#define DEFAULT_SPEED 0.4
#define DEFAULT_TURN_SPEED 0.15

#define GOOD_ENOUGH_POS 3.0
#define GOOD_ENOUGH_ANGLE 0.05

#define DELTA_POS_STOP 0.2
#define DELTA_ANGLE_STOP 0.05

class DriveCtl{
public:
    DriveCtl(PlayerCc::PlayerClient *robot, PlayerCc::Position2dProxy *position);

    Particle reset();
    Particle pose();

    Particle gotoPose( Particle position
                     , void *data = NULL
                     , bool (*callback)(Particle, void*) = NULL
                     );

    Particle turn( double rads
                 , void *data = NULL
                 , bool (*callback)(Particle, void*) = NULL
                 );

    Particle drive( double dist
                  , void *data = NULL
                  , bool (*callback)(Particle, void*) = NULL
                  );

    void setPose(Particle pos);

private:
    PlayerCc::PlayerClient *robot;
    PlayerCc::Position2dProxy *positionProxy;
    Particle odometryOffset;
    void overRunYaw();
    Particle odometryParticle();
};

#endif
