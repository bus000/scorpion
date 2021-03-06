#ifndef _STATE_
#define _STATE_

#include <opencv2/core/core.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "particles.h"
#include "measurement.hpp"
#include "DriveCtl.hpp"
#include "math.h"
#include <libplayerc++/playerc++.h>
#include "particleFilter.hpp"

using namespace std;

enum TaskStep {
  FirstSearch,
  GotoLandmark,
  SecondSearch,
  GotoFinish,
  GotoOtherSide
};

char *TaskString(TaskStep step);

class State {
  public:
    vector<particle> *particles;
    particle estimatedPose;
    TaskStep currentStep;
    measurement lastMeas;
    ObservedLandmark currentLandmark;
    DriveCtl *driveControl;
    particleFilter *filter;
    camera *cam;
    IplImage *image;
    

    State( vector<particle> *particles
         , particleFilter *filter
         , PlayerCc::PlayerClient *robot
         , PlayerCc::Position2dProxy *position
         , camera *cam
         )
    {
      this->particles = particles;
      this->currentStep = FirstSearch;
      this->currentLandmark = NoLandmark;
      this->driveControl = new DriveCtl(robot, position);
      this->estimatedPose = estimate_pose(*this->particles);
      this->filter = filter;
      this->cam = cam;
    }

    ~State() {
      // NOP
    }
};

measurement getMeasurement(State &state);

#endif
