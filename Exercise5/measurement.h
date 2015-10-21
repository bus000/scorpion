#ifndef MEASUREMENT_H
#define MEASUREMENT_H

#include <opencv2/core/core.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <pthread.h>
#include "state.hpp"

#include <stdio.h>
#include <iostream>
#include <cmath>

#include "camera.h"

class measurement {
public:
    particle position;
    double distance;
    double angle;
    ObservedLandmark landmark;
    vector<double> observedPosition;

    measurement(camera &cam, IplImage &im, particle position);

    measurement(particle position, double distance, double angle) {
        this->position = position;
        this->distance = distance;
        this->angle    = angle;
    }

    ~measurement() {};
};


#endif
