#include "particle.hpp"
#include <cstdlib>
#include <cmath>

Particle::Particle(double x, double y, double theta, double weight) {
    _x = x;
    _y = y;
    _theta = theta;
    _weight = weight;
    overRunTheta();
}

//Particle::Particle (Particle &particle) {
    //_x = particle.x();
    //_y = particle.y();
    //_theta = particle.theta();
    //_weight = particle.weight();
//}

void Particle::rotate(double delta){
    Particle tmp = Particle::createUnit(this->angle() + delta);
    tmp.scale(this->length());
    this->_x = tmp._x;
    this->_y = tmp._y;
    //_x = _x * cos(delta) - _y * sin(delta);
    //_y = _x * sin(delta) + _y * cos(delta);
}

void Particle::move(double x, double y, double theta){
    _x += x;
    _y += y;
    _theta += theta;
    overRunTheta();
}

void Particle::addLength(double scalar) {
  double amount = 1 + (scalar / this->length());

  this->scale(amount);
}

void Particle::add(Particle other) {
  _x += other.x();
  _y += other.y();
}

void Particle::addTheta(Particle other) {
  _x += other.x();
  _y += other.y();
  _theta += other.theta();
  overRunTheta();
}

void Particle::sub(Particle other) {
  _x -= other.x();
  _y -= other.y();
}

void Particle::subTheta(Particle other) {
  _x -= other.x();
  _y -= other.y();
  _theta -= other.theta();
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

double Particle::angleBetween(Particle par)
{
    return atan2(par.y(), par.x()) - atan2(y(), x());
}

void Particle::addNoise(double sigma, double thetaKappa){
    this->move(
        randn(0, sigma),
        randn(0, sigma),
        randn(0, thetaKappa)
    );
}

double Particle::randf()
{
    const int r = rand();
    return ((double)r)/(double)RAND_MAX;
}

double Particle::randn(double m, double s){
    double x1, x2, w, y1;

    do
      {
        x1 = 2.0 * randf () - 1.0;
        x2 = 2.0 * randf () - 1.0;
        w = x1 * x1 + x2 * x2;
      }
    while (w >= 1.0);

    w = sqrt ((-2.0 * log(w)) / w);
    y1 = x1 * w;
    return (m + y1 * s);
}

vector<Particle> interpolatePath(vector<Particle> &path, int steps){
    vector<Particle> interpolated;

    // Turning each step into 0 steps yields the empty path
    if (steps == 0)
      return interpolated;

    double factor = 1.0 / (double)steps;

    for (int i = 0; i < path.size()-1; i++) {
        Particle current = path[i];
        Particle next    = path[i+1];

        for (int j = 0; j < steps; j++) {
            Particle diff = next;
            diff.sub(current);

            diff.scale((double)j * factor);

            Particle step;
            step.add(current);
            step.add(diff);

            interpolated.push_back(step);
        }
    }

    // Add the last step of the original path to the new path
    if (path.size() > 0)
      interpolated.push_back(path[path.size() - 1]);

    return interpolated;
}
