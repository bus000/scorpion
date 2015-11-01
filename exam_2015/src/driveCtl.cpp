#include "driveCtl.hpp"

DriveCtl::DriveCtl(PlayerCc::PlayerClient *robot,
        PlayerCc::Position2dProxy *position){

    this->robot = robot;
    this->positionProxy = position;
    this->odometryOffset = Particle(0, 0, 0);
    reset();
}

DriveCtl::odometryParticle() {
    robot->Read();
    double x = positionProxy->getXPos();
    double y = positionProxy->getYPos();
    double t = positionProxy->getYaw();

    return Particle(x, y, t);
}

Particle DriveCtl::reset() {
    Particle odo = this->odometryParticle();
    this->odometryOffset = odo;
}

Particle DriveCtl::pose() {
    Particle odo = this->odometryParticle(); 

    odo.subTheta(this->odometryOffset);

    return odo;
}

void DriveCtl::gotoPose(Particle position, ) {
    // Calculate the vector we want to drive
    Particle diff(position);
    diff.subTheta(pose());

    // Make the ´real´ position in odometry-coordinates
    Particle realPosition(diff);
    realPosition.addTheta(odometryOffset());

    // Make a vector a little bit further than where
    // we want to be.
    Particle littleMore(diff);
    littleMore.addLength(GOTO_OFFSET);
    littleMore.addTheta(odometryOffset());

    // Start moving the robot
    positionProxy->GoTo( realPosition.x()
                       , realPosition.y()
                       , realPosition.theta()
                       );

    // Keep checking if we are there (or close)
    while (true) {
        Particle odo = odometryParticle();
        Particle remaining(littleMore);
        remaining.subTheta(odo);

        double thetaDiff = 
            abs( remaining.theta()
               - realPosition.theta());

        if (remaining.length() < GOTO_OFFSET &&
            thetaDiff < THETA_THRES)
          break;

        // Sleep for a short while (100 ms)
        usleep(100000);
    }
}

void DriveCtl::turn(double rads) {
    Particle newPose = pose(); 
    newPose.theta(newPose.theta() + rads);

    gotoPose(newPose);
}

void DriveCtl::drive(double dist) {
    Particle unit = Particle::createUnit(pose().theta());
    unit.scale(dist);

    gotoPose(unit);
}
