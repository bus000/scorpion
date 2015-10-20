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

/*
 * struct for particle thread job
 */
struct particle_pack {
    particle command;
    measurement meas;
    vector<particle> parts;
};

void* startMclFilter(void *data){
    struct particle_pack *data_p = (struct particle_pack*)data;
    mclFilter(
        data_p->command,
        data_p->meas,
        data_p->parts
    );
}

particle est_pack_pos(struct particle_pack *data, int length){
    particle result(0,0);
    for(int i = 0; i < length; i++){
        particle tmp = estimate_pose(data[i].parts);
        result.x += tmp.x;
        result.y += tmp.y;
        result.theta += tmp.theta;
    }

    result.x /= length;
    result.y /= length;
    result.theta /= length;
    return result;
}

std::vector<particle> join_particles(struct particle_pack *data, int length){
    std::vector<particle> tmp_p;
    
    //this is dume and comment it out when running the robot
    std::vector<particle> tmp;
    for(int i = 0; i < length; i++){
        for(int a = 0; a < data[i].parts.size(); a++)
            tmp_p.push_back(data[i].parts[a]);
    }
    return tmp_p;
}

/*************************\
 *      Main program     *
 \*************************/
int main() 
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
    //Particle threads
    struct particle_pack thread_data[NUM_THREADS];

    for(int i = 0; i < NUM_THREADS; i++){
        int p_count = (num_particles/NUM_THREADS);
        if(i == NUM_THREADS-1)
            p_count += num_particles%NUM_THREADS;

        for(int a = 0; a < p_count; a++){
            particle tmp_p(2000.0*randf() - 1000, 2000.0*randf() - 1000);
            tmp_p.theta = 2.0*M_PI*randf() - M_PI;
            tmp_p.weight = 1.0/(double)num_particles;
            thread_data[i].parts.push_back(tmp_p);
        }
    }

    // The estimate of the robots current pose
    particle est_pose = est_pack_pos (thread_data, NUM_THREADS);

    // The camera interface  
    camera cam;

    // Parameters
    const CvSize size = cvSize (480, 360);

    // Draw map
    std::vector<particle> tmp_p = join_particles(thread_data, NUM_THREADS);
    draw_world (est_pose, tmp_p, world);

    //command should be updated every time we move the robot
    particle command(0.0,0.0);

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


            particle zeroP(0, 0);
            measurement meas(est_pose, measured_distance, measured_angle);

            pthread_t threads[NUM_THREADS];
            for(int i = 0; i < NUM_THREADS; i++){
                thread_data[i].command = command;
                thread_data[i].meas = meas;
                pthread_create(&threads[i], NULL, startMclFilter,
                        (void*)&thread_data[i]);
            }

            for(int i = 0; i < NUM_THREADS; i++)
                pthread_join(threads[i], NULL);

            // Compute particle weights
            // XXX: You do this

            // Resampling
            // XXX: You do this

            // Draw the object in the image (for visualisation)
            cam.draw_object (im);

        } else { // end: if (found_landmark)

            // No observation - reset weights to uniform distribution
            for (int i = 0; i < NUM_THREADS; i++)
            {
                for(int a = 0; a < thread_data[i].parts.size(); a++)
                    thread_data[i].parts[a].weight = 1.0/(double)num_particles;
            }

        }  // end: if (not found_landmark)

        // Estimate pose
        est_pose = est_pack_pos (thread_data, NUM_THREADS);                   

        // Visualisation
        std::vector<particle> tmp_p = join_particles(thread_data, NUM_THREADS);
        draw_world (est_pose, tmp_p, world);
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
