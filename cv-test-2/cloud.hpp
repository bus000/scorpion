#ifndef CLOUD_HDR
#define CLOUD_HDR

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <vector>
#include <string>
#include <stdlib.h>
#include "locator.hpp"
#include "math.h"

using namespace std;

vector<Point> findCentroids(int k, int init, vector<Point> points, int maxIter = 100);

vector<Point> correlate(vector<Point> guide, vector<Point> points);

vector<Point> combine(vector<Point> p1, vector<Point> p2);

Point reduce(vector<Point> points);

Point rotateAround(Point origin, Point p, double angle);

double radius(vector<Point> points);

double hullHeight(vector<Point> hull);

#endif
