#include "camera.hpp"

Camera::Camera(double fx, double fy, double cx) {
    VideoCapture *vcap = new VideoCapture(CV_CAP_ANY);

    this->videoCapture = vcap;
    this->fx = fx;
    this->fy = fy;
    this->cx = cx;
    
    if (!vcap->isOpened()) {
      cerr << "Failed to open camera" << endl;
    }

    
    namedWindow(CAMERA_WINDOW, CV_WINDOW_NORMAL);
}

Camera::~Camera() {

}

Measurement Camera::measure(bool showGui) {
    cvWaitKey(30);
    VideoCapture *vcap = this->videoCapture;
    vcap->set(CV_CAP_PROP_FRAME_WIDTH, 1280);
    vcap->set(CV_CAP_PROP_FRAME_HEIGHT, 720);
    Mat frame;

    // Read frame from webcam
    vcap->read(frame);

    // enhanceContrast(frame, frame);

    // Find checkerboard
    Mat gray;
    cvtColor(frame, gray, CV_BGR2GRAY);
    
    vector<Point2f> corners;

    bool foundChessboard =
      findChessboardCorners( gray
                           , PATTERN_SIZE
                           , corners
                           , CALIB_CB_ADAPTIVE_THRESH
                           + CALIB_CB_NORMALIZE_IMAGE
                           + CALIB_CB_FAST_CHECK
                           );

    if (!foundChessboard) {
        if (showGui)
            imshow(CAMERA_WINDOW, frame);

        #ifdef DEBUG_CAMERA
        cout << "[Camera] No landmark found." << endl;
        #endif

        return Measurement();
    } 

    // Increase position accuracy
    cornerSubPix( gray
                , corners
                , Size(11, 11)
                , Size(-1, -1)
                , TermCriteria( CV_TERMCRIT_EPS
                              + CV_TERMCRIT_ITER
                              , 30
                              , 0.1
                              )
                );

    // Determine orientation
    double deltaX = abs(corners[0].x - corners[2].x);
    double deltaY = abs(corners[0].y - corners[2].y);

    bool horizontal = deltaY > deltaX;
   
    // Calculate the pixel pixelHeight and actual height
    // of the pattern. 
    double pixelHeight;
    double actualHeight;

    if (horizontal) {
        double diff = 
            fabs (corners [0].y - corners [2].y) +
            fabs (corners [9].y - corners [11].y);

        pixelHeight = diff / 2.0;
        actualHeight =
            (PATTERN_SIZE.width-1.0f) * PATTERN_UNIT;
    }
    else {
        double diff = 
            fabs (corners [0].y - corners [9].y) +
            fabs (corners [2].y - corners [11].y);

        pixelHeight = diff / 2.0;
        actualHeight =
            (PATTERN_SIZE.height-1.0f) * PATTERN_UNIT;
    }


    // Calculate the distance and angle.
    double distance = this->fy * (actualHeight / pixelHeight);

    double patternCenterX =
        ( corners [0].x + corners [2].x 
        + corners [9].x + corners [11].x) / 4.0;

    double angle =
        -atan2(patternCenterX - this->cx, this->fx);
    
    // Determine color
    Point poly[] = {
        Point(corners[0].x,  corners[0].y),
        Point(corners[2].x,  corners[2].y),
        Point(corners[9].x,  corners[9].y),
        Point(corners[11].x, corners[11].y),
    };

    Mat mask = Mat::zeros(frame.rows, frame.cols, CV_8U);

    fillConvexPoly(mask, poly, 4, Scalar(255, 255, 255));

    Scalar meanColor = mean(frame, mask);
    
    double red   = meanColor.val [2];
    double green = meanColor.val [1];
    double blue  = meanColor.val [0];
    double sum   = red + green + blue;

    red   = red / sum;
    green = green / sum;
    blue  = blue / sum;
    
    // Lookup landmark position
    Particle position;
      
    bool isRed = red > green;

    if (horizontal) {
      if (isRed)  position = Particle(300, 400);  // L4
      if (!isRed) position = Particle(0, 400);    // L3
    }
    else {
      if (isRed)  position = Particle(0, 0);      // L1
      if (!isRed) position = Particle(300, 0);    // L2
    }

    // Calculate distance and angle particle (vector)
    Particle measurement = Particle::createUnit(angle);
    measurement.scale(distance);

    // Possibly show GUI 
    if (showGui) {
        drawChessboardCorners( frame
                             , PATTERN_SIZE
                             , corners
                             , foundChessboard 
                             );

        imshow(CAMERA_WINDOW, frame);
    }

    #ifdef DEBUG_CAMERA
      cout << "[Camera] Found landmark:" << endl
           << "\tHorizontal:\t" << horizontal << endl
           << "\tPixel height:\t" << pixelHeight << endl
           << "\tActual height:\t" << actualHeight << endl
           << "\tColor:\t\t(" << red << ", "
                            << green << ", " 
                            << blue << ")" << endl
           << "\tDistance:\t" << distance << " cm" << endl
           << "\tAngle:\t\t" << angle << " rads ("
               << angle * (180 / M_PI) << " degrees)"
           << endl << endl;
    #endif

    return Measurement(position, measurement);
}

void Camera::enhanceContrast(Mat &frame, Mat &target) {
    vector<Mat> labChannels;

    Mat labFrame;
    cvtColor(frame, labFrame, CV_BGR2Lab);
    split(labFrame, labChannels);

    Mat luminance = labChannels[0];

    Mat enhanced;
    equalizeHist(luminance, enhanced);

    labChannels[0] = enhanced;
    merge(labChannels, target);
    cvtColor(target, target, CV_Lab2BGR);
}
