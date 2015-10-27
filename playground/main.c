#include <stdio.h>
#include "math.h"

void fromTo(double theta, double xPos, double yPos, double xTo, double yTo) {
  printf("fromTo(%f, %f, %f, %f, %f)\n", theta, xPos, yPos, xTo, yTo);
  double xDiff = xTo - xPos;
  double yDiff = yTo - xPos;

  double thetaDiff = atan2(yDiff, xDiff);

  double cs = cos(theta);
  double sn = sin(theta);

  double xRot = xDiff * cs - yDiff * sn;
  double yRot = xDiff * sn + yDiff * cs;

  double finTheta = 0.5 * M_PI - atan2(yRot, xRot);
  if (finTheta < 0) finTheta += 2*M_PI;

  if (finTheta > M_PI){
   finTheta = 2*M_PI - finTheta;
   printf("Turning left %f degrees\n", finTheta * 57.2958);
  } 
  else {
   printf("Turning right %f degrees\n", finTheta * 57.2958);
  }

  double dist = sqrt(pow(xRot, 2.0) + pow(yRot, 2.0));

  printf("Driving %f m\n", dist);
}

int main() {
  fromTo(0, 5, 5, 


   return 0;
}
