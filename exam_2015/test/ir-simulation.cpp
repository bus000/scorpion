#include <iostream>
#include <vector>
#include <stdio.h>
#include "worldMap.hpp"
#include "particle.hpp"

int main() {
  // Create our map
  WorldMap map(500, 500, 30, 30);

  Particle robot(map.height() / 4, map.width() / 2); 

  // The goal is 250 cm in forwards
  Particle goal(250, 100);
  goal.add(robot);

  vector<Particle> irvec;

  irvec.push_back(Particle(1,   0));
  irvec.push_back(Particle(1,   1));
  irvec.push_back(Particle(0,   1));
  irvec.push_back(Particle(-1,  1));
  irvec.push_back(Particle(-1,  0));
  irvec.push_back(Particle(-1, -1));
  irvec.push_back(Particle(0,  -1));
  irvec.push_back(Particle(1,  -1));

  vector<Particle> emptyPath;

  for (int i = 0; i < irvec.size(); i++) {
      cout << "\033[2J\033[1;1H";
      cout << "CHECKING MARKING OF IR PARTICLES" << endl;
      cout << "Tile width is " << map.squareWidth() << endl;
      Particle ir = irvec[i];
      ir.scale(map.squareWidth());
      ir.add(robot);
      map.markFrom(robot, ir);
      map.print(emptyPath, robot);
      getchar();
  }

  map.clear();

  vector<Particle> path = map.findPath(robot, goal);

  int step = 0;

  while (path.size() > 0) {
    cout << "\033[2J\033[1;1H";
    cout << "CHECKING PATHFINDING" << endl;
    if (step == 2) {
      cout << "Marking front" << endl;
      Particle ir = irvec[0];
      ir.scale(map.squareWidth()/1.5);
      ir.add(robot);
      map.markFrom(robot, ir);
    }

    if (step == 4) {
      cout << "Marking right" << endl;
      Particle ir = irvec[2];
      ir.scale(map.squareWidth()/1.5);
      ir.add(robot);
      map.markFrom(robot, ir);
    }

    if (step == 5) {
      cout << "Marking right" << endl;
      Particle ir = irvec[2];
      ir.scale(map.squareWidth()/1.5);
      ir.add(robot);
      map.markFrom(robot, ir);
    }

    if (step == 7) {
      cout << "Marking front-left" << endl;
      Particle ir = irvec[7];
      ir.scale(map.squareWidth()/1.5);
      ir.add(robot);
      map.markFrom(robot, ir);
    }

    if (step == 9) {
      cout << "Marking front-right" << endl;
      Particle ir = irvec[1];
      ir.scale(map.squareWidth()/1.5);
      ir.add(robot);
      map.markFrom(robot, ir);
    }

    if (step == 11) {
      cout << "Marking front" << endl;
      Particle ir = irvec[0];
      ir.scale(map.squareWidth()/1.5);
      ir.add(robot);
      map.markFrom(robot, ir);
    }


    path = map.findPath(robot, goal);
    map.print(path, robot);
    robot = path[0];
    getchar();

    step++;
  }

  return 0;
}
