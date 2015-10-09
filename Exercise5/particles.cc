#include <vector>
#include "particles.h"
#include "random_numbers.h"
#include <math.h>
#include <iostream>

particle estimate_pose (std::vector<particle> &particles)
{
    double x_sum = 0, y_sum = 0, cos_sum = 0, sin_sum = 0;
    const int len = particles.size ();
    for (int i = 0; i < len; i++)
      {
        x_sum += particles[i].x;
        y_sum += particles[i].y;
        cos_sum += cos (particles[i].theta);
        sin_sum += sin (particles[i].theta);
      }
    const double flen = (double)len;
    const double x = x_sum/flen;
    const double y = y_sum/flen;
    const double theta = atan2 (sin_sum/flen, cos_sum/flen);
    return particle (x, y, theta);
}

// XXX: You implement this
// Done by N-2The-I-2The-C-2The-K-2The-L-2The-A-2The-S
// Nicklas!!!!!!!!!! (not correctly though)
// Finished by P*E*L*L*E
void move_particle (particle &p, double delta_x, double delta_y, double delta_theta)
{
    p.x += delta_x;
    p.y += delta_y;
    p.theta += delta_theta;
}

void add_uncertainty (std::vector<particle> &particles, double sigma, double theta_kappa)
{
    const int len = particles.size();
    for (int i = 0; i < len; i++)
      {
        uncertain_particle(particles[i], sigma, theta_kappa);
      }
}

void uncertain_particle(particle &p, double sigma, double theta_kappa) {
  p.x += randn (0, sigma);
  p.y += randn (0, sigma);
  p.theta = fmod(rand_von_mises (p.theta, theta_kappa), 2.0f * M_PI) - M_PI;
}

