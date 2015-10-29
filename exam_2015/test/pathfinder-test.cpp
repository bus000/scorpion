#include "worldMap.hpp"
#include "particle.hpp"
#include <iostream>
#include <stdio.h>
#include <unistd.h>

int main() {
  WorldMap map(70, 70, 7, 7);

  map.field(3, 2, true);
  map.field(3, 3, true);
  map.field(3, 4, true);
  map.field(3, 5, true);
  map.field(3, 6, true);
  
  
  Particle start(15, 35);
  Particle goal(55, 35);


  vector<Particle> path = map.findPath(start, goal);

  cout << "\033[2J\033[1;1H";
  path = map.findPath(start, goal);
  map.print(path, start);
  start = path[0];
  getchar();

  cout << "\033[2J\033[1;1H";
  path = map.findPath(start, goal);
  map.print(path, start);
  start = path[0];
  getchar();

  map.field(3, 1, true);

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
