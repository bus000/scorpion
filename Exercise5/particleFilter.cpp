#include "particleFilter.hpp"

static double GaussianDist(double x, double sigma, double my = 1){
    return (1/sqrt(2*M_PI*sigma))*exp(-1*(pow(my-x,2)/(2*sigma)));
}

// Assume that weights are normalized.
particle drawParticle(vector<particle> particles) {
    vector<double> adjusted;

    adjusted.push_back(0.0);

    // Scan particle weights.
    for (int i = 0; i < particles.size(); i++) {
        double w = adjusted.back();
        adjusted.push_back(w + particles[i].weight);
    }

    // Get a random number.
    double toss = randf();

    // Pick a particle based on the toss.
    for (int i = 0; i < adjusted.size(); i++) {
        double w1 = adjusted[i];
        double w2 = adjusted[i+1];

        if (toss > w1 && toss < w2) {
            return particles[i];
        }
    }

    return particles.back();
}

// Move particle according to robot command.
//particle dynamicModel(particle target, particle command) {
//    particle p(0, 0, 0, 0); 
//    move_particle(p, target.x, target.y, 0);
//    move_particle(p, command.x, command.y, 0);
//    uncertain_particle(p, 1.5, 0.0);
//    return p;
//}

void dynamicModel(vector<particle> &parts, particle command){
    for(int i = 0; i < parts.size(); i++){
        move_particle(
            parts[i],
            command.x,
            command.y,
            command.theta
        );
        uncertain_particle(parts[i], 1.5, 0.0);
    }
}

// Calculate weight according to measurement.
//double measurementModel(particle pos, particle target, measurement meas) {
//    Point p(target.x, target.y);
//    Point m(meas.position.x, meas.position.y);
//
//    double dist = norm(p - m);
//    double angf = fabs((target.theta + 2 * M_PI) - (meas.angle + 2 * M_PI)) / M_PI;
//    double angd = angf * dist;
//
//    return GaussianDist(meas.distance);
//
//    return GaussianDist(dist, meas.distance)
//        * GaussianDist(meas.angle);
//}

// Calculate weight according to measurements
void observationModel(vector<particle> &parts, measurement meas){
    //something simlar have to be done with the angle(s)
    double mean = 0.0;
    vector<double> distances;
    double stdVariance = 0.0;

    for(int i = 0; i < parts.size(); i++){
        double distance = sqrt(pow(parts[i].x-meas.position.x, 2)
                +pow(parts[i].y-meas.position.y, 2));
        distances.push_back(distance);
        mean += distance;
    }
    mean /= parts.size();
    
    for(int i = 0; i < parts.size(); i++){
        stdVariance += pow(mean - distances[i], 2);
    }
    stdVariance = sqrt(stdVariance);

    for(int i = 0; i < parts.size(); i++){
        parts[i].weight
            = GaussianDist(meas.distance, stdVariance, distances[i]);
    }
}

// Does one iteration of the Bootstap/Monte Carlo Localiation algorithm.
void mclFilter(particle command, measurement meas, vector<particle> &previous) {
    vector<particle> newParts;

    // Move all particles according to command and calculate weight according to measurement.
    dynamicModel(previous, command);
    observationModel(previous, meas);

    // Calculate total weight.
    double totalWeight;
    for (int i = 0; i < previous.size(); i++) {
        totalWeight += previous[i].weight;
    }

    // Normalize weights.
    for (int i = 0; i < previous.size(); i++) {
        previous[i].weight /= totalWeight;
    }

    // Resample particles.
    for (int i = 0; i < previous.size(); i++) {
        particle p = drawParticle(previous);
        newParts.push_back(p);
    }

    add_uncertainty(newParts, 4.0, 0.0);
    previous = newParts;
}

