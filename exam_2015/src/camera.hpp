#ifndef _CAMERA_
#define _CAMERA_

#include <vector>
#include <string>
#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

// Chessboard x by y
#define PATTERN_SIZE Size(3, 4)

// Cell size in mm
#define PATTERN_UNIT 50.0

#define CAMERA_WINDOW "Camera Window"

using namespace cv;
using namespace std;

class Camera {
public:
  Camera(double fx, double fy, double cx);
  ~Camera();

  // TODO: return particle
  void measure(bool showGui = true);

private:
  void enhanceContrast(Mat &frame, Mat &target);

  // Camera parameters
  double fx;
  double fy;
  double cx;

  // OpenCV webcam video capture
  VideoCapture *videoCapture;
  
};


#endif
