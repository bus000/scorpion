#ifndef _PARTICLE_FILTER_
#define _PARTICLE_FILTER_

#include "random_numbers.h"
#include "particles.h"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <vector>
#include <string>
#include <stdlib.h>
#include "math.h"

using namespace std;
using namespace cv;

class measurement {
public:
  particle position;
  double distance;
  double angle;

  measurement() {};
  measurement(particle position, double distance, double angle) {
    this->position = position;
    this->distance = distance;
    this->angle    = angle;
  }

  ~measurement() {};
};

void mclFilter(particle pose, particle command, measurement meas, vector<particle> &previous);

#endif
