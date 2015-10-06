#ifndef _PARTICLE_FILTER_
#define _PARTICLE_FILTER_

#include "random_numbers.h"

class measurement {
  particle position;
  double distance;
  double angle;

  measurement() {};
  measurement(particle position, double distance, double angle) {
    this->position = position;
    this->distance = distance;
    this->angle    = angle;
  }

  ~measurement() {};
};

vector<particle> mclFilter(particle pose, particle command, vector<particle> previous) {
  vector<particle> tmpX;
  vector<particle> finX; 

  for (int i = 0; i < previous.size(); i++) {
    particle p;
    p->
  }
}



#endif
