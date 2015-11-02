#include "../src/worldMap.hpp"
#include "../src/particle.hpp"
#include <iostream>
#include <stdio.h>
#include <unistd.h>

int main() {
  WorldMap map(7, 7, 10, vector<Particle>());

  map.field(3, 2, true);
  map.field(2, 2, true);
  map.field(3, 3, true);
  map.field(3, 4, true);
  map.field(3, 5, true);
  map.field(3, 6, true);


  Particle start(15, 35);
  Particle goal(55, 35);

  vector<Particle> path = map.findPath(start, goal);

  while (path.size() > 0) {
    cout << "\033[2J\033[1;1H";
    path = map.findPath(start, goal);
    map.print(path, start);
    start = path[0];
    getchar();
  }

  cout << "done at (" << start.x() << ", " << start.y() << ")" << endl;

  return 0;
}
