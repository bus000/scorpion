#ifndef _STATE_
#define _STATE_

#include <opencv2/core/core.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "particles.h"
#include "measurement.h"
#include "DriveCtl.hpp"
#include "math.h"
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
    measurement *lastMeas;
    ObservedLandmark currentLandmark;
    DriveCtl *driveControl;
    camera cam;
    IplImage image;
    

    State( vector<particle> particles
         , PlayerCc::PlayerClient *robot
         , PlayerCc::Position2dProxy *position
         , camera &cam
         , IplImage &im
         , particle position)
    {
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

measurement getMeasurement(State state);

#endif
