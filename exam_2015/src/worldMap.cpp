#include "worldMap.hpp"
#include <cstring>
#include <cassert>
#include <iostream>

WorldMap::WorldMap(int numSqWidth, int numSqHeight, int sqSize){
    _width = numSqWidth*sqSize;
    _height = numSqHeight*sqSize;
    _numSqWidth = numSqWidth;
    _numSqHeight = numSqHeight;

    _sqSize = sqSize;

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

    return map[(col*_numSqHeight)+row];
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
    return _numSqHeight;
}

int WorldMap::squareSize(){
    return _sqSize;
}

int WorldMap::getRowFromY(double y){
    int row = y/_sqSize;
    return row;
}

int WorldMap::getColFromX(double x){
    int col = x/_sqSize;
    return col;
}

bool& WorldMap::fieldAt(double x, double y){
    return field(getColFromX(x), getRowFromY(y));
}
