#ifndef MAP_PRESENTER_H
#define MAP_PRESENTER_H
#include "worldMap.hpp"
#include <opencv2/core/core.hpp>
#include "particle.hpp"

class MapPresenter {
public:
  MapPresenter(WorldMap *wm, int PPS = 25, int height = 0, int width = 0);
  ~MapPresenter();
  cv::Mat draw(Particle robotPosition);
  cv::Mat draw();
  void drawRobot(cv::Mat img, Particle robot,
          cv::Scalar color = cv::Scalar(0xff,0x0,0x0));
private:
  WorldMap *_wm;
  int _height, _width, _PPS;

  void drawBoard(cv::Mat img);
  void fillField(cv::Mat img, Particle square, cv::Scalar color);
  void drawMap(cv::Mat img);
};

#endif
