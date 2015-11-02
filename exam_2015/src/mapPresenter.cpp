//#include "mapPresenter.hpp"
//#include <iostream>
//#include <cmath>

//MapPresenter::MapPresenter(WorldMap *wm, int PPS, int height, int width){
    //_height = height;
    //_width = width;
    //_wm = wm;
    //_PPS = PPS;
//}

//MapPresenter::~MapPresenter(){
//}


//cv::Mat MapPresenter::draw(Particle robotPosition){
    //cv:: Mat img = draw();
    //Particle square(
        //_wm->getColFromX(robotPosition.x()),
        //_wm->getRowFromY(robotPosition.y())
    //);
    //cv::Scalar robotFieldColor(0x0,0xff,0x0);
    //if(_wm->fieldAt(robotPosition.x(), robotPosition.y()))
        //robotFieldColor = cv::Scalar(0x0,0x0,0xff);

    //fillField(img, square, robotFieldColor);
    //drawRobot(img, robotPosition);
    //return img;
//}

//cv::Mat MapPresenter::draw(){
    ////clear internal matrix
    //cv::Mat img(_wm->numSquareHeight()*_PPS,
            //_wm->numSquareWidth()*_PPS, CV_64FC4);
    //drawBoard(img);
    //drawMap(img);

    //return img;
//}

//void MapPresenter::drawBoard(cv::Mat img){
    ////clear image
    //img = cv::Scalar(0xff,0xff,0xff);
    ////Loop through vertical lines
    //int vMax = _wm->numSquareWidth();
    //for(int v = 1; v < vMax; v++){
        //cv::line(
            //img,
            //cv::Point(v*_PPS, 0),
            //cv::Point(v*_PPS, img.rows-1),
            //cv::Scalar(0x0,0x0,0x0)
        //);
    //}

    ////loop through horizontal lines
    //int hMax = _wm->numSquareHeight();
    //for(int h = 1; h < hMax; h++){
        //cv::line(
            //img,
            //cv::Point(0, h*_PPS),
            //cv::Point(img.cols-1, h*_PPS),
            //cv::Scalar(0x0,0x0,0x0)
        //);
    //}

    ////borders
    //cv::line(
        //img,
        //cv::Point(0, 0),
        //cv::Point(img.cols-1, 0),
        //cv::Scalar(0x0,0x0,0x0)
    //);
    //cv::line(
        //img,
        //cv::Point(0, 0),
        //cv::Point(0, img.rows-1),
        //cv::Scalar(0x0,0x0,0x0)
    //);
    //cv::line(
        //img,
        //cv::Point(img.cols-1, 0),
        //cv::Point(img.cols-1, img.rows-1),
        //cv::Scalar(0x0,0x0,0x0)
    //);
    //cv::line(
        //img,
        //cv::Point(0, img.rows-1),
        //cv::Point(img.cols-1, img.rows-1),
        //cv::Scalar(0x0,0x0,0x0)
    //);
//}

//void MapPresenter::fillField(cv::Mat img, Particle square, cv::Scalar color){
    //int x = square.x()*_PPS;
    //int y = (_wm->numSquareHeight()-1-square.y())*_PPS;

    //cv::rectangle(
        //img,
        //cv::Point(x+1, y+1),
        //cv::Point(x+_PPS-1, y+_PPS-1),
        //color,
        //-1
    //);
//}

//void MapPresenter::drawRobot(cv::Mat img, Particle robot, cv::Scalar color){
    //if(robot.x() < 0.0 || robot.y() < 0.0 ||
            //robot.x() > _wm->width() || robot.y() > _wm->height())
        //return;

    //float scale = (float)_PPS/(float)_wm->squareSize();
    //int x = (int)floor(robot.x()*scale);
    //int y = img.rows-1-(int)floor(robot.y()*scale);
    //int radius = _PPS/4;

    ////robot circle
    //cv::circle(img, cv::Point(x, y), radius, color, 2);
    ////Direction line
    //Particle direction = Particle::createUnit(robot.theta());
    //direction.normalize();
    //direction.scale(radius*3);
    //cv::line(
        //img,
        //cv::Point(x,y),
        //cv::Point(x+(int)floor(direction.x()), y-direction.y()),
        //color,
        //2
    //);
//}

//void MapPresenter::drawMap(cv::Mat img){
    //int cols = _wm->numSquareWidth();
    //int rows = _wm->numSquareHeight();
    //for(int col = 0; col < cols; col++){
        //bool *colPtr = (*_wm)[col];
        //for(int row = 0; row < rows; row++){
            //bool fieldPtr = &colPtr[row];
            //bool field = colPtr[row];
            //if(field)
                //fillField(img, Particle(col, row), cv::Scalar(0x0,0x0,0x0));
        //}
    //}
//}
