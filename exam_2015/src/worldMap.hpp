#ifndef WORLD_MAP_H
#define WORLD_MAP_H
#include <vector>
#include <cmath>
#include <algorithm>
#include <cstring>
#include <cassert>
#include "particle.hpp"

#define STRAIGHT_COST 10
#define DIAGONAL_COST 14

using namespace std;

class PathNode {
  private:
    int _x;
    int _y;
    PathNode *_parent;
    int _g;
    int _h;

    void calculateG();
    int manhattan(int goalX, int goalY);

  public:

    PathNode(int x, int y, int goalX, int goalY);

    PathNode(int x, int y, int goalX, int goalY, PathNode *parent);

    PathNode *parent() { return this->_parent; };

    void giveParent(PathNode *_parent);

    int x() { return this->_x; };
    int y() { return this->_y; };
    int g() { return this->_g; };
    int f();
};

class WorldMap {
public:
    WorldMap(double width, double height, int numSqWidth, int numSqHeight);
    ~WorldMap();
    void field(int col, int row, bool mark);
    bool& field(int col, int row);
    //[col][row]
    bool* operator[] (int col);
    int getRowFromY(double y);
    int getColFromX(double x);
    bool& fieldAt(double x, double y);
    void clear();

    int squareWidth();
    int squareHeight();
    int numSquareWidth();
    int numSquareHeight();
    double width();
    double height();

    vector<Particle> findPath( Particle &start
                             , Particle &goal
                             ); 

private:
    bool *map;

    double sqWidth;
    double sqHeight;

    double _width;
    double _height;
    int _numSqWidth;
    int _numSqHeight;

    vector<PathNode*> getWalkable(int x, int y, int goalX, int goalY);
};

#endif
