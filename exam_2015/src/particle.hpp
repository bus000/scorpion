#ifndef PARTICLE_H
#define PARTICLE_H

#include <vector>

using namespace std;

class Particle {
public:
    Particle(double x = 0, double y = 0, double theta = 0, double weight = 0);

    //Particle(Particle &particle);

    void rotate(double delta);
    void move(double x, double y, double theta);
    void addLength(double scalar);
    void add(Particle other);
    void addTheta(Particle other);

    void sub(Particle other);
    void subTheta(Particle other);


    static Particle createUnit(double angle);
    void scale(double factor);
    void normalize();
    double length();
    double angle();
    double angleBetween(Particle par);
    void addNoise(double sigma, double thetaKappa);
    static double randf();
    static double randn(double m, double s);

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

vector<Particle> interpolatePath(vector<Particle> &path, int steps);

#endif
