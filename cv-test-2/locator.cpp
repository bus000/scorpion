#include "locator.hpp"
#include <stdio.h>
#include <math.h>


int mu = 0;
int k = 150;
int satWeight = 100;
int minSat = 50;

void initHullWindow(string WindowName) {
    namedWindow(WindowName.c_str(), CV_WINDOW_NORMAL);
    createTrackbar("k:", WindowName.c_str(), &k, 1000);
    createTrackbar("mu:", WindowName.c_str(), &mu, 180);
    createTrackbar("minSat:", WindowName.c_str(), &minSat, 255);
}

double hullCircum(vector<Point> hull) {
    double circum = 0.0;

    for (int i = 1; i < hull.size(); i++) {
        Point a = hull[i-1];
        Point b = hull[i];

        circum += norm(b - a); 
    }

    return abs(circum);
}

vector<Point> *findHull(Mat src) {
    RNG rng(12345);

    Mat denoized;
    GaussianBlur(src, denoized, Size(5, 5), 0);
    GaussianBlur(denoized, denoized, Size(5, 5), 0);

    // Convert to lab
    Mat imgLab;
    cvtColor(denoized, imgLab, CV_BGR2Lab);

    // Split channels
    vector<Mat> channels;
    split(imgLab, channels);

    Mat threshed;
    threshold(channels[0], threshed, 40, 255, THRESH_BINARY);

    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    findContours(threshed, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

    vector<vector<Point> > hull(contours.size());
    for( int i = 0; i < contours.size(); i++ ) {
        convexHull( Mat(contours[i]), hull[i], false );
    }

    if (hull.size() < 1) {
        return NULL;
    }

    vector<Point> *biggestHull = new vector<Point>;
    double biggetCirc = 0.0;

    for (int i = 0; i < hull.size(); i++) {
        double circ = hullCircum(hull[i]);

        if (circ > biggetCirc) {
            biggetCirc = circ;
            *biggestHull = hull[i];
        }
    }

    return biggestHull;
}

void drawHull(vector<Point> hull, Scalar color, Mat &dst) {
    for( int i = 1; i <= hull.size(); i++ )
    {
        int j = i % hull.size();
        Point a = hull[i-1];
        Point b = hull[j];

        line(dst, a, b, color, 3);
    }
}

Point hullCenter(vector<Point> hull) {
    Point sum(0.0, 0.0);
    int count = 0;

    for (int j = 0; j < hull.size(); j++) {
        sum += hull[j];
        count += 1;
    }

    Point center = sum * (1.0/(double)count);

    return center;
}

vector<Point>* getHull(Mat src) {
    Mat frameHSV;
    Mat redOnly;
    Mat redOnlyBGR;
    Mat withKeyPoints;
    Mat channels[3];

    cvtColor(src, frameHSV, COLOR_BGR2HSV);
    split(frameHSV, channels);

    ColorFilter(frameHSV, redOnly, mu, k, minSat);
    cvtColor(redOnly, redOnlyBGR, COLOR_HSV2BGR);

    return findHull(redOnly);
}

vector<Point> findPoints(vector<Point> hull, int target, double step, double minT, double maxT) {
  for (int i = 1; i < hull.size(); i++) {
    double t = minT;
    for (; t < maxT; t += step) {
      auto points = filterHull(hull, i, t);

      if (points.size() == target) {
        return points;
      }
    }
  }
 
  return hull; 
}

vector<Point> filterHull(vector<Point> hull, int clusterSize, double threshold) {
  vector<Point> filtered;

  vector<vector<Point> > clusters;
  vector<Point> first;
  first.push_back(hull[0]);
  clusters.push_back(first);
  
  for (int i = 1; i < hull.size(); i++) {
    auto cluster = &clusters.back();
    auto a = cluster->back();
    auto b = hull[i];
    
    if (norm(b-a) < threshold) {
      cluster->push_back(b);
    }
    else {
      vector<Point> newCluster;
      newCluster.push_back(b);
      clusters.push_back(newCluster);
    }
  }

  for(int i = 0; i < clusters.size(); i++) {
    auto cluster = clusters[i];

    if (cluster.size() >= clusterSize) {
      for (int j = 0; j < cluster.size(); j++) {
        Point c = hullCenter(cluster);
        filtered.push_back(c);
      } 
    }
  }

  return filtered;
}
