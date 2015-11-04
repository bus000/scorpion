#include "particleFilter.hpp"
#include <iostream>
#include <cstdlib>
#include <fstream>

ParticleFilter::ParticleFilter(std::vector<Particle> *particles,
        WorldMap *map, double fallbackDistVar, double fallbackAngleVar) {

    _particles = particles;
    _fallbackDistVar = fallbackDistVar;
    _fallbackAngleVar = fallbackAngleVar;
    _map = map;
}

void ParticleFilter::filter(Measurement measurement, Particle command){
    int resampleLimit = _particles->size();
    _angleWeights.assign(_particles->size(), 0.0);
    _distanceWeights.assign(_particles->size(), 0.0);

    //substitute x% procent of the particles with random ones

    dynamicModel(command);
    updateVariance(measurement);
    if(!measurement.invalid){
        resampleLimit *= 0.9999;
        int newParticlesLimit = _particles->size()-resampleLimit;
        _particles->erase(_particles->begin()+resampleLimit, _particles->end()); 
        addRandomParticles(newParticlesLimit);
        observationModel(measurement);
        mergeAndNormalizeWeights();
    }else{
        resetWeights();
    }

    resample(_particles->size());
    calcBelieve();
}

void ParticleFilter::calcBelieve(){
    double rX = 0, rY = 0, rTheta = 0;
    for(int i = 0; i < _particles->size(); i++){
        rX += _particles->at(i).x();
        rY += _particles->at(i).y();
        rTheta += _particles->at(i).theta();
    }
    rX /= (double)_particles->size();
    rY /= (double)_particles->size();
    rTheta /= (double)_particles->size();
    this->believe = Particle(rX,rY,rTheta);
}

void ParticleFilter::mergeAndNormalizeWeights(){
    assert(_distanceWeights.size() == _particles->size());
    assert(_angleWeights.size() == _particles->size());
    double distWeightSum = 0;
    double angleWeightSum = 0;
    double totalWeightSum = 0;
    for(int i = 0; i < _particles->size(); i++){
        _particles->at(i).weight(_distanceWeights[i]*_angleWeights[i]);
        distWeightSum += _distanceWeights[i];
        angleWeightSum += _angleWeights[i];
        totalWeightSum += _distanceWeights[i]*_angleWeights[i];
    }

    for(int i = 0; i < _particles->size(); i++){
        if(distWeightSum == 0.0){
            _particles->at(i).weight(_angleWeights[i]/angleWeightSum);
        }else if(angleWeightSum == 0.0){
            _particles->at(i).weight(_distanceWeights[i]/distWeightSum);
        }else{
            _particles->at(i).weight(_particles->at(i).weight()/totalWeightSum);
            continue;
        }

        _particles->at(i).addNoise(10.0, 0.1);

        if(_particles->at(i).weight() != _particles->at(i).weight()){
            std::cout << "FUCKING NaN\n";
            exit(0);
        }
    }
}

void ParticleFilter::updateVariance(Measurement measurement){
    //do something clever
}

void ParticleFilter::addRandomParticles(int limit){
    std::ifstream file("/dev/urandom", ios::in);
    if(!file.is_open()){
        std::cout << "error opening urandom" << std::endl;
        exit(0);
    }

    int random;
    for(int i = 0; i < limit; i++){
        if(i%10000)
            file.read((char*)&random, sizeof(int));
        srand(random);
        double width = _map->width();
        double height = _map->height();
        Particle tmp(
            Particle::randf()*width,
            Particle::randf()*height,
            (Particle::randf()*2.0*M_PI)-M_PI
        );
        _particles->push_back(tmp);
    }
    file.close();
}

void ParticleFilter::dynamicModel(Particle command){
    std::vector<Particle>::iterator it;

    command.theta(command.theta()*-1.0);
    for(it = _particles->begin(); it != _particles->end(); it++){
        it->addTheta(command);
        double sigma;
        if(command.x() == 0.0 && command.y() == 0.0)
           sigma = 3.5;
        else
           sigma = command.length()*0.3;
        double kappa;
        if(command.theta() == 0.0)
           kappa = 0.1;
        else
           kappa = (abs(command.theta())/M_PI)*0.5;
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
            0.13,
            hypoAngle
        );
        Particle pDiff = *it;
        pDiff.sub(measurement.position);

        double distance = pDiff.length();
        _distanceWeights[w_count] = GaussianDist(
            measurement.measurement.length(),
            //0.274, //sigma: something else here
            16.0,
            distance
        );
        w_count++;
    }
}

double ParticleFilter::GaussianDist(double x, double sigma, double my = 0){
    return (1.0/sqrt(2.0*M_PI*sigma))*exp(-1.0*(pow(my-x,2.0)/(2.0*sigma)));
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

        while(_particles->at(front).weight() == 0.0){
            front--;
        }

        //cout << "front: " << front << "=" << _particles->at(front).weight() << std::endl;
        newParticles.push_back(_particles->at(front));
    }


    *_particles = newParticles;
}

void ParticleFilter::resetWeights(){
    for(int i = 0; i < _particles->size(); i++)
        _particles->at(i).weight(1.0/(double)_particles->size());
}
