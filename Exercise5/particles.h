#ifndef PARTICLES_H
#define PARTICLES_H

#include <vector>
#include "cxcore.h"

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

#endif
