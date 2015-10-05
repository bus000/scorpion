#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;

int main() {
    int blockSize=100;
    int imageWidth=blockSize*4;
    int imageHeight=blockSize*5;
    Mat chessBoard(imageHeight,imageWidth,CV_8UC3,Scalar::all(0));
    unsigned char color=0;
    
    for(int i=0;i<imageHeight;i=i+blockSize){
        color=~color;
        for(int j=0;j<imageWidth;j=j+blockSize){
            Mat ROI=chessBoard(Rect(j,i,blockSize,blockSize));
            ROI.setTo(Scalar::all(color));
            color=~color;
        }
    }
    
    imwrite("checkerboard_pattern.png", chessBoard);
    
    
    return 0;
}
