#include "stateMachine.hpp"
#include <iostream>
#include <unistd.h>

StateMachine::StateMachine(DriveCtl *driveCtl, vector<particle> *particles) {
    this->driveCtl = driveCtl;
    this->particles = particles;
    this->foundRed = false;
    this->foundGreen = false;
    this->initialTotalYawed = 0;
    this->currentState = STATE_INITIAL;
    this->filter = new particleFilter(1);
    this->lookMoreCounter = 0;
    this->flushCount = 0;
    this->deltaTheta = 0;
    this->deltaX = 0;
    this->deltaY = 0;
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
        case STATE_LOOK_MORE:
            this->currentState = this->run_lookMore(meas);
            break;
        case STATE_FLUSH:
            this->currentState = this->run_flush(meas);
            break;
    }
}

void StateMachine::updateParticleFilter(measurement meas) {
    double yaw = driveCtl->toRadians(driveCtl->getYaw());
    if(yaw > M_PI)
        yaw -= 2.0*M_PI;
    particle command(this->deltaX, this->deltaY, -1.0*this->deltaTheta);
    this->filter->filter(command, meas, this->particles);
    this->deltaTheta = 0;
    this->deltaX = 0;
    this->deltaY = 0;
}

state_t StateMachine::run_lookMore(vector<measurement> meas){
    if(this->lookMoreCounter == 15){
        this->lookMoreCounter = 0;
        return STATE_INITIAL;
    }
    this->lookMoreCounter++;

    for(int i = 0; i < meas.size(); i++)
        this->updateParticleFilter(meas[i]);
    return STATE_LOOK_MORE;
}

state_t StateMachine::run_flush(vector<measurement> meas){
    if(this->flushCount == 5){
        this->flushCount = 0;
        return STATE_INITIAL;
    }

    this->flushCount++;

    return STATE_FLUSH;
}

state_t StateMachine::run_initial(vector<measurement> meas) {
    for(int i = 0; i < meas.size(); i++){
        this->updateParticleFilter(meas[i]);
        switch(meas[i].landmark) {
            case NoLandmark:
                break;
            case RedLandmark:
                if(!this->foundRed){
                    this->foundRed = true;
                    return STATE_LOOK_MORE;
                }
                break;
            case GreenLandmark:
                if(!this->foundGreen){
                    this->foundGreen = true;
                    return STATE_LOOK_MORE;
                }
                break;
            default:
                cout << "run initial error" << endl << flush;
        }
    }

    if(this->foundRed && this->foundGreen){
        this->initialTotalYawed = 0;
        return STATE_FOUND_BOTH;
    }

    //this is in degrees (sorry)
    this->driveCtl->turn(5);
    this->deltaTheta = this->driveCtl->toRadians(5.0);
    usleep(500000);
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

    this->deltaTheta = est_pose.theta -
        this->driveCtl->toRadians(this->driveCtl->getYaw());
    this->deltaX = est_pose.x - driveCtl->getXPos();
    this->deltaY = est_pose.y - driveCtl->getYPos();

    this->foundRed = false;
    this->foundGreen = false;
    this->initialTotalYawed = 0.0;
    return STATE_FLUSH;
}
