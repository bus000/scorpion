#include "particleFilter.hpp"
#include <cstdlib>
#include <cstring>
#include <iostream>

double particleFilter::GaussianDist(double x, double sigma, double my = 0){
    return (1/sqrt(2*M_PI*sigma))*exp(-1*(pow(my-x,2)/(2*sigma)));
}

particleFilter::particleFilter(int numThreads){
    this->mutualData.numThreads = numThreads;
    this->threads = new pthread_t[numThreads];
    this->threadData = new thread_data_t[numThreads];
    pthread_mutex_init(&this->mutualData.lock_mean, NULL);
    pthread_mutex_init(&this->mutualData.lock_variance, NULL);
    pthread_mutex_init(&this->mutualData.lock_weight, NULL);
    pthread_mutex_init(&this->mutualData.data_ready_lock, NULL);
    pthread_mutex_init(&this->mutualData.calculation_lock, NULL);
    pthread_mutex_init(&this->mutualData.thread_terminate_lock, NULL);
    pthread_cond_init(&this->mutualData.cond_mean, NULL);
    pthread_cond_init(&this->mutualData.cond_variance, NULL);
    pthread_cond_init(&this->mutualData.cond_weight, NULL);
    pthread_cond_init(&this->mutualData.data_ready_cond, NULL);
    pthread_cond_init(&this->mutualData.calculation_cond, NULL);

    for(int i = 0; i < numThreads; i++)
        this->threadData[i].mutualData = &this->mutualData;

    //set thread termination lock
    pthread_mutex_lock(&this->mutualData.thread_terminate_lock);

    //spin up threads
    for(int i = 0; i < numThreads; i++)
        pthread_create(&this->threads[i], NULL,
                threadStart, (void*)&this->threadData[i]);
}

particleFilter::~particleFilter(){
    pthread_mutex_unlock(&this->mutualData.thread_terminate_lock);
    for(int i = 0; i < this->mutualData.numThreads; i++)
        pthread_join(this->threads[i], NULL);

    delete[] this->threads;
    delete[] this->threadData;
    pthread_mutex_destroy(&this->mutualData.lock_mean);
    pthread_mutex_destroy(&this->mutualData.lock_variance);
    pthread_mutex_destroy(&this->mutualData.lock_weight);
    pthread_mutex_destroy(&this->mutualData.data_ready_lock);
    pthread_mutex_destroy(&this->mutualData.calculation_lock);
    pthread_mutex_destroy(&this->mutualData.thread_terminate_lock);
    pthread_cond_destroy(&this->mutualData.cond_mean);
    pthread_cond_destroy(&this->mutualData.cond_variance);
    pthread_cond_destroy(&this->mutualData.cond_weight);
    pthread_cond_destroy(&this->mutualData.calculation_cond);
    pthread_cond_destroy(&this->mutualData.data_ready_cond);
}

void particleFilter::filter(particle command, measurement meas,
        vector<particle> *parts){
    particle est_pose;

    this->mutualData.command = &command;
    this->mutualData.meas = &meas;
    this->mutualData.mean_thread_count =
        this->mutualData.variance_thread_count =
        this->mutualData.weight_thread_count = 0;
    this->mutualData.distanceSum = 0;
    this->mutualData.variance = 0;
    this->mutualData.totalLength = parts->size();
    this->mutualData.totalWeight = 0;
    this->mutualData.angleSum = 0.0;
    this->mutualData.a_variance = 0.0;
    this->mutualData.a_mean_thread_count = 0;
    this->mutualData.a_variance_thread_count = 0;

    //initialize data to be calculated
    for(int i = 0; i < this->mutualData.numThreads; i++){
        this->threadData[i].length = parts->size()/this->mutualData.numThreads;
        if(i == this->mutualData.numThreads-1)
            this->threadData[i].length += parts->size()%this->mutualData.numThreads;

        this->threadData[i].particles =
            parts->data()+(i*(parts->size()/this->mutualData.numThreads));
    }

    //wake working threads
    this->mutualData.calculation_counter = 0;
    pthread_cond_broadcast(&this->mutualData.data_ready_cond);

    //wait for threads to finish
    pthread_mutex_lock(&this->mutualData.calculation_lock);
    while(this->mutualData.calculation_counter < this->mutualData.numThreads)
        pthread_cond_wait(&this->mutualData.calculation_cond,
                &this->mutualData.calculation_lock);
    pthread_mutex_unlock(&this->mutualData.calculation_lock);

    particleFilter::resample(parts, parts->size());
}

void particleFilter::angleObservationModel(thread_data_t * data_p){
    particleFilter::mutual_data_t *mutualData = data_p->mutualData;

    vector<double> angles;
    double ownAngleSum = 0.0;
    double ownVariance = 0.0;
    double mean;
    double stdVariance;

    //calculate angle between particle p and the obervation
    for(int i = 0; i < data_p->length; i++){
        /*double L[2] = {
            mutualData->meas->position.x - data_p->particles[i].x,
            mutualData->meas->position.y - data_p->particles[i].y
        }; 
        double Llength = sqrt(pow(L[0],2)+pow(L[1],2));
        L[0] /= Llength;
        L[1] /= Llength;

        double lAngle = atan2(L[1], L[0]);
        double diffAngle = data_p->particles[i].theta - lAngle;
        if(lAngle >= M_PI)
            diffAngle -= 2.0*M_PI;
        angles.push_back(diffAngle);
        ownAngleSum += diffAngle;*/

        double thetaNorm[2] = {
            cos(data_p->particles[i].theta),
            -1*sin(data_p->particles[i].theta)
        };
        double L[2] = {
            mutualData->meas->position.x - data_p->particles[i].x,
            mutualData->meas->position.y - data_p->particles[i].y
        };
        double pAngle = atan2(thetaNorm[1], thetaNorm[0]) - atan2(L[1], L[0]);
        if(pAngle >= M_PI)
            pAngle -= 2.0*M_PI;

        angles.push_back(pAngle);
        ownAngleSum += pAngle;
    }

    //Sum the angles for all threads
    pthread_mutex_lock(&mutualData->lock_mean);
    mutualData->angleSum += ownAngleSum;
    mutualData->a_mean_thread_count++;
    pthread_cond_broadcast(&mutualData->cond_mean);
    while(mutualData->a_mean_thread_count < mutualData->numThreads)
        pthread_cond_wait(&mutualData->cond_mean, &mutualData->lock_mean);
    pthread_mutex_unlock(&mutualData->lock_mean);

    //Calculate angle mean
    mean = mutualData->angleSum/mutualData->totalLength;

    //Calculate the angle variance for every particle
    for(int i = 0; i < data_p->length; i++){
        ownVariance += pow(mean - angles[i], 2);
    }

    //Sum the angle variance for all threads
    pthread_mutex_lock(&mutualData->lock_variance);
    mutualData->a_variance += ownVariance;
    mutualData->a_variance_thread_count++;
    pthread_cond_broadcast(&mutualData->cond_variance);
    while(mutualData->a_variance_thread_count < mutualData->numThreads)
        pthread_cond_wait(&mutualData->cond_variance, &mutualData->lock_variance);
    pthread_mutex_unlock(&mutualData->lock_variance);

    stdVariance = sqrt(mutualData->a_variance);

    //update particle weight
    static bool t = true;
    for(int i = 0; i < data_p->length; i++){
        data_p->particles[i].weight *=
            particleFilter::GaussianDist(mutualData->meas->angle,
                    stdVariance, angles[i]);

        if(t){
            cout << "weight: " << data_p->particles[i].weight;
            cout << endl << "angle: " << angles[i] << endl << endl;
        }
        //cout << 
        //    particleFilter::GaussianDist(mutualData->meas->angle,
        //            stdVariance, angles[i]) << endl;
    }
    t = false;
}

void particleFilter::distObservationModel(thread_data_t *data_p){
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
        distances.push_back(distance);
        ownDistanceSum += distance;
    }

    //Sum the distances sum for all threads
    pthread_mutex_lock(&mutualData->lock_mean);
    mutualData->distanceSum += ownDistanceSum;
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

void particleFilter::dynamicModel(particleFilter::thread_data_t *data_p){
    particle command = *data_p->mutualData->command;

    for(int i = 0; i < data_p->length; i++){
        move_particle(
            data_p->particles[i],
            command.x,
            command.y,
            command.theta
        );
        uncertain_particle(data_p->particles[i], 1.5, 0.1);
    }
}

void particleFilter::normalizeWeights(thread_data_t *data_p){
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

void particleFilter::resample(vector<particle> *particles, int limit){
    vector<particle> newParticles;
    vector<double> weightGraf;

    weightGraf.reserve(particles->size()+1);
    weightGraf.push_back(0.0);

    for(int i = 0; i < particles->size(); i++)
        weightGraf.push_back(weightGraf.back() + particles->at(i).weight);


    for(int i = 0; i < limit; i++){
        int front = 0;
        int back = weightGraf.size()-1;
        double toss = randf();

        while(front+1 != back){
            int middle = front + (back-front)/2;
            if(weightGraf[middle] == toss){
                //will probably never happen
                front = middle;
                break;
            }
            if(weightGraf[middle] > toss)
                back = middle;
            else
                front = middle;
        }

        newParticles.push_back(particles->at(front));
    }

    *particles = newParticles;
}

void particleFilter::resetWeight(particleFilter::thread_data_t *data_p){
    for(int i = 0; i < data_p->length; i++)
        data_p->particles[i].weight = 1.0/data_p->mutualData->totalLength;
}

void* particleFilter::threadStart(void* data){
    particleFilter::thread_data_t *data_p = (particleFilter::thread_data_t*)data;

    while(true){
        //check for termination
        if(!pthread_mutex_trylock(&data_p->mutualData->thread_terminate_lock)){
            pthread_mutex_unlock(&data_p->mutualData->thread_terminate_lock);
            break;
        }

        //wait for data ready signal
        pthread_mutex_lock(&data_p->mutualData->data_ready_lock);
        pthread_cond_wait(&data_p->mutualData->data_ready_cond,
                &data_p->mutualData->data_ready_lock);
        pthread_mutex_unlock(&data_p->mutualData->data_ready_lock);
        
        //And we have data, let the action begin!!
        if(data_p->mutualData->meas->landmark != NoLandmark){
            particleFilter::distObservationModel(data_p);
            particleFilter::angleObservationModel(data_p);
        }else{
            particleFilter::resetWeight(data_p);
        }
        particleFilter::dynamicModel(data_p);
        particleFilter::normalizeWeights(data_p);

        //Signal that we are done with the calculation
        pthread_mutex_lock(&data_p->mutualData->calculation_lock);
        data_p->mutualData->calculation_counter++;
        pthread_cond_signal(&data_p->mutualData->calculation_cond);
        pthread_mutex_unlock(&data_p->mutualData->calculation_lock);
    }

    pthread_exit(NULL);
}

