#include "stateMachine.hpp"
#include <iostream>

StateMachine::StateMachine(DriveCtl *driveCtl, vector<particle> *particles) {
    this->driveCtl = driveCtl;
    this->particles = particles;
    this->foundRed = false;
    this->foundGreen = false;
    this->initialTotalYawed = 0;
    this->currentState = STATE_INITIAL;
    this->filter = new particleFilter(1);
}

StateMachine::~StateMachine(){
    delete this->filter;
}

void StateMachine::run(vector<measurement> meas) {
    switch(this->currentState) {
        case STATE_INITIAL:
            this->currentState = this->run_initial(meas);
            break;
        case STATE_FOUND_BOTH:
            this->currentState = this->run_found_both(meas);
            break;
    }
}

void StateMachine::updateParticleFilter(measurement meas) {
    double yaw = driveCtl->toRadians(driveCtl->getYaw());
    if(yaw > M_PI)
        yaw -= 2.0*M_PI;
    particle command(driveCtl->getXPos(), driveCtl->getYPos(), yaw*-1);
    this->filter->filter(command, meas, this->particles);
    this->driveCtl->setXPos(0.0);
    this->driveCtl->setYPos(0.0);
    this->driveCtl->setYaw(0.0);
    this->driveCtl->resetCounters();
}

state_t StateMachine::run_initial(vector<measurement> meas) {
    for(int i = 0; i < meas.size(); i++){
        this->updateParticleFilter(meas[i]);
        switch(meas[i].landmark) {
            case NoLandmark:
                break;
            case RedLandmark:
                this->foundRed = true;
                break;
            case GreenLandmark:
                this->foundGreen = true;
                break;
            default:
                cout << "run initial error" << endl << flush;
        }
    }

    if(this->foundRed && this->foundGreen){
        this->initialTotalYawed = 0;
        return STATE_FOUND_BOTH;
    }else if(this->initialTotalYawed > 2.0*M_PI){
        this->initialTotalYawed = 0;
        return STATE_PANIC;
    }

    //this is in degrees (sorry)
    this->driveCtl->turn(5);
    this->initialTotalYawed += driveCtl->toRadians(driveCtl->getYawed());

    return STATE_INITIAL;
}

state_t StateMachine::run_found_both(vector<measurement> meas) {
    for(int i = 0; i < meas.size()*3; i++)
        this->updateParticleFilter(meas[i%meas.size()]);

    particle est_pose = estimate_pose(*this->particles);

    if(est_pose.theta < 0.0)
        est_pose.theta += 2.0*M_PI;

    //cout << "we are here: (" << est_pose.
    this->driveCtl->setYaw(this->driveCtl->toDegrees(est_pose.theta));
    this->driveCtl->setXPos(est_pose.x);
    this->driveCtl->setYPos(est_pose.y);
    this->driveCtl->goToPos(150.0, 0);
    //double tx = (150.0-est_pose.x)*0.8;
    //double ty = est_pose.y*0.8;

    //this->driveCtl->goToPos(tx, ty);

    this->foundRed = false;
    this->foundGreen = false;
    this->initialTotalYawed = 0.0;
    return STATE_INITIAL;
}
