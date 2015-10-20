#include "kmeans.hpp"
#include "stdio.h"

class Group {
public:
  vector<Point> points;
  Point centroid;

  Group(Point centroid, vector<Point>){
    this->centroid = centroid;
    this->points = points;
  }

  ~Group() {};
};

int random(int min, int max) {
  return (rand() % (max-min)) + min;
}

Point randomPoint(int min, int max) {
   int xOff = random(min, max);
   int yOff = random(min, max);
   return Point(xOff, yOff);
}

// A modified k-means clustering algorithm, where output points is the reduction
// of each cluster.
vector<Point> findCentroids(int k, int init, vector<Point> points, int maxIter) {
  vector<Group> groups;

  Point center = hullCenter(points);

  // Init groups to a random point
  for(int i = 0; i < k; i++) {
   Point c = center + randomPoint(-init, init); 

   Group group(c, points);
   groups.push_back(group);
  }

  bool cont = true;
  int iter = 0;

  while(cont) {
    iter++;
    

    vector<Point> centroids;

    // Save centroids and clear points 
    for (int i = 0; i < k; i++) {
      centroids.push_back(groups[i].centroid);
      (&(groups[i].points))->clear();
    }

    // Assing points to the closest centroid
    for (int i = 0; i < points.size(); i++) {
      int closest = 0;
      double dist = 100000000.0;
      Point p = points[i];

      for (int j = 0; j < k; j++) {
       Point c = groups[j].centroid;
       double d = norm(c-p);
       if (d < dist) {
         closest = j;
         dist = d; 
       }
      }

      (&(groups[closest].points))->push_back(p);
    }

    cont = false;

    // Relocate centroids and check for change.
    for (int i = 0; i < k; i++) {
      Point c = groups[i].centroid;
      
      if (c == Point(0, 0))
        (&groups[i])->centroid = center + randomPoint(-init, init);
      else
        (&groups[i])->centroid = hullCenter(groups[i].points);
      
      // Check if centroid has not changed.
      if (norm(groups[i].centroid - centroids[i]) > 0.001) {
        cont = true; 
      }
    }

    if (iter > maxIter) {
      break;
    }
  }

  // Extract centroids.
  vector<Point> centroids;

  for (int i = 0; i < k; i++) {
    Point reduced = reduce(groups[i].points);
    centroids.push_back(reduced);
  }

  printf("Found centroids after %d iterations.\n", iter);

  return centroids;
}

// Returns the vector of points in 'points' which are closest to the 
// points in 'guide'.
vector<Point> correlate(vector<Point> guide, vector<Point> points) {
  vector<Point> result;

  for (int i = 0; i < guide.size(); i++) {
    Point t = Point(100000, 100000);
    
    Point a = guide[i];

    for (int j = 0; j < points.size(); j++) {
      Point b = points[j];

      if (norm(a - b) < norm(a - t)) {
        t = b;
      }
    }

    result.push_back(t);
  }

  return result;
}

// Reduce a vector of points to a single point by removing
// the point furthest from the mean, until only one point is left.
Point reduce(vector<Point> points) {
  vector<Point> temp = points;

  while (temp.size() > 1) {
    Point c = hullCenter(temp);
    Point m = c;
    int rm = 0;

    for (int i = 0; i < temp.size(); i++) {
      Point a = temp[i];

      if (norm(c - a) > norm(c - m)) {
        m = a;
        rm = i;
      }
    }

    temp.erase(temp.begin() + rm);
  }

  if (temp.size() < 1) {
    return Point(0, 0);
  }

  return temp[0];
}

Point rotateAround(Point origin, Point p, double angle) {
  // Translate
  Point tp = p - origin;

  Point rp = Point(tp.x * cos(angle) - tp.y * sin(angle), tp.x * sin(angle) + tp.y * cos(angle));

  return rp + origin;
}

double radius(vector<Point> points) {
  double max   = 0;
  Point center = hullCenter(points);

  for (int i = 0; i < points.size(); i++) {
    double d = norm(points[i] - center);
    if (d > max) {
      max = d;
    } 
  }

  return max;
}

vector<Point> findPoly(int vertices, vector<Point> points) {
  double angle = (2 * M_PI) / ((double)vertices); 
  double dist  = radius(points);
  Point center = hullCenter(points);
  Point offset = center + Point(0, 2 * dist);
  vector<Point> result;
  vector<Point> polygon;

  Point first = rotateAround(center, offset, - (0.5 * angle));
  polygon.push_back(first);

  for (int i = 0; i < vertices - 1; i++) {
    Point p = rotateAround(center, first, angle * (i+1));
    polygon.push_back(p);
  }

  return correlate(polygon, points);
 }

// The pairwise mean of points in p1 and p2.
vector<Point> combine(vector<Point> p1, vector<Point> p2) {
  vector<Point> result;

  for (int i = 0; i < p1.size(); i++) {
    Point sum = p1[i] + p2[i];
    result.push_back(Point(sum.x / 2, sum.y / 2));
  }

  return result;
}
