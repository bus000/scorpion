#include "driveCtl.hpp"

DriveCtl::DriveCtl(PlayerCc::PlayerClient *robot,
        PlayerCc::Position2dProxy *position){

    this->robot = robot;
    this->positionProxy = position;
    this->odometryOffset = Particle(0, 0, 0);
    reset();
}

Particle DriveCtl::odometryParticle() {
    usleep(100000); 
    robot->Read();
    double x = positionProxy->GetXPos();
    double y = positionProxy->GetYPos();
    double t = positionProxy->GetYaw();

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

void DriveCtl::gotoPose(Particle position) {
    // Calculate the vector we want to drive
    Particle diff(position);
    diff.subTheta(pose());

    // Make the ´real´ position in odometry-coordinates
    Particle realPosition(diff);
    realPosition.addTheta(odometryOffset);

    // Make a vector a little bit further than where
    // we want to be.
    Particle littleMore(diff);
    littleMore.addLength(GOTO_OFFSET);
    littleMore.addTheta(odometryOffset);

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
}

void DriveCtl::drive(double dist) {
  dist = dist / 100.0;

  Particle odo = odometryParticle();

  Particle unit = Particle::createUnit(odo.theta());
  unit.scale(dist); // In cm

  Particle final = odo;
  final.addTheta(unit);

  positionProxy->SetSpeed(DEFAULT_SPEED, 0);

  while(true) {
      Particle diff = odometryParticle();
      diff.sub(final);

      printf("Diff: l: %f, (%f, %f, %f)\n", diff.length()
                                          , diff.x()
                                          , diff.y()
                                          , diff.theta()
                                          );

      if (diff.length() < GOOD_ENOUGH_POS)
        break;
     
  }

  positionProxy->SetSpeed(0, 0);
}
