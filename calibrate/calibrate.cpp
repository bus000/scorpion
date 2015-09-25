// This works with OpenCV 2 and above
#include <opencv2/core/core.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <string>
#include <stdlib.h>
#include <vector>
#include <iostream>

using namespace cv;


int main(int argc, char **argv) {
    int images = 8;

    if (argc < 2) {
      printf("No arguments given, using 8 images.\n");
    }
    else {
      images = atoi(argv[1]);
      printf("Using %d images.\n", images);
    }

    std::string winname = "Calibration image";
    namedWindow(winname);
    
    Mat cameraMatrix = Mat::eye(3, 3, CV_64F);;
    Mat distCoeffs = Mat::zeros(8, 1, CV_64F);
    std::vector<Mat > rvecs, tvecs;
    
    Mat img, gray;

    std::vector<Point2f> corners;
    std::vector<std::vector<Point2f> > corners_list;
    
    Size patternSize(4, 3); // Number of internal corner points in the calibration checkerboard pattern
    int patternUnit = 30; // measured in mm

    VideoCapture cap(0);
    
    for (int i = 0; i < images; i++) {
        while(true) {
          if(!cap.read(img)) {
            printf("Could not read frame from camera.\n");
            return 2;
          }
          
          imshow(winname, img);

          int key = cvWaitKey(1); // Wait forever for a key press
          
          if (key != -1)
            break;
        }


        cvtColor(img, gray, CV_BGR2GRAY); // Convert to gray scale

        bool patternFound = findChessboardCorners(img, patternSize, corners);
        if (patternFound){
          printf("Pattern fround; image no. %d\n%d", i+1, images); 
          cornerSubPix(gray, corners, Size(5,5), Size(-1,-1), TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1));
        }
        else {
          printf("No pattern; image no. %d/%d\n", i+1, images); 
          i--;
          continue;
        }
        
        drawChessboardCorners(img, patternSize, corners, patternFound);
        
        imshow(winname, img);
        
        corners_list.push_back(corners);
        
        cvWaitKey(500); 
    }
    
    // Create a list of checkerboard corner points in object 3D coordinates
    std::vector<Point3f> patternPoints;
    
    for (int i = 0; i < patternSize.height; i++) {
        for (int j = 0; j < patternSize.width; j++) {
            Point3f pPoint(j * patternUnit, i * patternUnit, 0.0f);
            patternPoints.push_back(pPoint);
        }
    }
    
    std::vector<std::vector<Point3f> > patterns_list;
    
    // Populate the patterns list by duplicating as many times as we have images
    for (int i = 0; i < corners_list.size(); i++)
        patterns_list.push_back(patternPoints);
    
    // Calibrate camera instrinsic parameters based on analysed images
    Size imgSize(img.cols, img.rows);
    double rms = calibrateCamera(patterns_list, corners_list, imgSize, cameraMatrix, distCoeffs, rvecs, tvecs);
    
    std::cout << "Calibration reprojection error = " << rms << std::endl;
    
    // Save results in YAML format
    FileStorage fs( "calibration.yml", FileStorage::WRITE );
    
    fs << "reprojection_error" << rms;
    fs << "camera_matrix" << cameraMatrix;
    fs << "distortion_coefficients" << distCoeffs;

    printf("camera fx: %f\n", cameraMatrix.at<double>(0,0));
    printf("camera fy: %f\n", cameraMatrix.at<double>(1,1));
    
    return 0;
}
