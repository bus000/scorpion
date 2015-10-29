#include "worldMap.hpp"
#include <cstring>
#include <cassert>

WorldMap::WorldMap(double width, double height, int numSqWidth, int numSqHeight){
    _width = width;
    _height = height;
    _numSqWidth = numSqWidth;
    _numSqHeight = numSqHeight;

    sqWidth = width/(double)numSqWidth;
    sqHeight = height/(double)numSqHeight;

    map = new bool[numSqWidth*numSqHeight];
    clear();
}

WorldMap::~WorldMap(){
    delete[] map;
}

void WorldMap::clear(){
    memset(map, 0, _numSqWidth*_numSqHeight);
}

void WorldMap::field(int col, int row, bool mark){
    field(col, row) = mark;
}

bool& WorldMap::field(int col, int row){
    assert(col < _numSqWidth);
    assert(row < _numSqHeight);
    return map[(col*_numSqWidth)+row];
}

bool* WorldMap::operator[] (int col){
    return &field(col, 0);
}

double WorldMap::width(){
    return _width;
}

double WorldMap::height(){
    return _height;
}

int WorldMap::numSquareWidth(){
    return _numSqWidth;
}

int WorldMap::numSquareHeight(){
    return sqHeight;
}

int WorldMap::squareWidth(){
    return sqWidth;
}

int WorldMap::squareHeight(){
    return _numSqHeight;
}
int WorldMap::getRowFromY(double y){
    double row = y/sqHeight;
    return (int)row;
}

int WorldMap::getColFromX(double x){
    double col = x/sqWidth;
    return (int)col;
}

bool& WorldMap::fieldAt(double x, double y){
    return field(getColFromX(x), getRowFromY(y));
}
