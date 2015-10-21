#ifndef _STATE_
#define _STATE_

#include "particles.h"
#include "DriveCtl.hpp"
#include <libplayerc++/playerc++.h>

using namespace std;

enum ObservedLandmark {
  NoLandmark,
  RedLandmark,
  GreenLandmark,
  BothLandmarks
};

enum TaskStep {
  FirstSearch,
  GotoLandmark,
  SecondSearch,
  GotoFinish,
  GotoOtherSide
};

class State {
  public:
    vector<particle> particles;
    particle estimatedPose;
    TaskStep currentStep;
    ObservedLandmark currentLandmark;
    DriveCtl *driveControl;

    State(vector<particle> particles, PlayerCc::PlayerClient *robot, PlayerCc::Position2dProxy *position) {
      this->particles = particles;
      this->currentStep = FirstSearch;
      this->currentLandmark = NoLandmark;
      this->driveControl = new DriveCtl(robot, position);
      this->estimatedPose = estimate_pose(this->particles);
    }

    ~State() {
      // NOP
    }
};

#endif
