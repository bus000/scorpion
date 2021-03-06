#include <opencv2/core/core.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <pthread.h>
#include <ctime>
#include <sys/time.h>

#include <stdio.h>
#include <iostream>
#include <cmath>

#include "camera.h"
#include "particles.h"
#include "random_numbers.h"
#include "particleFilter.hpp"
#include "state.hpp"
#include "stateMachine.hpp"
#include <unistd.h>

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

void test(){
    measurement meas(particle(0.0,0.0), 1.0, 0.0);
    particle par[5];

    par[0] = particle(1.0, 0.0, M_PI, 1.0);
    par[1] = particle(1.0, 0.0, M_PI/2.0, 1.0);
    par[2] = particle(1.0, 0.0, M_PI/3.0, 1.0);
    par[3] = particle(1.0, 0.0, M_PI/4.0, 1.0);
    par[4] = particle(1.0, 0.0, M_PI/5.0, 1.0);
    par[5] = particle(1.0, 0.0, M_PI/6.0, 1.0);

    particleFilter::thread_data_t data;
    data.particles = par;
    data.length = 5;

    particleFilter::mutual_data_t mData;
    mData.meas = &meas;
    pthread_mutex_init(&mData.lock_mean, NULL);
    mData.angleSum = 0;
    mData.a_mean_thread_count = 0;
    mData.numThreads = 1;
    pthread_cond_init(&mData.cond_mean, NULL);
    mData.totalLength = 5;
    pthread_mutex_init(&mData.lock_variance, NULL);
    mData.a_variance = 0;
    mData.a_variance_thread_count = 0;
    pthread_cond_init(&mData.cond_variance, NULL);

    data.mutualData = &mData;
    particleFilter::angleObservationModel(&data);

    for(int i = 0; i < 5; i++){
        cout << "Weight " << i << ": " << par[i].weight << endl;
    }
}

void test2(DriveCtl *driveCtl){
    //driveCtl->turnLeft(90.0);
    //usleep(1000000);
    //driveCtl->turnRight(90.0);
    //usleep(1000000);

    driveCtl->goToPos(150.0,50.0);
    usleep(1000000);
    driveCtl->goToPos(100.0,100.0);
    usleep(1000000);
    driveCtl->goToPos(100.0,0.0);
    usleep(1000000);
    driveCtl->goToPos(0.0,100.0);
    usleep(1000000);
    driveCtl->goToPos(0.0,0.0);
    usleep(1000000);
    driveCtl->turn(180.0);
}

int main() {
 // test();
 // return 0;
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
  const int num_particles = 30000;
  std::vector<particle> particles;

  for(int i = 0; i < num_particles; i++){
      particle tmp_p(2000.0*randf() - 1000, 2000.0*randf() - 1000);
      tmp_p.theta = 360.0 * randf();
      tmp_p.weight = 1.0/(double)num_particles;
      particles.push_back(tmp_p);
  }

  // Initialize robot and position proxy.
  //PlayerCc::PlayerClient robot("localhost");
  PlayerCc::PlayerClient robot("192.168.100.253");
  PlayerCc::Position2dProxy position(&robot);
  DriveCtl driveCtl(&robot, &position);

  // Initialize state.
  StateMachine state(&driveCtl, &particles);

  /* -- MAIN LOOP -- */
  while (true) {
    vector<measurement> meas;
    for(int i = 0; i < 1; i++){
        cam.get_colour();
        IplImage *im = cam.get_colour();
        measurement tmp(cam, im);
        meas.push_back(tmp);
        if(tmp.landmark != NoLandmark)
            cam.draw_object (im);
        cvShowImage (window, im);
    }
    int action = cvWaitKey (10);
    // Run state machine.
    state.run(meas);

    // Draw map.
    particle est_pose = estimate_pose(particles);
    cout << "(" << est_pose.x << ", " << est_pose.y << ")" << endl << endl;
    draw_world (est_pose, particles, world);
    cvShowImage (map, world);
  }
}

#ifdef _THIS_IS_NOT_DEFINED_

/*************************\
 *      Main program     *
 \*************************/
#if 0
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
    const int num_particles = 200000;
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
    particleFilter pFilter(1);

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

            struct timeval tv;
            gettimeofday(&tv,NULL);
            unsigned long start = 1000000 * tv.tv_sec + tv.tv_usec;
            //zeroP should be replaced with the landmark position
            //we are observing
            particle zeroP(0, 0);
            measurement meas(zeroP, measured_distance, measured_angle);
            meas.landmark = RedLandmark;
            pFilter.filter(command, meas, &particles);

            gettimeofday(&tv, NULL);
            long unsigned diff = (1000000 * tv.tv_sec + tv.tv_usec) - start;
            printf("Exec time: %d\n", diff);


            printf("Robot angle: %f\n", est_pose.theta * 180/M_PI);

            // Draw the object in the image (for visualisation)
            cam.draw_object (im);

        } else { // end: if (found_landmark)
            struct timeval tv;
            gettimeofday(&tv,NULL);
            unsigned long start = 1000000 * tv.tv_sec + tv.tv_usec;


            particle zeroP(0, 0);
            measurement meas(zeroP, 0, 0);
            meas.landmark = NoLandmark;
            pFilter.filter(command, meas, &particles);
            
            gettimeofday(&tv, NULL);
            long unsigned diff = (1000000 * tv.tv_sec + tv.tv_usec) - start;
            printf("Exec time: %d\n", diff);


        }  // end: if (not found_landmark)

        // Estimate pose
<<<<<<< HEAD
        est_pose =  estimate_pose(particles);                   
        printf("robot: (%f, %f)\n", est_pose.x, est_pose.y);
||||||| merged common ancestors
        est_pose =  estimate_pose(particles);                   
=======
        est_pose =  estimate_pose(particles);
>>>>>>> 9e8b90cb363eacadc57b133e089a928dfcdf0d9d

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
#endif
#endif
