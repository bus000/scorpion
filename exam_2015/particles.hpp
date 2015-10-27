#ifndef PARTICLES_H
#define PARTICLES_H

#include <vector>
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
#include "random_numbers.h"

/*
 * Some colors
 */
#define CRED     CV_RGB(255, 0, 0)
#define CGREEN   CV_RGB(0, 255, 0)
#define CBLUE    CV_RGB(0, 0, 255)
#define CCYAN    CV_RGB(0, 255, 255)
#define CYELLOW  CV_RGB(255, 255, 0)
#define CMAGENTA CV_RGB(255, 0, 255)
#define CWHITE   CV_RGB(255, 255, 255)
#define CBLACK   CV_RGB(0, 0, 0)

/*
 * Landmarks.
 * The robot knows the position of 2 landmarks. Their coordinates are in cm.
 */
#define num_landmarks 2
const CvPoint landmarks [num_landmarks] = {
    cvPoint (0, 0),
    cvPoint (300, 0),
};

/**
 * Data structure for storing particle information (state and weight)
 */
class particle {
    public:
        // Constructor
        particle() {};
        particle(double _x, double _y, double _theta = 0, double _weight = 0) {
            x = _x; y = _y; theta = _theta; weight = _weight;
        };
        // Data
        double x;
        double y;
        double theta;
        double weight;
};

/**
 * Estimate the pose from particles by computing the average position and orientation over all particles.
 * This is not done using the particle weights, but just the sample distribution.
 */
particle estimate_pose(std::vector<particle> &particles);


void move_particle(particle &p, double delta_x, double delta_y, double delta_theta);

void add_uncertainty(std::vector<particle> &particles, double sigma, double theta_kappa);

void uncertain_particle(particle &p, double sigma, double theta_kappa);

void draw_world (particle &est_pose, std::vector<particle> &particles, IplImage *im);

CvScalar jet (const double x);

#endif
