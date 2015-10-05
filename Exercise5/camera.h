#ifndef CAMERA_H
#define CAMERA_H

#include "cxcore.h"
#include "highgui.h"

#include <string>

namespace object
{
  enum type
  {
    none = 0,
    horizontal,
    vertical
  };
  
  std::string name (type t);
}

typedef struct _cprop
{
  double red, green, blue;
} colour_prop;

class camera
{
  public:
    // Constructor / Destructor
    camera (const int idx = -1);
    ~camera ();
    
    // Accessor for the OpenCV camera handle
    CvCapture* get_capture ();
    
    // Image acquisition
    IplImage* get_colour ();
    IplImage* get_grey (IplImage *colour = NULL);
    
    // Object detection
    object::type get_object (IplImage *im, colour_prop &p, double &distance, double &angle);
    void draw_object (IplImage *im);

    // Low-level object detection
    CvPoint2D32f* get_corners (IplImage *im, bool &found, int &corner_count);
  
  private:
    // Object parameters
    const CvSize pattern_size;
    const double patternUnit; // Size of one checker square in mm
    
    // Camera handle
    CvCapture *cam;
    
    // Camera calibration parameters
    CvMat *intrinsic_matrix, *distortion_coeffs, *mapx, *mapy;
    
    // Buffers
    IplImage *grey, *colour, *mask;
    CvPoint2D32f *corners;
    
    // States for object detection
    bool found;
    int corner_count;
};

#endif // CAMERA_H
