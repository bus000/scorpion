#include "driveCtl.hpp"
#include "unistd.h"

DriveCtl::DriveCtl(PlayerCc::PlayerClient *robot,
        PlayerCc::Position2dProxy *position){

    this->robot = robot;
    this->positionProxy = position;
    this->odometryOffset = Particle(0, 0, 0);
    reset();
}

Particle DriveCtl::odometryParticle() {
    robot->Read();
    usleep(100000); 
    double x = positionProxy->GetXPos()*100.0;
    double y = positionProxy->GetYPos()*100.0;
    double t = positionProxy->GetYaw();

    return Particle(x, y, t);
}

Particle DriveCtl::reset() {
    Particle odo = this->odometryParticle();
    this->odometryOffset = odo;
}

void DriveCtl::setPose(Particle pos){
    Particle odo = odometryParticle();
    pos.theta(pos.theta());
    odo.subTheta(pos);
    odometryOffset = odo;
}

Particle DriveCtl::pose() {
    Particle odo = this->odometryParticle();

    odo.subTheta(this->odometryOffset);
    odo.theta(odo.theta());

    return odo;
}

Particle DriveCtl::gotoPose(Particle position, void *data,
            bool (*callback)(Particle, void*)) {
    Particle diff = position;
    Particle odo = odometryParticle();
    Particle goal = odo;
    diff.sub(pose());
    goal.add(diff);

    Particle diffParticle = goal;
    diffParticle.sub(odo);

    double diffAngle = diffParticle.angle() - pose().theta();
    double diffDistance = diffParticle.length();

    if(diffAngle > M_PI)
        diffAngle -= 2.0*M_PI;
    else if(diffAngle < -1.0*M_PI)
        diffAngle += 2.0*M_PI;

    turn(diffAngle, data, callback);
    drive(diffDistance, data, callback);

    return pose();
}

Particle DriveCtl::turn(double rads, void *data, bool(*callback)(Particle, void*)) {
    Particle odo = odometryParticle();
    Particle goal = odo;
    goal.theta(goal.theta()+rads);

    double sign = 1.0;
    if(rads < 0.0)
        sign = -1.0;
    positionProxy->SetSpeed(0.0,DEFAULT_TURN_SPEED*sign);

    double lastDiff = -1.0;
    while(true){
        Particle diff = odometryParticle();
        diff.subTheta(goal);

        printf("AngleDiff: %f\n", -1.0*diff.theta());

        if(fabs(diff.theta()) < GOOD_ENOUGH_ANGLE)
            break;
        if(lastDiff > 0.0 && fabs(diff.theta()) > lastDiff+GOOD_ENOUGH_ANGLE)
            break;
        else
            lastDiff = fabs(diff.theta());

        if(callback != NULL && !callback(pose(), data))
            break;
    }

    positionProxy->SetSpeed(0, 0);
    usleep(100000);
    return pose();
}

Particle DriveCtl::drive(double dist, void *data,
            bool (*callback)(Particle, void*)) {
    //dist = dist / 100.0;

    Particle odo = odometryParticle();

    Particle unit = Particle::createUnit(odo.theta());
    unit.scale(dist); // In cm

    Particle final = odo;
    final.add(unit);

    positionProxy->SetSpeed(DEFAULT_SPEED, 0);

    double lastDiff = -1.0;
    while(true) {
        Particle diff = final;
        diff.sub(odometryParticle());

        printf("Diff: l: %f, (%f, %f, %f)\n", diff.length()
                , diff.x()
                , diff.y()
                , diff.theta()
              );

        if (diff.length() < GOOD_ENOUGH_POS)
            break;
        if(lastDiff > 0.0 && diff.length() > lastDiff+0.2)
            break;
        else
            lastDiff = diff.length();

        if(callback != NULL && !callback(pose(), data))
            break;
    }


    Particle p = pose();
    printf("pose: l: %f, (%f, %f, %f)\n", p.length()
            , p.x()
            , p.y()
            , p.theta()
          );

    positionProxy->SetSpeed(0, 0);
    usleep(100000);
    return pose();
}
