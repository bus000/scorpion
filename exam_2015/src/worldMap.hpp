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
public:

    PathNode(int x, int y, int goalX, int goalY);

    PathNode(int x, int y, int goalX, int goalY, PathNode *parent);

    PathNode *parent() { return this->_parent; };

    void giveParent(PathNode *_parent);

    int x() { return this->_x; };
    int y() { return this->_y; };
    int g() { return this->_g; };
    int f();

private:
    int _x;
    int _y;
    PathNode *_parent;
    int _g;
    int _h;

    void calculateG();
    int manhattan(int goalX, int goalY);
};

class WorldMap {
public:
    WorldMap(int numSqWidth, int numSqHeight, int sqSize);
    ~WorldMap();
    void field(int col, int row, bool mark);
    bool& field(int col, int row);

    void markFrom(Particle robot, Particle obstacle);
    //[col][row]
    bool* operator[] (int col);
    int getRowFromY(double y);
    int getColFromX(double x);
    bool& fieldAt(double x, double y);
    void clear();
    void print();
    void print(vector<Particle> &path, Particle curPos);

    int squareSize();
    int numSquareWidth();
    int numSquareHeight();
    double width();
    double height();

    vector<Particle> findPath( Particle &start
            , Particle &goal
            );

private:
    bool *map;

    int _sqSize;
    int _width;
    int _height;
    int _numSqWidth;
    int _numSqHeight;

    vector<PathNode*> getWalkable(int x, int y, int goalX, int goalY);
};

#endif
