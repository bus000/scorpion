#include <vector>
#ifndef PARTICLE_H
#define PARTICLE_H

class Particle {
public:
    Particle(double x, double y, double theta, double weight);
    void rotate(double delta);
    void move(double x, double y, double theta);
    static Particle createUnit(double angle);
    void scale(double factor);
    void normalize();
    double length();
    double angle();

    //Setters
    void x(double newX);
    void y(double newY);
    void theta(double newTheta);
    void weight(double newWeight);

    //Getters
    double x();
    double y();
    double theta();
    double weight();
private:
    double _x, _y, _theta, _weight;
    void overRunTheta();
};

#endif
