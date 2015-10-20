#ifndef _STATE_
#define _STATE_

#include "particles.h"

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
    TaskStep currentStep;
    ObservedLandmark currentLandmark;

    State(vector<particle> particles) {
      this->particles = particles;
      this->currentStep = FirstSearch;
      this->currentLandmark = NoLandmark;
    };

    ~State() {
      // NOP
    }
};

#endif
