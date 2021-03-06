#include "locator.hpp"
#include <stdio.h>
#include <iostream>

int mu = 0;
int k = 200;

void initHullWindow(string WindowName) {
    namedWindow(WindowName.c_str(), CV_WINDOW_NORMAL);
    createTrackbar("k:", WindowName.c_str(), &k, 1000);
    createTrackbar("mu:", WindowName.c_str(), &mu, 255);
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
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    findContours(src, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

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
        cout << "Hull size: "  << hull[i].size() << endl;
        double circ = hullCircum(hull[i]);

        if (circ > biggetCirc) {
            biggetCirc = circ;
            *biggestHull = hull[i];
        }
    }

    return biggestHull;
}

void drawHull(vector<Point> hull, Scalar color, Mat &dst) {
    for( int i = 1; i < hull.size(); i++ )
    {
        Point a = hull[i-1];
        Point b = hull[i];

        line(dst, a, b, color);
    }
}

Point hullCenter(vector<Point> *hull) {
    Point sum(0.0, 0.0);
    int count = 0;

    for (int j = 0; j < hull->size(); j++) {
        sum += hull->at(j);
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

    ColorFilter(frameHSV, redOnly, mu, k);
    cvtColor(redOnly, redOnlyBGR, COLOR_HSV2BGR);
    ToBin(redOnly, redOnly, 5);

    return findHull(redOnly);
}
