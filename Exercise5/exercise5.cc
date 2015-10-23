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
#include "particleFilter.hpp"
#include "state.hpp"
#include "stateMachine.hpp"

/*
 * Keyboard constants
 */
#define KEY_UP    65362
#define KEY_DOWN  65364
#define KEY_LEFT  65361
#define KEY_RIGHT 65363

/*
 * Number of particle threads
 */
#define NUM_THREADS 8

int main() {
  // The GUI
  const char *map = "World map";
  const char *window = "Robot View";
  IplImage *world = cvCreateImage (cvSize (500,500), IPL_DEPTH_8U, 3);
  cvNamedWindow (map, CV_WINDOW_AUTOSIZE);
  cvNamedWindow (window, CV_WINDOW_AUTOSIZE);
  cvMoveWindow (window, 20, 20);

  // The camera interface  
  camera cam;

  // Parameters
  const CvSize size = cvSize (480, 360);

  // Initialize particles
  const int num_particles = 1000;
  std::vector<particle> particles;

  for(int i = 0; i < num_particles; i++){
      particle tmp_p(2000.0*randf() - 1000, 2000.0*randf() - 1000);
      tmp_p.theta = 360.0 * randf();
      tmp_p.weight = 1.0/(double)num_particles;
      particles.push_back(tmp_p);
  }

  // Initialize robot and position proxy.
  PlayerClient robot("192.168.240.129");
  Position2dProxy position(&robot);
  
  // Initialize state.
  State state(particles, &robot, &position, &cam, world, estimate_pose(particles));

  /* -- MAIN LOOP -- */
  while (true) {
    // Run state machine.
    RunState(state);

    // Draw map.
    draw_world (est_pose, particles, world);
    cvShowImage (map, world);
    cvShowImage (window, im);
    cam.draw_object (im);

    // TODO: Maybe run particle filter?
  } 
}

/*************************\
 *      Main program     *
 \*************************/
int oldmain() 
{
    // The GUI
    const char *map = "World map";
    const char *window = "Robot View";
    IplImage *world = cvCreateImage (cvSize (500,500), IPL_DEPTH_8U, 3);
    cvNamedWindow (map, CV_WINDOW_AUTOSIZE);
    cvNamedWindow (window, CV_WINDOW_AUTOSIZE);
    cvMoveWindow (window, 20, 20);


    // Initialize particles
    const int num_particles = 1000;
    std::vector<particle> particles;

    for(int i = 0; i < num_particles; i++){
        particle tmp_p(2000.0*randf() - 1000, 2000.0*randf() - 1000);
        tmp_p.theta = 2.0*M_PI*randf() - M_PI;
        tmp_p.weight = 1.0/(double)num_particles;
        particles.push_back(tmp_p);
    }

    // The estimate of the robots current pose
    particle est_pose = estimate_pose (particles);

    // The camera interface  
    camera cam;

    // Parameters
    const CvSize size = cvSize (480, 360);

    // Draw map
    draw_world (est_pose, particles, world);

    //command should be updated every time we move the robot
    particle command(0.0,0.0);
    particleFilter pFilter(8);

    // Main loop
    while (true)
    {
        // Move the robot according to user input
        int action = cvWaitKey (10);

        //XXX: Make player drive

        // Read odometry, see how far we have moved, and update particles.
        // Or use motor controls to update particles
        //XXX: You do this

        // Grab image
        IplImage *im = cam.get_colour ();
        //rgb_im = cam.get_colour ();
       
        // Do landmark detection
        double measured_distance, measured_angle;
        colour_prop cp;
        object::type ID = cam.get_object (im, cp, measured_distance, measured_angle);
        if (ID != object::none)
        {
            printf ("Measured distance: %f\n", measured_distance);
            printf ("Measured angle:    %f\n", measured_angle);
            printf ("Colour probabilities: %.3f %.3f %.3f\n", cp.red, cp.green, cp.blue);

            if (ID == object::horizontal)
            {
                printf ("Landmark is horizontal\n");
            }
            else if (ID == object::vertical)
            {
                printf ("Landmark is vertical\n");
            }
            else
            {
                printf ("Unknown landmark type!\n");
                continue;
            }


            //zeroP should be replaced with the landmark position
            //we are observing
            particle zeroP(0, 0);
            measurement meas(zeroP, measured_distance, measured_angle);
            pFilter.filter(est_pose, command, meas, &particles);

            // Draw the object in the image (for visualisation)
            cam.draw_object (im);

        } else { // end: if (found_landmark)

            // No observation - reset weights to uniform distribution
            for (int i = 0; i < num_particles; i++)
            {
                particles[i].weight = 1.0/(double)num_particles;
            }

        }  // end: if (not found_landmark)

        // Estimate pose
        est_pose =  estimate_pose(particles);                   

        // Visualisation
        draw_world (est_pose, particles, world);
        cvShowImage (map, world);
        cvShowImage (window, im);
    } // End: while (true)

theend:
    // Stop the robot
    // position.SetSpeed(0.0, 0.0);

    // Clean up and return
    //cvReleaseImage (&rgb_im);
    //cvReleaseImage (&im);

    return 0;
}
