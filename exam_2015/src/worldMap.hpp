#ifndef WORLD_MAP_H
#define WORLD_MAP_H
#include <vector>

class WorldMap {
public:
    WorldMap(double width, double height, int numSqWidth, int numSqHeight);
    ~WorldMap();
    void field(int col, int row, bool mark);
    bool& field(int col, int row);
    //[col][row]
    bool* operator[] (int col);
    int getRowFromY(double y);
    int getColFromX(double x);
    bool& fieldAt(double x, double y);
    void clear();

    int squareWidth();
    int squareHeight();
    double width();
    double height();

private:
    bool *map;

    double sqWidth;
    double sqHeight;

    double _width;
    double _height;
    int _numSqWidth;
    int _numSqHeight;
};

#endif
