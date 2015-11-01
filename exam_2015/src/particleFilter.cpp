#include "particleFilter.hpp"
#include <iostream>

ParticleFilter::ParticleFilter(std::vector<Particle> *particles,
        WorldMap *map, double fallbackDistVar, double fallbackAngleVar) {

    _particles = particles;
    _fallbackDistVar = fallbackDistVar;
    _fallbackAngleVar = fallbackAngleVar;
    _map = map;
}

void ParticleFilter::filter(Measurement measurement, Particle command){
    int resampleLimit = _particles->size()*0.99;
    int newParticlesLimit = _particles->size()-resampleLimit;
    _angleWeights.assign(_particles->size(), 0.0);
    _distanceWeights.assign(_particles->size(), 0.0);

    //substitute x% procent of the particles with random ones
    _particles->erase(_particles->begin()+resampleLimit, _particles->end()); 
    addRandomParticles(newParticlesLimit);

    dynamicModel(command);
    updateVariance(measurement);
    if(!measurement.invalid){
        observationModel(measurement);
        mergeAndNormalizeWeights();
    }else{
        resetWeights();
    }


    resample(_particles->size());
}

void ParticleFilter::mergeAndNormalizeWeights(){
    assert(_distanceWeights.size() == _particles->size());
    assert(_angleWeights.size() == _particles->size());
    double weightSum = 0;
    for(int i = 0; i < _particles->size(); i++){
        _particles->at(i).weight(_distanceWeights[i]*_angleWeights[i]);
        weightSum += _particles->at(i).weight();
    }

    for(int i = 0; i < _particles->size(); i++){
        _particles->at(i).weight(_particles->at(i).weight()/weightSum);
    }
}

void ParticleFilter::updateVariance(Measurement measurement){
    //do something clever
}

void ParticleFilter::addRandomParticles(int limit){
    for(int i = 0; i < limit; i++){
        double width = _map->width();
        double height = _map->height();
        Particle tmp(
            Particle::randf()*width,
            Particle::randf()*height,
            Particle::randf()*M_PI
        );
        _particles->push_back(tmp);
    }
}

void ParticleFilter::dynamicModel(Particle command){
    std::vector<Particle>::iterator it;
    if(command.x() == 0.0 &&
            command.y() == 0.0 &&
            command.angle() == 0.0){

        for(it = _particles->begin(); it != _particles->end(); it++)
            it->addNoise(0.3,0.2);
        std::cout << "we are here\n\n";
        return;
    }

    for(it = _particles->begin(); it != _particles->end(); it++){
        it->add(command);
        double sigma = command.length()*0.15;
        double kappa = (abs(command.angle())/M_PI)*0.1;
        it->addNoise(sigma, kappa);
    }
}

void ParticleFilter::observationModel(Measurement measurement){
    std::vector<Particle>::iterator it;

    int w_count = 0;
    for(it = _particles->begin(); it != _particles->end(); it++){
        Particle hypoUnit = Particle::createUnit(it->theta());
        Particle diffVector = measurement.position;
        diffVector.sub(*it);
        double hypoAngle = hypoUnit.angleBetween(diffVector);

        _angleWeights[w_count] = GaussianDist(
            measurement.measurement.angle(),
            0.3, //sigma: something else here
            hypoAngle
        );
        Particle pDiff = *it;
        pDiff.sub(measurement.position);

        double distance = pDiff.length();
        _distanceWeights[w_count] = GaussianDist(
            measurement.measurement.length(),
            1.5, //sigma: something else here
            distance
        );
        w_count++;
    }
}

double ParticleFilter::GaussianDist(double x, double sigma, double my = 0){
    return (1.0/sqrt(2.0*M_PI*sigma))*exp(-1.0*(pow(my-x,2)/(2.0*sigma)));
}

void ParticleFilter::resample(int limit){
    vector<Particle> newParticles;
    vector<double> weightGraf;

    weightGraf.reserve(_particles->size()+1);
    weightGraf.push_back(0.0);

    for(int i = 0; i < _particles->size(); i++){
        weightGraf.push_back(weightGraf.back() + _particles->at(i).weight());
    }

    for(int i = 0; i < limit; i++){
        int front = 0;
        int back = weightGraf.size()-1;
        double toss = Particle::randf();

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

        newParticles.push_back(_particles->at(front));
    }

    *_particles = newParticles;
}

void ParticleFilter::resetWeights(){
    for(int i = 0; i < _particles->size(); i++)
        _particles->at(i).weight(1.0/(double)_particles->size());
}
