#include <vector>
#ifndef PARTICLE_H
#define PARTICLE_H

class Particle {
public:
    Particle(double x = 0, double y = 0, double theta = 0, double weight = 0);
    void rotate(double delta);
    void move(double x, double y, double theta);
    void addLength(double scalar);
    void add(Particle other);
    void sub(Particle other); 
    static Particle createUnit(double angle);
    void scale(double factor);
    void normalize();
    double length();
    double angle();
    double angleBetween(Particle *par);

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
