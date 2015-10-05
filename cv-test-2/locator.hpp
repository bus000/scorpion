#ifndef LOCATOR
#define LOCATOR

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include "colorFilter.hpp"
#include <vector>
#include <string>

// OBOY

using namespace cv;
using namespace std;

void initHullWindow(string windowName);

void drawHull(vector<Point> hull, Scalar color, Mat &dst);

double hullCircum(vector<Point> hull);

double hullHeight(vector<Point> hull);

Point hullCenter(vector<Point> hull);

vector<Point> *findHull(Mat src);

vector<Point>* getHull(Mat src);

vector<Point> findPoints(vector<Point> hull, int target, double step, double minT, double maxT);

vector<Point> filterHull(vector<Point> hull, int clusterSize, double threshold);

#endif
