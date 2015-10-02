#include "colorFilter.hpp"
#include <cmath>
#include <stdio.h>
#include <iostream>

void ColorFilter(Mat src, Mat &dest, double mu, double k) {
    int nRows = src.rows;
    int nCols = src.cols*src.channels();
    dest.create(src.rows, src.cols, src.type());
    
    if(src.isContinuous()){
        nCols = nCols*nRows;
        nRows = 1;
    }

    for(int row = 0; row < nRows; row++){
        uchar *rowPtr = src.ptr<uchar>(row);
        uchar *destRowPtr = dest.ptr<uchar>(row);
        for(int col = 0; col < nCols; col += 3){
            uchar hue = rowPtr[col];
            uchar sat = rowPtr[col+1];

            uchar model = round((sat*pow(0.5*(1+cos((hue*(M_PI/(H_MAX/2)))-mu)), k)));
            destRowPtr[col+2] = model;
            destRowPtr[col+1] = 0;
            destRowPtr[col] = 0;
        }
    }
}

void ToBin(Mat src, Mat &dest, int filter) {
    Mat tmpDest;
    int x, y;
    x = round(src.size().width/6);
    y = round(src.size().height/6);

    if(x % 2 == 0)
        x++;
    if(y % 2 == 0)
        y++;

    std::cout << "x: " << x << std::endl << "y: " << y << std::endl;

    cv::GaussianBlur(src, tmpDest, cv::Size (x, y), 0, 0);
    cv::inRange(tmpDest, cv::Scalar(0,0, round(V_MAX/filter)), cv::Scalar(H_MAX, S_MAX, V_MAX), dest);
}
