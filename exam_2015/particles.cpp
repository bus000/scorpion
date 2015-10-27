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

  // FUCK VON MISIS TIS' SOME REAL OG SHIT
  p.theta += randn(0, theta_kappa);
  
  if (p.theta < -M_PI) {
    p.theta += 2*M_PI;
  }
  else if (p.theta > M_PI) {
    p.theta -= 2*M_PI;
  }
}

void draw_world (particle &est_pose, std::vector<particle> &particles, IplImage *im)
{
    const int offset = 100;

    // White background
    cvSet (im, CWHITE);

    // Find largest weight
    const int len = particles.size ();
    double max_weight = particles [0].weight;
    for (int i = 1; i < len; i++)
        max_weight = std::max (max_weight, particles [i].weight);

    // Draw particles
    for (int i = 0; i < len; i++)
    {
        const int x = (int)particles[i].x + offset;
        const int y = (int)particles[i].y + offset;
        const CvScalar colour = jet (particles[i].weight / max_weight);
        cvCircle (im, cvPoint (x,y), 2, colour, 2);
        const CvPoint b = cvPoint ((int)(particles[i].x + 15.0*cos(particles[i].theta))+offset,
                (int)(particles[i].y + 15.0*sin(particles[i].theta))+offset);
        cvLine   (im, cvPoint (x,y), b, colour, 2);
    }

    // Draw landmarks
    const CvPoint lm0 = cvPoint (landmarks[0].x+offset, landmarks[0].y+offset);
    const CvPoint lm1 = cvPoint (landmarks[1].x+offset, landmarks[1].y+offset);
    cvCircle (im, lm0, 5, CRED, 2);
    cvCircle (im, lm1, 5, CGREEN, 2);

    // Draw estimated robot pose
    const CvPoint a = cvPoint ((int)est_pose.x+offset, (int)est_pose.y+offset);
    const CvPoint b = cvPoint ((int)(est_pose.x + 15.0*cos(est_pose.theta))+offset,
            (int)(est_pose.y + 15.0*sin(est_pose.theta))+offset);
    cvCircle (im, a, 5, CMAGENTA, 2);
    cvLine   (im, a, b, CMAGENTA, 2);
}

/*
 * Colour map for drawing particles. This function determines the colour of a
 * particle from its weight.
 */
CvScalar jet (const double x)
{
    const double r = (x >= 3.0/8.0 && x < 5.0/8.0) * (4.0 * x - 3.0/2.0)
        + (x >= 5.0/8.0 && x < 7.0/8.0)
        + (x >= 7.0/8.0) * (-4.0 * x + 9.0/2.0);
    const double g = (x >= 1.0/8.0 && x < 3.0/8.0) * (4.0 * x - 1.0/2.0)
        + (x >= 3.0/8.0 && x < 5.0/8.0)
        + (x >= 5.0/8.0 && x < 7.0/8.0) * (-4.0 * x + 7.0/2.0);
    const double b = (x < 1.0/8.0) * (4.0 * x + 1.0/2.0)
        + (x >= 1.0/8.0 && x < 3.0/8.0)
        + (x >= 3.0/8.0 && x < 5.0/8.0) * (-4.0 * x + 5.0/2.0);

    return CV_RGB (255.0*r, 255.0*g, 255.0*b);
}
