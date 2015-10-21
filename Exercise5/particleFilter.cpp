#include "particleFilter.hpp"
#include <cstdlib>
#include <cstring>

static double particleFilter::GaussianDist(double x, double sigma, double my = 0){
    return (1/sqrt(2*M_PI*sigma))*exp(-1*(pow(my-x,2)/(2*sigma)));
}

particleFilter::particleFilter(int numThreads){
    this->mutualData.numThreads = numThreads;
    this->threads = new pthread_t[numThreads];
    this->threadData = new thread_data_T[numThreads];
    pthread_mutex_init(&this->mutualData.lock_mean);
    pthread_mutex_init(&this->mutualData.lock_variance);
    pthread_mutex_init(&this->mutualData.lock_weight);
    pthread_cond_init(&this->mutualData.cond_mean);
    pthread_cond_init(&this->mutualData.cond_variance);
    pthread_cond_init(&this->mutualData.cond_weight);

    for(int i = 0; i < numThreads; i++)
        this->threadData[i].mutualData = &this->mutualData;
}

particleFilter::~particleFilter(){
    delete[] this->threads;
    delete[] this->threadData;
    pthread_mutex_destroy(&this->mutualData.lock_mean);
    pthread_mutex_destroy(&this->mutualData.lock_variance);
    pthread_mutex_destroy(&this->mutualData.lock_weight);
    pthread_cond_destroy(&this->mutualData.cond_mean);
    pthread_cond_destroy(&this->mutualData.cond_variance);
    pthread_cond_destroy(&this->mutualData.cond_lock);
}

void particleFilter::filter(particle position, particle command, measurement meas,
        vector<particle> *parts){

    this->mutualData.position = &position;
    this->mutualData.command = &command;
    this->mutualData.meas = &meas;
    this->mutualData.mean_thread_count =
        this->mutualData.variance_thread_count = 0;
    this->mutualData.distanceSum = 0;
    this->mutualData.variance = 0;
    this->mutualData.totalLength = parts.size();
    this->mutualData.totalWeight = 0;

    for(int i = 0; i < this->mutualData.numThreads; i++){
        this->threadData[i].length = parts.size()/this->mutualData.numThreads;
        if(i == this->mutualData.numThreads-1)
            this->threadData[i].length += parts.size()%this->mutualData.numThreads;
        this->threadData[i].particles =
            parts.data+(i*(parts.size()/this->mutualData.numThreads));

        pthread_create(&this->threads[i], NULL,
                particleFilter::threadStart, (void*)&this->threadData[i]);
    }

    for(int i = 0; i < this->mutualData.numThreads; i++){
        pthread_join(this->threads[i], NULL);
    }

    particleFilter::resample(parts, parts.size());
}

static void particleFilter::observationModel(thread_data_t *data_p){
    particleFilter::mutual_data_t *mutualData = data_p->mutualData;

    vector<double> distances;
    double ownDistanceSum = 0.0;
    double ownVariance = 0.0;
    double mean;
    double stdVariance;

    //calculate distance from particle p to observation
    for(int i = 0; i < data_p->length; i++){
        particle p = data_p->particles[i];
        double distance = sqrt(pow(p.x-mutualData->meas->position.x, 2)
                +pow(p.y-mutualData->meas->position.y, 2));
        distances.push(distance);
        ownSum += distance;
    }

    //Sum the distances sum for all threads
    pthread_mutex_lock(&mutualData->lock_mean);
    mutualData->distanceSum += ownSum;
    mutualData->mean_thread_count++;
    pthread_cond_broadcast(&mutualData->cond_mean);
    while(mutualData->mean_thread_count < mutualData->numThreads)
        pthread_cond_wait(&mutualData->cond_mean, &mutualData->lock_mean);
    pthread_mutex_unlock(&mutualData->lock_mean);

    //calculate distance mean
    mean = mutualData->distanceSum/mutualData->totalLength;

    //Calculate the variance for every particle
    for(int i = 0; i < data_p->length; i++)
        ownVariance += pow(mean - distances[i], 2);

    //Sum the distance variance for all threads
    pthread_mutex_lock(&mutualData->lock_variance);
    mutualData->variance += ownVariance;
    mutualData->variance_thread_count++;
    pthread_cond_broadcast(&mutualData->cond_variance);
    while(mutualData->variance_thread_count < mutualData->numThreads)
        pthread_cond_wait(&mutualData->cond_variance, &mutualData->lock_variance);
    pthread_mutex_unlock(&mutualData->lock_variance);

    stdVariance = sqrt(mutualData->variance);

    for(int i = 0; i < data_p->length; i++)
        data_p->particles[i].weight =
            particleFilter::GaussianDist(mutualData->meas->distance,
                stdVariance, distances[i]);
}

static void particleFilter::dynamicModel(particleFilter::thread_data_t *data_p){
    particle command = *data_p->mutualData->command;

    for(int i = 0; i < data_p->length; i++){
        move_particle(
            data_p->particles[i],
            command.x,
            command.y,
            command.theta
        );
        uncertain_particle(data_p->particles[i], 1.5, 0.0);
    }
}

static void normalizeWeights(thread_data_t *data_p){
    particleFilter::mutual_data_t *mutualData = data_p->mutualData;
    double ownTotalWeight = 0.0;

    for(int i = 0; i < data_p->length; i++)
        ownTotalWeight += data_p->particles[i].weight;
    
    pthread_mutex_lock(&mutualData->lock_weight);
    mutualData->totalWeight += ownTotalWeight;
    mutualData->weight_thread_count++;
    pthread_cond_broadcast(&mutualData->cond_weight);
    while(mutualData->weight_thread_count < mutualData->numThreads)
        pthread_cond_wait(&mutualData->cond_weight, &mutualData->lock_weight);
    pthread_mutex_unlock(&mutualData->lock_weight);

    for(int i = 0; i < data_p->length; i++)
        data_p->particles[i].weight /= mutualData->totalWeight;
}

static void particleFilter::resample(vector<particle *particles, int limit){
    vector<particle> newParticles;
    vector<double> weightGraf;

    weightGraf.push(0.0);

    for(int i = 0; i < particles.size(); i++)
        weightGraf.push_back(weightGraf.back() + particles[i].weight);

    for(int i = 0; i < limit; i++){
        double toss = randf();

        for(int a = 0; a < particles.size(); a++){
            double w1 = weightGraf[i];
            double w2 = weightGraf[i+1];

            if(toss > w1 && toss < w2)
                newParticles.push_back(particles[i]);
        }
    }

    *particles = newParticles;
}

static void* particleFilter::threadStart(void* data){
    particleFilter::thread_data_t *data_p = (particleFilter::thread_data_t*)data;
    particleFilter::observationModel(data_p);
    particleFilter::dynamicModel(data_p);
    particleFilter::normalizeWeight(data_p);

    pthread_exit(NULL);
}
