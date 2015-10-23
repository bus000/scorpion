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
#include "measurement.hpp"

using namespace std;
using namespace cv;

class particleFilter {
public:
    particleFilter(int numThreads);
    ~particleFilter();
    void filter(particle command, measurement meas,
            vector<particle> *parts);
    static void* threadStart(void *data);

    struct mutual_data_t {
        //Only alive while filter is running
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
        int a_mean_thread_count;
        int a_variance_thread_count;
        int numThreads;
        double distanceSum;
        double variance;
        double totalWeight;
        double angleSum;
        double a_variance;
        int totalLength;
    };

    struct thread_data_t {
        mutual_data_t *mutualData;
        particle *particles;
        int length;
    };

    static double GaussianDist(double x, double sigma, double my);
    static void distObservationModel(thread_data_t *data_p);
    static void angleObservationModel(thread_data_t * data_p);
    static void dynamicModel(particleFilter::thread_data_t *data_p);
    static void normalizeWeights(thread_data_t *data_p);
    static void resample(std::vector<particle> *particles, int limit);
    static void resetWeight(particleFilter::thread_data_t *data_p);
    mutual_data_t mutualData;

private:
    struct thread_data_t *threadData;
    pthread_t *threads;
};

#endif
