#ifndef MEASUREMENT_H
#define MEASUREMENT_H

#include <opencv2/core/core.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <pthread.h>

#include <stdio.h>
#include <iostream>
#include <cmath>

#include "camera.h"
#include "particles.h"

typedef enum {
  NoLandmark,
  RedLandmark,
  GreenLandmark,
  BothLandmarks
} ObservedLandmark;

class measurement {
public:
    particle position;
    double distance;
    double angle;
    ObservedLandmark landmark;

    measurement(camera &cam, IplImage *im);

    measurement(particle position, double distance, double angle) {
        this->position = position;
        this->distance = distance;
        this->angle    = angle;
    }
    measurement() {
        this->landmark = NoLandmark;
    }

    ~measurement(void);
};

#endif
