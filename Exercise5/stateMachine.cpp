#include "stateMachine.hpp"

State TurnToFirstLandmark(State &state);
State DriveToFirstLandmark(State &state);
State TurnToSecondLandmark(State &state);
State DriveToFinishPosition(State &state);
State DriveToOtherSide(State &state);

State RunState(State &state) {
  switch(state.currentStep) {
    case FirstSearch:
      return TurnToFirstLandmark(state);
    case GotoLandmark:
      return DriveToFirstLandmark(state);
    case SecondSearch:
      return TurnToSecondLandmark(state);
    case GotoFinish:
      return DriveToFinishPosition(state);
    case GotoOtherSide:
      return DriveToOtherSide(state);
    default:
      return state;
  }
}

// Step 1: Searches for the first landmark.
State TurnToFirstLandmark(State &state) {
  DriveCtl *control = state.driveControl;
  measurement *meas;

  // Turn the robot until a landmark is found.
  while (true) {
    control->turnLeft(15);

    meas = getMeasurement(state);

    if (meas->landmark != NoLandmark) {
      state.lastMeas = meas;
      break;
    }
    else
      delete meas;
  }
  control->resetCounters();

  // TODO: Particle filter.

  state.currentStep     = GotoLandmark;
  state.currentLandmark = meas->landmark;

  return state;
}

// Step 2: Positions the robot at a right angle 150 cm from the landmark.
State DriveToFirstLandmark(State &state) {
  DriveCtl *control = state.driveControl;
  
  double dist = state.lastMeas->distance; 
  double angl = state.lastMeas->angle;
  particle pos = state.lastMeas->position;

  // TODO: RADS
  double posX = cos(angl) * dist;
  double posY = sin(angl) * dist - 150;

  double dot = pos.x * posX + pos.y + posY; 
  double det = pos.x * posY - pos.y + posX; 
  double recoverAngl = atan2(det, dot);

  // Drive to landmark position.
  control->resetCounters();
  control->goToPos(posX, posY);
  control->turn(recoverAngl);
  control->resetCounters();

  state.currentStep = SecondSearch;
  
  return state;  
}

// Step 3: Searches for the second landmark.
State TurnToSecondLandmark(State &state) {
  DriveCtl *control = state.driveControl;

  bool found = false;

  // TODO: Turn the robot until a new landmark is found
  control->resetCounters();
  while (control->getYawed() < 355) { // MAGIC NUMBER
    control->turnLeft(15);

    measurement *meas = getMeasurement(state);

    if (meas->landmark != NoLandmark && meas->landmark != state.currentLandmark) {
      state.lastMeas = meas;
      found = true;
      break;
    }
    else
      delete meas;
  }
  control->resetCounters();
  
  if (found) {
    // TODO: Particle filter with new landmark.

    state.currentStep     = GotoFinish;
    state.currentLandmark = BothLandmarks;
  }
  else {
    state.currentStep = GotoOtherSide;
  }

  return state;
}

// Step 3.1: Drives to the other side of the current landmark.
State DriveToOtherSide(State &state) {
  DriveCtl *control = state.driveControl;

  control->resetCounters();
  control->goToPos(150, 150);
  control->turnLeft(control->getYawed());
  control->resetCounters();
  control->goToPos(150, -150);
  control->resetCounters();
  
  // Maybe we should go to SecondSearch?
  state.currentStep = GotoFinish;

  return state;
}

State DriveToFinishPosition(State &state) {
  DriveCtl *control = state.driveControl;

  particle pose = estimate_pose(state.particles);

  control->setXPos(pose.x);
  control->setYPos(pose.y);
  control->setYaw(pose.theta);

  control->goToPos(0, 150);
  control->resetCounters();

  // DONE
  // TODO: Check position
}

void PrintState(State state) {
  // TODO: finish this
  printf("Task:\t");
}

measurement* getMeasurement(State &state) {
  return (new measurement(state.cam, state.image));
}
