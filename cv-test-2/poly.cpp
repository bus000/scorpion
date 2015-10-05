#include <poly.hpp>

vector<Point> findPoly(int vertices, vector<Point> points) {
  double angle = (2 * M_PI) / ((double)vertices); 
  double dist  = radius(points);
  Point center = polyCenter(points);
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

bool lineIsShorter(Line l1, Line l2) {
  return l1.length() < l2.length(); 
}

double polyHeight(vector<Point> poly) {
  auto lines = horizontalLines(poly); 

  return max_element(lines.begin(), lines.end(), lineIsShorter)->length();
}

double polyWidth(vector<Point> poly) {
  auto lines = verticalLines(poly);

  return max_element(lines.begin(), lines.end(), lineIsShorter)->length();
}

Point polyCenter(vector<Point> poly) {
    Point sum(0.0, 0.0);
    int count = 0;

    for (int j = 0; j < poly.size(); j++) {
        sum += poly[j];
        count += 1;
    }

    Point center = sum * (1.0/(double)count);

    return center;
}

vector<Line> verticalLines(vector<Point> poly) {
  vector<Line> result;

  for (int i = 0; i <= poly.size(); i++) {
    int j = i == 0 ? (poly.size() - 1) : (i - 1);

    Point a = poly[j];
    Point b = poly[i];
    
    if (abs(a.x - b.x) < (abs(a.y - b.y))) {
      result.push_back(Line(a, b));
    }
  }

  return result;
}

vector<Line> horizontalLines(vector<Point> poly){
  vector<Line> result;

  for (int i = 0; i < poly.size(); i++) {
    int j = i == 0 ? (poly.size() - 1) : (i - 1);

    Point a = poly[j];
    Point b = poly[i];
    
    if (abs(a.y - b.y) < (abs(a.x - b.x))) {
      result.push_back(Line(a, b));
    }
  }

  return result;
}

/* The following functions depend on camera parameters. For a MacBook Pro this
 * is:
 *
 * > camera fx: 1058.051100
 * > camera fy: 1068.394230
 */

double distanceToPoly(double realHeight, double pixelHeight, double camerafy) {
  return camerafy * (realHeight / pixelHeight);
}

/* b = fx * (B cos(v) / Z)
 * b = fx * (B/Z) * cos(v)
 * b / (fx * (B/Z)) = cos(v)
 * v = arccos(b / fx * (B/Z))
 */
double angleToPoly(double distance, double realWidth, double pixelWidth, double camerafx) {
  return acos(pixelWidth /  (camerafx * (realWidth / distance)));
}
