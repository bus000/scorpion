#include "worldMap.hpp"

WorldMap::WorldMap(double width, double height, int numSqWidth, int numSqHeight){
    _width = width;
    _height = height;
    _numSqWidth = numSqWidth;
    _numSqHeight = numSqHeight;

    sqWidth = width/(double)numSqWidth;
    sqHeight = height/(double)numSqHeight;

    map = new bool[numSqWidth*numSqHeight];
    clear();
}

WorldMap::~WorldMap(){
    delete[] map;
}

void WorldMap::clear(){
    memset(map, 0, _numSqWidth*_numSqHeight);
}

void WorldMap::field(int col, int row, bool mark){
    field(col, row) = mark;
}

bool& WorldMap::field(int col, int row){
    assert(col < _numSqWidth);
    assert(row < _numSqHeight);
    return map[(col*_numSqWidth)+row];
}

bool* WorldMap::operator[] (int col){
    return &field(col, 0);
}

double WorldMap::width(){
    return _width;
}

double WorldMap::height(){
    return _height;
}

int WorldMap::squareWidth(){
    return _numSqWidth;
}

int WorldMap::squareHeight(){
    return _numSqHeight;
}

int WorldMap::getRowFromY(double y){
    double row = y/sqHeight;
    return (int)row;
}

int WorldMap::getColFromX(double x){
    double col = x/sqWidth;
    return (int)col;
}

bool& WorldMap::fieldAt(double x, double y){
    return field(getColFromX(x), getRowFromY(y));
}

//                PATH-FINDING                //
// ------******------******------******------ //


PathNode::PathNode( int x
                  , int y
                  , int goalX
                  , int goalY) {
    this->_x = x;
    this->_y = y;
    this->_h = manhattan(goalX, goalY);
    this->giveParent(NULL);
}

PathNode::PathNode( int x
                  , int y
                  , int goalX
                  , int goalY
                  , PathNode *parent) {
    this->_x = x;
    this->_y = y;
    this->_h = manhattan(goalX, goalY);
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

int PathNode::manhattan(int goalX, int goalY) {
  return ( abs(goalX - this->x()) 
         + abs(goalY - this->y())) 
         * STRAIGHT_COST;
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
        NULL
    );

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
      open.erase(find(open.begin(), open.end(), current));
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

          // If tryOut is in the closed set, we have already checked it out
          if (find(closed.begin(), closed.end(), tryOut) != closed.end()) {
              delete tryOut;
              continue;
          }

          tryOut->giveParent(current);
          bool inOpen = false;
          
          for (int j = 0; j < open.size(); j++) {
              PathNode *other = open[i];
              if (other->x() == tryOut->x() &&
                  other->y() == tryOut->y() &&
                  other->g() > tryOut->g()) {
                  //
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

    while (true) {
      Particle p( this->squareWidth()  * root->x() + this->squareWidth()  / 2.0
                , this->squareHeight() * root->y() + this->squareHeight() / 2.0);

      result.push_back(p);

      if (root->parent() == NULL) 
        break;
      else
        root = root->parent();
    }
    
    // The path comes out in reverse, unreverse it
    reverse(result.begin(), result.end());

    // Cleanup
    for (int i=0; i<closed.size(); i++) delete closed[i];
    for (int i=0; i<open.size();   i++) delete open[i];

    return result; 
}

vector<PathNode*> WorldMap::getWalkable(int x, int y, int goalX, int goalY) {
  vector<PathNode*> walkable;

  bool up, down, left, right = false;

  // Check sides
  if (this->field(x, y+1) && y+1 < this->height()) {
    up = true;
    walkable.push_back(new PathNode(x, y + 1, goalX, goalY));
  }

  if (!this->field(x, y-1) && y-1 >= 0) {
    down = true;
    walkable.push_back(new PathNode(x, y - 1, goalX, goalY));
  }

  // Check sides
  if (!this->field(x+1, y) && x+1 < this->width()) {
    left = true;
    walkable.push_back(new PathNode(x + 1, y , goalX, goalY));
  }

  if (!this->field(x-1, y) && x-1 >= 0) {
    right = true;
    walkable.push_back(new PathNode(x - 1, y , goalX, goalY));
  }

  // Check diagonals
  // We must only move to diagnal tiles if they are not
  // adjacet to an unwalkable tile.
  if (!this->field(x + 1, y + 1) && right && up) {
    walkable.push_back(new PathNode(x + 1, y + 1, goalX, goalY));
  }

  if (!this->field(x - 1, y + 1) && left && up) {
    walkable.push_back(new PathNode(x - 1, y + 1, goalX, goalY));
  }

  if (!this->field(x - 1, y - 1) && left && down) {
    walkable.push_back(new PathNode(x - 1, y - 1, goalX, goalY));
  }

  if (!this->field(x + 1, y - 1) && right && down) {
    walkable.push_back(new PathNode(x + 1, y - 1, goalX, goalY));
  }

  return walkable;
}
