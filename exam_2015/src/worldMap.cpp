#include "worldMap.hpp"
#include <cstring>
#include <cassert>
#include <stdio.h>

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

int WorldMap::squareWidth(){
    return _numSqWidth;
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

void WorldMap::print() {
    for (int i = 0; i < (this->_width * 2) + 2; i++)
        putchar('-');
    putchar('\n');

    for (int x = 0; x < this->_width; x++) {
        putchar('|');
        for (int y = 0; y < this->_height; y++) {
            if (fieldAt(x, y))
                printf("X|");
            else
                printf(" |");
        }
        printf("\n");
        for (int i = 0; i < (this->_width * 2) + 2; i++)
            putchar('-');
        printf("\n");
    }
}
