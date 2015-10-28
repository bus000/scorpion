#include "Particle.hpp"
#include <cmath>

Particle::Particle(double x = 0, double y = 0, double theta = 0, double weight = 0) {
    _x = x;
    _y = y;
    _theta = theta;
    _weight = weight;
    overRunTheta();
}

void Particle::rotate(double delta){
    _x = _x * cos(delta) - _y * sin(delta);
    _y = _x * sin(delta) + _y * cos(delta);
}

void Particle::move(double x, double y, double theta){
    _x += x;
    _y += y;
    _theta += theta;
    overRunTheta();
}

Particle Particle::createUnit(double angle){
    Particle tmp(cos(angle), sin(angle));
    return tmp;
}

void Particle::scale(double factor){
    _x *= factor;
    _y *= factor;
}

void Particle::normalize(){
    double length = this->length();
    _x *= length;
    _y *= length;
}

double Particle::length(){
    return sqrt(pow(_x,2)+pow(_y,2));
}

void Particle::overRunTheta(){
    while(_theta < -1.0*M_PI || _theta >M_PI){
        if(_theta < -1.0*M_PI)
            _theta += 2.0*M_PI;
        else
            _theta -= 2.0*M_PI;
    }
}

void Particle::x(double newX){
    _x = newX;
}

void Particle::y(double newY){
    _y = newY;
}

void Particle::theta(double newTheta){
    _theta = newTheta;
    overRunTheta();
}

void Particle::weight(double newWeight){
    _weight = newWeight;
}

double Particle::x(){
    return _x;
}

double Particle::y(){
    return _y;
}

double Particle::theta(){
    return _theta;
}

double Particle::weight(){
    return _weight;
}

double Particle::angle(){
    return atan2(_y, _x);
}
