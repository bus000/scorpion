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
#include <pthread.h>

using namespace std;
using namespace cv;

class measurement {
public:
  particle position;
  double distance;
  double angle;

  measurement(int numThreads);
  measurement(particle position, double distance, double angle) {
    this->position = position;
    this->distance = distance;
    this->angle    = angle;
  }

  ~measurement() {};
};

class particleFilter {
public:
    particleFilter(int numThreads);
    ~particleFilter();
    void filter(particle position, particle command, measurement meas,
            vector<particle> *parts);
    static void* threadStart(void *data);

    struct mutual_data_t {
        //Only alive while filter is running
        particle *position;
        particle *command;
        measurement *meas;
        ////////////////////////////////////
        pthread_mutex_t lock_mean;
        pthread_mutex_t lock_variance;
        pthread_cond_t cond_mean;
        pthread_cond_t cond_variance;
        pthread_mutex_t lock_weight;
        pthread_cond_t cond_weight;
        int mean_thread_count;
        int variance_thread_count;
        int weight_thread_count;
        int numThreads;
        int distanceSum;
        int variance;
        int totalWeight;
        int totalLength;
    }

    struct thread_data_t {
        mutual_data_t *mutualData;
        particle *particles;
        int length;
    };

    static double GaussianDist(double x, double sigma, double my);
    static void observationModel(thread_data_t *data_p);
    static void normalizeWeights(thread_data_t *data_p);
    static void resample(std::vector<particle> *particles);

private:
    struct thread_data_t *threadData;
    pthread_t *threads;
    mutual_data_t mutualData;
};

void mclFilter(particle pose, particle command, measurement meas, vector<particle> &previous);

#endif
