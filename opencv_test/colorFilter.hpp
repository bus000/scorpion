#ifndef COLOR_FILTER
#define COLOR_FILTER
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

void ColorFilter(Mat src, Mat &dest, double my, double k);

#endif
