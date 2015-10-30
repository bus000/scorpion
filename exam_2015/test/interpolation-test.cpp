#include <stdio.h>
#include <vector>
#include "particle.hpp"

using namespace std;

int main() {
  vector<Particle> path;

  path.push_back(Particle(0, 0));
  path.push_back(Particle(100, 0));
  path.push_back(Particle(200, 0));
  path.push_back(Particle(300, 0));

  vector<Particle> interpolated = interpolatePath(path, 3);

  for (int i = 0; i < interpolated.size(); i++) {
    Particle node = interpolated[i];

    printf("Node %d.\t (%f, %f)\n", i, node.x(), node.y());
  }

  return 0;
}
