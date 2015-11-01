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
#define BESIDE_COST   50
#define PROB_ADJUST   10.0

using namespace std;

class WorldMap;

class PathNode {
public:

    PathNode( int x
            , int y
            , int goalX
            , int goalY
            , WorldMap *map);

    PathNode( int x
            , int y
            , int goalX
            , int goalY
            , PathNode *parent
            , WorldMap *map);

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
    int heuristic(int goalX, int goalY, WorldMap *map);
};

class WorldMap {
public:
    WorldMap(int numSqWidth, int numSqHeight, int sqSize);
    ~WorldMap();
    void field(int col, int row, double prob);
    double& field(int col, int row);

    void markFrom(Particle robot, Particle obstacle, double prob);
    //[col][row]
    double* operator[] (int col);
    int getRowFromY(double y);
    int getColFromX(double x);
    double& fieldAt(double x, double y);
    void clear();
    void print();
    void print(vector<Particle> &path, Particle curPos);

    int squareSize();
    int numSquareWidth();
    int numSquareHeight();
    double width();
    double height();

    double besideObstacle(int col, int row);

    vector<Particle> findPath( Particle &start , Particle &goal);

    /* Decrease the probability of there being an object by 10% in all
     * fields. */
    void decreaseProb(double percent = 10.0);


private:
    double *map;

    int _sqSize;
    int _width;
    int _height;
    int _numSqWidth;
    int _numSqHeight;

    vector<PathNode*> getWalkable(int x, int y, int goalX, int goalY);
};

#endif
