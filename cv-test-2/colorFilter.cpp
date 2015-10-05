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
    printf("%f\n", mu);

    for(int row = 0; row < nRows; row++){
        uchar *rowPtr = src.ptr<uchar>(row);
        uchar *destRowPtr = dest.ptr<uchar>(row);
        for(int col = 0; col < nCols; col += 3){
            uchar hue = rowPtr[col];
            uchar sat = rowPtr[col+1];

            uchar model = round((S_MAX*pow(0.5*(1+cos(((hue-mu)*(M_PI/(H_MAX/2))))), k)));
            if(rowPtr[col+1] > minSat)
                destRowPtr[col+2] = model;
            else
                destRowPtr[col+2] = 0;
            destRowPtr[col+1] = 0;
            destRowPtr[col] = 0;
        }
    }
}
