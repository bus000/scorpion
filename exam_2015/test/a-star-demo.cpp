#include <algorithm>
#include <iostream>
#include <vector>
#include <math.h>
#include <unistd.h>

using namespace std;

#define STRAIGHT_COST 10
#define DIAGONAL_COST 14 // sqrt(2);

class Tup {
  public:
    int x;
    int y;

    Tup() {
      this->x = 0;
      this->y = 0;
    }

    Tup(int xP, int yP) {
      this->x = xP;
      this->y = yP;
    };

    ~Tup() {};

    int manhattan(Tup goal) {
      return abs(goal.x - this->x) + abs(goal.y - this->y); 
    };

    Tup add(int a, int b) {
      return Tup(this->x + a, this->y + b);
    };

    bool operator==(Tup& rhs) {
      return rhs.x == this->x &&
             rhs.y == this->y;
    };
};

class PathNode {
  public:
    PathNode *parent;
    Tup position;
    int g;
    int h;

    PathNode() {};
    
    PathNode(Tup pos, Tup goal) {
      this->parent = NULL;
      this->position = pos;
      this->h = pos.manhattan(goal) * STRAIGHT_COST;

      this->calcG();
    };

    PathNode(Tup pos, Tup goal, int weight) {
      this->parent = NULL;
      this->position = pos;
      this->h = pos.manhattan(goal) * STRAIGHT_COST;
      this->calcG();
    };

    void calcG() {
      if (this->parent == NULL) {
        this->g = 0;
        return;
      }

      if (this->parent->position.x == this->position.x)
        this->g = STRAIGHT_COST;

      else if (this->parent->position.y == this->position.y)
        this->g = STRAIGHT_COST;

      else
        this->g = DIAGONAL_COST;

      this->g += parent->g; 
    };

    void setParent(PathNode *parent) {
      this->parent = parent;

      this->calcG();
    };

    bool operator==(PathNode& rhs) {
      return this->position == rhs.position;
    };

    ~PathNode() {};

    int getF() { return g + h; };
};


void printMap(vector<vector<int> > map, int width, int height);
vector<Tup> aStar(Tup start, Tup goal, vector<vector<int> > &map);

int main() {
    int mapWidth = 20;
    int mapHeight = 20;
    vector<vector<int> > map;

    for (int i = 0; i < mapWidth; i++) {
      vector<int> col;
      map.push_back(col);

      for (int j = 0; j < mapHeight; j++) {
        map[i].push_back(0);
      }
    }

    for (int j = 0; j < mapHeight; j++) {
        map[5][j] = 1;
    }

    map[5][13] = 0;
    map[5][1] = 0;
    map[6][10] = 1;
    map[7][10] = 1;

    for (int j = 0; j < mapHeight; j++) {
        map[8][j] = 1;
    }

    map[8][17] = 0;

    for (int j = 0; j < mapHeight; j++) {
        map[11][j] = 1;
    }

    map[11][10] = 0;

    for (int j = 0; j < mapHeight; j++) {
        map[13][j] = 1;
    }

    map[13][18] = 0;
    

    vector<Tup> path = aStar(Tup(1, 3), Tup(15, 10), map);
    reverse(path.begin(), path.end());

    for (int i = 0; i < path.size(); i++) {
      for (int j = 0; j < i; j++) {
        map[path[i].x][path[i].y] = 2;
      }

      cout << "\033[2J\033[1;1H";
      printMap(map, mapWidth, mapHeight);
      usleep(100000);
    }
    
    cout << "done" << endl;
    return 0;
}

vector<PathNode*> getWalkable(Tup start, Tup goal, vector<vector<int> > &map) {
  vector<PathNode> candidates;

  // All the eight adjacent squares
  candidates.push_back(PathNode(start.add( 1,  0), goal));
  candidates.push_back(PathNode(start.add( 1,  1), goal));
  candidates.push_back(PathNode(start.add( 0,  1), goal));
  candidates.push_back(PathNode(start.add(-1,  1), goal));
  candidates.push_back(PathNode(start.add(-1,  0), goal));
  candidates.push_back(PathNode(start.add(-1, -1), goal));
  candidates.push_back(PathNode(start.add( 0, -1), goal));
  candidates.push_back(PathNode(start.add( 1, -1), goal));

  vector<PathNode*> walkable;

  for (int i = 0; i < candidates.size(); i++) {
    PathNode candidate = candidates[i];

    // Check if x is out of bounds
    if (candidate.position.x >= map.size() || candidate.position.x < 0) continue;

    // Check if y is out of bounds
    if (candidate.position.y >= map[0].size() || candidate.position.y < 0) continue;

    // Check if position is unwalkable
    if (map[candidate.position.x][candidate.position.y] != 0) continue;

    walkable.push_back(new PathNode(candidate.position, goal));
  }
  
  return walkable; 
};

int inPathNodeVector(PathNode node, vector<PathNode*> vec) {
  for (int i = 0; i < vec.size(); i++) {
    if (*vec[i] == node) {
      return i;
    } 
  }

  return -1;
}

vector<Tup> aStar(Tup start, Tup goal, vector<vector<int> > &map) {
  vector<PathNode*> open;
  vector<PathNode*> closed;

  open.push_back(new PathNode(start, goal));

  int goalI = 0;

  while (true) {
    if (open.size() == 0) {
      cout << "Failed to find path" << endl;
      vector<Tup> res;
      return res;
    }

    // Find the lowest f in the open list
    int minF = 10000;
    int currentI = 0;
    PathNode *current;

    for (int i = 0; i < open.size(); i++) {
      int f = open[i]->getF();
      if (f < minF) {
        minF = f;
        currentI = i;
        current = open[i];
      }
    }

    // Move the current node to the closed list
    open.erase(open.begin() + currentI);
    closed.push_back(current);

    // Find all walkable squares
    vector<PathNode*> walkables = getWalkable(current->position, goal, map);
    
    for (int i = 0; i < walkables.size(); i++) {
      PathNode *walkable = walkables[i];

      if (inPathNodeVector(*walkable, closed) != -1) {
        delete walkable;
        continue;
      }

      int openI = inPathNodeVector(*walkable, open);

      if (openI == -1) {
        walkable->setParent(current);
        open.push_back(walkable);
        continue;
      }

      PathNode *dup = open[openI];

      if (dup->g < walkable->g) {
        dup->setParent(current);
        continue;
      }
    }

    // Stop if the goal is in the closed list.
    goalI = inPathNodeVector(PathNode(goal, goal), closed);

    if (goalI != -1)
      break;
  }

  vector<Tup> path;
  PathNode *root = closed[goalI];

  while (true) {
    path.push_back(root->position);

    if (root->parent == root) {
      cout << "Cyclic path" << endl;
      break;
    }

    if (root->parent == NULL) {
      break;
    }

    root = root->parent;
  }

  for (int i = 0; i < closed.size(); i++) {
    delete closed[i];
  }

  for (int i = 0; i < open.size(); i++) {
    delete open[i];
  }

  return path; 
}

void printMap(vector<vector<int> > map, int width, int height) {

    for (int y = 0; y < height; y++) {
      for (int x = 0; x < width; x++) {
        cout << " ";
        switch(map[x][y]) {
          case 1:
            cout << "🌺";
            break;
          case 2:
            cout << "🌹";
            break;
          default:
            cout << "🌼";
            break;
        }
        cout << " ";
      }

      cout << endl;
    }

    cout << endl;
}
