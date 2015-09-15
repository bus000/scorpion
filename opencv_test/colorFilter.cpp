#include "colorFilter.hpp"
#include <cmath>
#include <stdio.h>

void ColorFilter(Mat src, Mat &dest, double mu, double k, double minSat) {
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

            uchar model = round((255*pow(0.5*(1+cos((hue*(M_PI/(180/2)))-mu)), k)));
            if(rowPtr[col+1] > minSat)
                destRowPtr[col+2] = model;
            else
                destRowPtr[col+2] = 0;
            destRowPtr[col+1] = 0;
            destRowPtr[col] = 0;
        }
    }
}
