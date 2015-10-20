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
#include "measurement.h"

using namespace std;
using namespace cv;

void mclFilter(particle command, measurement meas, vector<particle> &previous);

#endif
