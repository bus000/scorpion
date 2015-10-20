#include "stateMachine.hpp"

State TurnToFirstLandmark(State state);
State DriveToFirstLandmark(State state);
State TurnToSecondLandmark(State state);
State DriveToFinishPosition(State state);
State DriveToOtherSide(State state);

State RunState(State state) {
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
State TurnToFirstLandmark(State state) {
  // TODO: Turn the robot until a landmark is found.

  // TODO: Determine landmark from color
  ObservedLandmark landmark = GreenLandmark;

  state.currentStep     = GotoLandmark;
  state.currentLandmark = landmark;

  return state;
}

// Step 2: Positions the robot at a right angle 150 cm from the landmark.
State DriveToFirstLandmark(State state) {
  // TODO: Drive to landmark position.
  
  state.currentStep = SecondSearch;
  
  return state;  
}

// Step 3: Searches for the second landmark.
State TurnToSecondLandmark(State state) {
  // TODO: Turn the robot until a new landmark is found
  
  if (true) { // TODO: If a new landmark was found
    state.currentStep     = GotoFinish;
    state.currentLandmark = BothLandmarks;
  }
  else {
    state.currentStep = GotoOtherSide;
  }

  return state;
}

// Step 3.1: Drives to the other side of the current landmark.
State DriveToOtherSide(State state) {
  // TODO: 
  // Reset odometry.
  // Drive to (150, 150).
  // Turn -theta.
  // Drive to (150, -150).
  
  // Maybe we should go to SecondSearch?
  state.currentStep = GotoFinish;

  return state;
}
