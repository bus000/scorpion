#ifndef COLOR_FILTER
#define COLOR_FILTER
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#define H_MAX 180
#define V_MAX 255
#define S_MAX 255

using namespace cv;

void ColorFilter(Mat src, Mat &dest, double my, double k);

void ToBin(Mat src, Mat &dest, int filter);

#endif
