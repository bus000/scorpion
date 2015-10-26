#include "stateMachine.hpp"

State TurnToFirstLandmark(State &state);
State DriveToFirstLandmark(State &state);
State TurnToSecondLandmark(State &state);
State DriveToFinishPosition(State &state);
State DriveToOtherSide(State &state);

State RunState(State &state) {
  printf("State: %s\n", state.currentStep);

  state.estimatedPose = estimate_pose(state.particles);

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
  // TODO: Particle filter.

  control->resetCounters();

  printf("Found first landmark at (%d, %d, %d)\n", meas->position.x, meas->position.y, meas->angle);


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
  
  if (found) {
    measurement *meas = state.lastMeas;
    printf("Found second landmark at (%d, %d, %d)\n", meas->position.x, meas->position.y, meas->angle);

    // TODO: Particle filter with new landmark.
    control->resetCounters();

    state.currentStep     = GotoFinish;
    state.currentLandmark = BothLandmarks;
  }
  else {
    control->resetCounters();
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

measurement* getMeasurement(State &state) {
  return (new measurement(state.cam, state.image));
}

char d_str1[] = "FirstSearch";
char d_str2[] = "GotoLandmark";
char d_str3[] = "SecondSearch";
char d_str4[] = "GotoFinish";
char d_str5[] = "GotoOtherSide";
char d_str6[] = "Unknown state";

char *TaskString(TaskStep step) {
  switch(step) {
    case FirstSearch:
        return d_str1;
    case GotoLandmark:
        return d_str2;
    case SecondSearch:
        return d_str3;
    case GotoFinish:
        return d_str4;
    case GotoOtherSide:
        return d_str5;
    default:
        return d_str6;
  }
}
