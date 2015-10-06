#include "particleFilter.hpp"

// Does one iteration of the Bootstap/Monte Carlo Localiation algorithm.
vector<particle> mclFilter(particle pose, particle command, measurement meas, vector<particle> previous) {
  vector<particle> tmpX;
  vector<particle> finX; 

  // Move all particles according to command and calculate weight according to measurement.
  for (int i = 0; i < previous.size(); i++) {
    particle p = dynamicModel(previous[i], command);
    p.weight   = measurementModel(p, meas);
    tmpX.push_back(p);
  }

  // Calculate total weight.
  double totalWeight;
  for (int i = 0; i < tmpX.size(); i++) {
    totalWeight += tmpX[i].weight;
  }

  // Normalize weights.
  for (int i = 0; i < tmpX.size(); i++) {
    particle *p = &(tmpX[i]);
    p->weight = p->weight / totalWeight;
  }

  // Resample particles.
  for (int i = 0; i < tmpX.size(); i++) {
    particle p = drawParticle(tmpX);
    finX.push_back(p);
  }

  return finX;
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
particle dynamicModel(particle target, particle command) {
  particle p; 
  move_particle(p, target.x, target.y, 0);
  move_particle(p, command.x, command.y, 0);
  return p;
}

// Calculate weight according to measurement.
double measurementModel(particle target, measurement meas) {
  Point p(target.x, target.y);
  Point m(meas.position.x, meas.position.y);

  double dist = norm(p - m);

  return 1.0 / abs(dist - meas.distance);
} 
