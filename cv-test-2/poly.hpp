#ifndef _POLY_
#define _POLY_

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <vector>
#include <string>
#include <stdlib.h>
#include "locator.hpp"
#include "math.h"
#include "cloud.hpp"

class Line {
public:
  Point a;
  Point b;

  Line(Point a, Point b) {
    this->a = a;
    this->b = b;
  };

  ~Line() {};

  double length() { return norm(a-b); };

  vector<Point> toPoints() {
    vector<Point> result;
    result.push_back(a);
    result.push_back(b);
    return(result);
  };

  double width()  { return abs(a.x - b.x); };
  double height() { return abs(a.y - b.y); };
};

vector<Point> findPoly(int verticies, vector<Point> point);

double polyHeight(vector<Point> poly);

double polyWidth(vector<Point> poly);

Point polyCenter(vector<Point> poly);

vector<Line> horizontalLines(vector<Point> poly);

vector<Line> verticalLines(vector<Point> poly);

double distanceToPoly(double realHeight, double pixelHeight, double camerafy);

double angleToPoly(double distance, double realWidth, double pixelWidth, double camerafx);

#endif
