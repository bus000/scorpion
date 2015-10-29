#include "mapPresenter.hpp"

MapPresenter::MapPresenter(WorldMap *wm, int height, int width){
    _height = height;
    _width = width;

    if(height != 0)
        externalImg = new cv::Mat(height, width, CV_64FC4);
    else
        externalImg = NULL;

    internalImg = new cv::Mat(wm->numSquareHeight()*PPS,
            wm->numSquareWidth()*PPS, CV_64FC4);
}

MapPresenter::~MapPresenter(){
    delete internalImg;
    delete externalImg;
}

cv::Mat* MapPresenter::draw(){
    internalImg.clear();
    drawBoard();
}

void MapPresenter::drawBoard(){
    for(int i = 0; 
}
