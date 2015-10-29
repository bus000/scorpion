#ifndef MAP_PRESENTER_H
#define MAP_PRESENTER_H
#include "worldMap.hpp"
#include <opencv2/core/core.hpp>

#define PPS 25

class MapPresenter {
public:
    MapPresenter(WorldMap *wm, int height = 0, int width = 0);
    ~MapPresenter();
    cv::Mat* draw();
private:
    cv::Mat *internalImg;
    cv::Mat *externalImg;
    int _height, _width;

    void drawBoard();
};

#endif
