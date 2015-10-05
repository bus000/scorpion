#include "camera.h"
#include "cv.h"

#if CV_MAJOR_VERSION == 3 || CV_MAJOR_VERSION == 2
#include <opencv2/calib3d.hpp> // This works with OpenCV 2 and above
#endif

#include <iostream>

std::string object::name (object::type t)
{
  const std::string name_list [] = {
    "None",
    "Horizontal",
    "Vertical"
  };
  
  return name_list [t];
}

// Constructor / Destructor
camera::camera (const int idx)
  : pattern_size (cvSize (3, 4)), patternUnit(50.0f)
{
  // Allocate buffers
  corners = new CvPoint2D32f [pattern_size.width * pattern_size.height];

  const CvSize imsize = cvSize (640, 480);
  grey = cvCreateImage (imsize, IPL_DEPTH_8U, 1);
  colour = cvCreateImage (imsize, IPL_DEPTH_8U, 3);
  mask = cvCreateImage (imsize, IPL_DEPTH_8U, 1);
  
  // Get camera
  cam = cvCreateCameraCapture (idx);
  cvSetCaptureProperty (cam, CV_CAP_PROP_FRAME_WIDTH, imsize.width);
  cvSetCaptureProperty (cam, CV_CAP_PROP_FRAME_HEIGHT, imsize.height);
  
  // Camera parameters from the caibration of the Scorpion camera
  intrinsic_matrix = cvCreateMat(3, 3, CV_64FC1);
  CV_MAT_ELEM(*intrinsic_matrix, double, 0, 1) = 0;
  CV_MAT_ELEM(*intrinsic_matrix, double, 1, 0) = 0;
  CV_MAT_ELEM(*intrinsic_matrix, double, 2, 0) = 0;
  CV_MAT_ELEM(*intrinsic_matrix, double, 2, 1) = 0;
  CV_MAT_ELEM(*intrinsic_matrix, double, 2, 2) = 1;
  CV_MAT_ELEM(*intrinsic_matrix, double, 0, 0) = 476.36883;
  CV_MAT_ELEM(*intrinsic_matrix, double, 0, 2) = 239.23852;
  CV_MAT_ELEM(*intrinsic_matrix, double, 1, 1) = 482.93459;
  CV_MAT_ELEM(*intrinsic_matrix, double, 1, 2) = 245.90425;

  distortion_coeffs = cvCreateMat(1, 4, CV_64FC1);
  CV_MAT_ELEM(*distortion_coeffs, double, 0, 0) = -0.29624;
  CV_MAT_ELEM(*distortion_coeffs, double, 0, 1) =  0.13749;
  CV_MAT_ELEM(*distortion_coeffs, double, 0, 2) = -0.01171;
  CV_MAT_ELEM(*distortion_coeffs, double, 0, 3) =  0.01984;
  
  // Initialise undistortion map
  mapx = cvCreateMat (imsize.height, imsize.width, CV_32FC1);
  mapy = cvCreateMat (imsize.height, imsize.width, CV_32FC1);
  cvInitUndistortMap (intrinsic_matrix, distortion_coeffs, mapx, mapy);
}

camera::~camera ()
{
  // Deallocate buffers
  delete [] corners;
  cvReleaseImage (&grey);
  cvReleaseImage (&colour);
  cvReleaseImage (&mask);
  
  // Release camera
  cvReleaseCapture (&cam);
  
  // Release undistortion map
  cvReleaseMat (&mapx);
  cvReleaseMat (&mapy);
}

// Accessor for the OpenCV camera handle
CvCapture* camera::get_capture ()
{
  return cam;
}
    
// Image acquisition
IplImage* camera::get_colour ()
{
  IplImage *distorted = cvQueryFrame (cam);
  cvRemap (distorted, colour, mapx, mapy);
    
    
  //  colour = cvQueryFrame (cam);

  return colour;
}

IplImage* camera::get_grey (IplImage *col)
{
  if (col == NULL)
    col = get_colour ();
  cvCvtColor (col, grey, CV_BGR2GRAY);
  
  return grey;
}

inline CvPoint round_cvPoint (const CvPoint2D32f p)
{
  return (cvPoint ((int)p.x, (int)p.y));
}

// Object detection
object::type camera::get_object (IplImage *im, colour_prop &p, double &distance, double &angle)
{
  object::type retval = object::none;

  // Detect corners
  get_corners (im, found, corner_count);
  
  if (found)
    {
      // Determine if the object is horizontal or vertical
      const int delta_x = abs (corners [0].x - corners [2].x);
      const int delta_y = abs (corners [0].y - corners [2].y);
      const bool horizontal = (delta_y > delta_x);
      if (horizontal)
        retval = object::horizontal;
      else
        retval = object::vertical;
          
      // Compute distances and angles
      double height, patternHeight;
        if (horizontal) {
            height = ((fabs (corners [0].y - corners [2].y) +
                       fabs (corners [9].y - corners [11].y)) / 2.0);
            patternHeight = (pattern_size.width-1.0f) * patternUnit;
        } else {
            height = (fabs (corners [0].y - corners [9].y) +
                      fabs (corners [2].y - corners [11].y)) / 2.0;
            patternHeight = (pattern_size.height-1.0f) * patternUnit;
        }
      //distance = 1.0 / (0.0001 * height);
      distance = CV_MAT_ELEM(*intrinsic_matrix, double, 1, 1) * patternHeight / (height*10.0f); // in cm
      
      const double center = (corners [0].x + corners [2].x +
                             corners [9].x + corners [11].x) / 4.0;
        
      //angle = 0.0018 * center - 0.4425;
      //angle = 0.0018 * center - 0.6425;
      //angle *= -1.0;
        
      angle = -atan2(center - CV_MAT_ELEM(*intrinsic_matrix, double, 0, 2), CV_MAT_ELEM(*intrinsic_matrix, double, 0, 0));
        
  
      // Classify object by colour
      if (im->nChannels == 3)
        {
          // Extract rectangle corners
          CvPoint points [] = {
            round_cvPoint (corners [0]),
            round_cvPoint (corners [2]),
            round_cvPoint (corners [9]),
            round_cvPoint (corners [11])
          };
          
          // Compute region of interest
          cvZero (mask);
          cvFillConvexPoly (mask, points, 4, CV_RGB (255, 255, 255));

          // Compute mean colour inside region of interest
          CvScalar mean_colour = cvAvg (im, mask);
          const double red   = mean_colour.val [2];
          const double green = mean_colour.val [1];
          const double blue  = mean_colour.val [0];
          const double sum = red + green + blue;
          
          p.red = red / sum;
          p.green = green / sum;
          p.blue = blue / sum;
        }
      else
        retval = object::none;
    }
  
  return retval;
}

void camera::draw_object (IplImage *im)
{
  cvDrawChessboardCorners (im, pattern_size, corners, corner_count, found);  
}

// Low-level object detection
CvPoint2D32f* camera::get_corners (IplImage *im, bool &found, int &corner_count)
{
  if (im->nChannels == 3)
    {
      cvCvtColor (im, grey, CV_BGR2GRAY);
      im = grey;
    }
  
  found = cvFindChessboardCorners (im, pattern_size, corners, &corner_count,
                    CV_CALIB_CB_NORMALIZE_IMAGE | CV_CALIB_CB_ADAPTIVE_THRESH);
  if (found)
    cvFindCornerSubPix (im, corners, corner_count, cvSize (5, 5), cvSize (-1, -1),
                        cvTermCriteria (CV_TERMCRIT_ITER, 3, 0.0));

  return corners;
}

