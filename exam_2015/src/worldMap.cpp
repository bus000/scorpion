#include "worldMap.hpp"
#include <cstring>
#include <cassert>
#include <stdio.h>
#include <iostream>
#include <float.h>

WorldMap::WorldMap(int numSqWidth, int numSqHeight, int sqSize,
        vector<Particle> landmarks) {
    _width = numSqWidth*sqSize;
    _height = numSqHeight*sqSize;
    _numSqWidth = numSqWidth;
    _numSqHeight = numSqHeight;

    _sqSize = sqSize;

    map = new double[numSqWidth*numSqHeight];

    for (int i = 0; i < landmarks.size(); i++) {
        Particle landmark = landmarks.at(i);
        Particle mapPosition(this->getColFromX(landmark.x()),
                this->getRowFromY(landmark.y()));
        this->landmarks.push_back(mapPosition);
    }

    clear();
}

WorldMap::~WorldMap(){
    delete[] map;
}

void WorldMap::decreaseProb(double percent)
{
    for (int x = 0; x < this->_numSqWidth; x++)
        for (int y = 0; y < this->_numSqHeight; y++)
            field(x, y, field(x, y) * (percent / 100.0));

    /* Set landmark positions as infinate. */
    for (int i = 0; i < this->landmarks.size(); i++) {
        Particle landmark = this->landmarks.at(i);
        field(landmark.x(), landmark.y(), DBL_MAX);
    }
}

void WorldMap::clear(){
    memset(map, 0, _numSqWidth*_numSqHeight);
}

void WorldMap::field(int col, int row, double prob){
    if (col > _numSqWidth  ||
        row > _numSqHeight ||
        col < 0 ||
        row < 0) {
      cout << "Field out of bounds (" << col << ", " << row << ")" << endl;
      return;
    }

    field(col, row) = prob;
}

double& WorldMap::field(int col, int row){
    assert(col < _numSqWidth);
    assert(row < _numSqHeight);

    return map[(col*_numSqHeight)+row];
}

double* WorldMap::operator[] (int col){
    return &field(col, 0);
}

double WorldMap::width(){
    return _width;
}

double WorldMap::height(){
    return _height;
}

int WorldMap::numSquareWidth(){
    return _numSqWidth;
}

int WorldMap::numSquareHeight(){
    return _numSqHeight;
}

int WorldMap::squareSize(){
    return _sqSize;
}

int WorldMap::getRowFromY(double y){
    int row = y/_sqSize;
    return row;
}

int WorldMap::getColFromX(double x){
    int col = x/_sqSize;
    return col;
}

double& WorldMap::fieldAt(double x, double y){
    return field(getColFromX(x), getRowFromY(y));
}

void WorldMap::print() {
    for (int y = 0; y < this->numSquareHeight(); y++) {
        for (int x = 0; x < this->numSquareWidth(); x++) {
            if (field(x, y) == 0)
                cout << " . ";
            else
                cout << " X ";
        }

        cout << endl;
    }
}

void WorldMap::markFrom(Particle robot, Particle obstacle, double prob) {
  int robX = this->getColFromX(robot.x());
  int robY = this->getRowFromY(robot.y());

  // Keep increasing the distance to the obstacle until
  // it is not in the same square as the robot.
  while (robX == this->getColFromX(obstacle.x()) &&
         robY == this->getRowFromY(obstacle.y())) {
    obstacle.sub(robot);
    obstacle.addLength((double)this->squareSize());
    obstacle.add(robot);
  }

  // Mark the tile
  this->field( this->getColFromX(obstacle.x())
             , this->getRowFromY(obstacle.y())
             , prob);
}

void WorldMap::print(vector<Particle> &path, Particle curPos) {
    for (int y = 0; y < this->numSquareHeight(); y++) {
        for (int x = 0; x < this->numSquareWidth(); x++) {

            if (this->getColFromX(curPos.x()) == x &&
                    this->getRowFromY(curPos.y()) == y) {
                cout << " R ";
                continue;
            }

            bool onPath = false;

            for (int i = 0; i < path.size(); i++) {
                if (this->getColFromX(path[i].x()) == x &&
                    this->getRowFromY(path[i].y()) == y) {
                    onPath = true;
                    break;
                };
            }

            if (onPath) {
                cout << " * ";
            }
            else if (field(x, y) <= 0.001)
                cout << " . ";
            else
                cout << " @ ";
        }

        cout << endl;
    }
}

double WorldMap::besideObstacle(int col, int row) {
    double sum = 0;
    double n   = 0;

    if (col+1 < numSquareWidth()) {
        n++;
        sum += field(col+1, row);
    }
    if (col-1 >= 0) {
        n++;
        sum += field(col-1, row);
    }
    if (row+1 < numSquareHeight()) {
        n++;
        sum += field(col, row+1);
    }
    if (row-1 > 0) {
        n++;
        sum += field(col, row-1);
    }

    return sum / n;
}

//                PATH-FINDING                //
// ------******------******------******------ //

PathNode::PathNode( int x
                  , int y
                  , int goalX
                  , int goalY
                  , WorldMap *map) {
    this->_x = x;
    this->_y = y;
    this->_h = heuristic(goalX, goalY, map);
    this->giveParent(NULL);
}

PathNode::PathNode( int x
                  , int y
                  , int goalX
                  , int goalY
                  , PathNode *parent
                  , WorldMap *map) {
    this->_x = x;
    this->_y = y;
    this->_h = heuristic(goalX, goalY, map);
    this->giveParent(parent);
}

void PathNode::calculateG() {
    if (this->parent() == NULL) {
        this->_g = 0;
        return;
    }

    if (this->parent()->x() == this->x())
        this->_g = STRAIGHT_COST;
    else if (this->parent()->y() == this->y())
        this->_g = STRAIGHT_COST;
    else
        this->_g = DIAGONAL_COST;

    this->_g += this->parent()->g();
}

void PathNode::giveParent(PathNode *parent) {
    this->_parent = parent;
    this->calculateG();
};

int PathNode::f() {
    return this->g() + this->_h;
}

int PathNode::heuristic(int goalX, int goalY, WorldMap *map) {
    int movement = abs(goalX - this->x())
                 + abs(goalY - this->y())
                 * STRAIGHT_COST;

    int obstacle = map->besideObstacle(this->x(), this->y())
                 ? BESIDE_COST
                 : 0;

    double prob = map->field(this->x(), this->y());

    int nprob = 0.5 + ((double)prob) * PROB_ADJUST;

    return movement + obstacle + nprob;
}

bool byF(PathNode *a, PathNode *b) {
    return a->f() < b->f();
};

vector<Particle> WorldMap::findPath( Particle &start
                                   , Particle &goal) {
    vector<PathNode*> open;
    vector<PathNode*> closed;

    int goalX = this->getColFromX(goal.x());
    int goalY = this->getRowFromY(goal.y());

    PathNode *startNode = new PathNode(
            this->getColFromX(start.x()),
            this->getRowFromY(start.y()),
            goalX,
            goalY,
            NULL,
            this
            );

    if (startNode->x() == goalX && startNode->y() == goalY) {
        return vector<Particle>();
    }

    if (this->field(startNode->x(), startNode->y())) {
        this->field(startNode->x(), startNode->y(), false);
    }

    cout << "Finding path from (" << startNode->x() << ", "
                                  << startNode->y() << ") "
         << "to (" << goalX << ", " << goalY << ")" << endl;

    PathNode *goalNode;

    open.push_back(startNode);

    while (true) {

        // If there are no nodes in 'open' there is no path.
        if (open.size() == 0) {
            return vector<Particle>();
        }

        // The current square is the one with the minimal f value.
        PathNode *current = *min_element(open.begin(), open.end(), byF);

        // Move the current square from open to closed.
        for (int i = 0; i < open.size(); i++) {
          if (open[i] == current) {
            open.erase(open.begin() + i);
            break;
          }
        }

        closed.push_back(current);

        // If the current square is the goal, we are done
        if (current->x() == goalX && current->y() == goalY) {
            goalNode = current;
            break;
        }

        // Find and check all the tiles reachable from here
        vector<PathNode*> walkable =
            this->getWalkable(current->x(), current->y(), goalX, goalY);

        for (int i = 0; i < walkable.size(); i++) {
            PathNode *tryOut = walkable[i];

            bool inClosed = false;

            // If tryOut is in the closed set, we have already checked it out
            for (int i = 0; i < closed.size(); i++) {
                PathNode *other = closed[i];

                if (other->x() == tryOut->x() && other->y() == tryOut->y()) {
                    delete tryOut;
                    inClosed = true;
                    break;
                }
            }

            if (inClosed) continue;

            tryOut->giveParent(current);
            bool inOpen = false;

            // Check if the same tile is already in open.
            for (int j = 0; j < open.size(); j++) {
                PathNode *other = open[j];
                if (other->x() == tryOut->x() &&
                    other->y() == tryOut->y() &&
                    other->f() > tryOut->f()) {

                    // If the other tile has a higher f, we
                    // have found a better path to the same tile.
                    other->giveParent(current);
                    inOpen = true;
                    break;
                }
            }

            if (!inOpen) {
                open.push_back(tryOut);
                continue;
            }

            // If we reach here there is no more use for tryOut
            delete tryOut;
        }
    }

    // Trace back through the path and calculate positions
    vector<Particle> result;

    PathNode *root = goalNode;

    double offset = (double)this->squareSize() / 2.0;

    while (root != NULL) {
        // cout << "(" << root->x() << ", " << root->y() << ")" << endl;

        Particle p( this->squareSize() * root->x() + offset
                  , this->squareSize() * root->y() + offset);

        result.push_back(p);

        root = root->parent();
    }

    // The path comes out in reverse, unreverse it
    reverse(result.begin(), result.end());
    result.erase(result.begin());

    // Cleanup
    for (int i=0; i<closed.size(); i++) delete closed[i];
    for (int i=0; i<open.size();   i++) delete open[i];

    return result;
}

vector<PathNode*> WorldMap::getWalkable(int x, int y, int goalX, int goalY) {
    vector<PathNode*> walkable;

    bool up    = false;
    bool down  = false;
    bool left  = false;
    bool right = false;

    // Check sides
    if (y+1 < this->numSquareHeight()) {
        right = true;
        walkable.push_back(new PathNode(x, y + 1, goalX, goalY, this));
    }

    if (y-1 >= 0) {
        left = true;
        walkable.push_back(new PathNode(x, y - 1, goalX, goalY, this));
    }

    // Check sides
    if ( x+1 < this->numSquareWidth()) {
        up = true;
        walkable.push_back(new PathNode(x + 1, y , goalX, goalY, this));
    }

    if (x-1 >= 0) {
        down = true;
        walkable.push_back(new PathNode(x - 1, y , goalX, goalY, this));
    }

    // Check diagonals
    // We must only move to diagnal tiles if they are not
    // adjacet to an unwalkable tile.
    //if (up && right && !this->field(x + 1, y + 1)) {
        //walkable.push_back(new PathNode(x + 1, y + 1, goalX, goalY, this));
    //}

    //if (up && left && !this->field(x + 1, y - 1)) {
        //walkable.push_back(new PathNode(x + 1, y - 1, goalX, goalY, this));
    //}

    //if (down && left && !this->field(x - 1, y - 1)) {
        //walkable.push_back(new PathNode(x - 1, y - 1, goalX, goalY, this));
    //}

    //if (down && right && !this->field(x - 1, y + 1)) {
        //walkable.push_back(new PathNode(x - 1, y + 1, goalX, goalY, this));
    //}

    return walkable;
}
