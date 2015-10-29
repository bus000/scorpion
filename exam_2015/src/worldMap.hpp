#ifndef WORLD_MAP_H
#define WORLD_MAP_H
#include <vector>

class WorldMap {
public:
    WorldMap(int numSqWidth, int numSqHeight, int sqSize);
    ~WorldMap();
    void field(int col, int row, bool mark);
    bool& field(int col, int row);
    //[col][row]
    bool* operator[] (int col);
    int getRowFromY(double y);
    int getColFromX(double x);
    bool& fieldAt(double x, double y);
    void clear();

    int squareSize();
    int numSquareWidth();
    int numSquareHeight();
    double width();
    double height();

private:
    bool *map;

    int _sqSize;
    int _width;
    int _height;
    int _numSqWidth;
    int _numSqHeight;
};

#endif
