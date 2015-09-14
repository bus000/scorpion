#include <libplayerc++/playerc++.h>
#include <unistd.h>
#include "IrSensor.h"
#include "SensorGroup.h"
#include <math.h>
#include <stdio.h>
using namespace PlayerCc;

#define IR_bn_ene 0
#define IR_bn_wnw 1
#define IR_bn_n 2
#define IR_bn_ne 3
#define IR_bn_nw 4
#define IR_te_nnw 5
#define IR_te_nne 6
#define IR_tw_nnw 7
#define IR_tw_nne 8
#define IR_bs_w 9
#define IR_bs_e 10
#define IR_bw_s 11
#define IR_be_s 12

#define FRONT_SPEED 0.3
#define FRONT_SPEED_MOD 0.6
#define TURN_SPEED 0.25
#define TURN_SPEED_MOD 1.6
#define NUM_SAMPLES 2

void run(Position2dProxy *position, double sf, double sl, double sr) {
    double speed = FRONT_SPEED * (sf - FRONT_SPEED_MOD);
    double turn = (((sl - TURN_SPEED_MOD) * M_PI) +
           ((sr - TURN_SPEED_MOD) * (- M_PI)))*TURN_SPEED;
    position->SetSpeed(speed, turn);
}

PlayerClient robot("192.168.240.129");
Position2dProxy position(&robot);

int main(int argc, char *argv[]) {
  IrSensor northIr(&robot, IR_bn_n);
  IrSensor northWestIr(&robot, IR_bn_nw);
  IrSensor northEastIr(&robot, IR_bn_ne);
  IrSensor westNorthWestIr(&robot, IR_bn_wnw);
  IrSensor eastNorthEastIr(&robot, IR_bn_ene);

  SensorGroup frontIr; 
  frontIr.addSensor(&northIr,     0.9);
  frontIr.addSensor(&northWestIr, 0.05);
  frontIr.addSensor(&northEastIr, 0.05);

  SensorGroup leftIr;
  leftIr.addSensor(&northWestIr,     0.65);
  leftIr.addSensor(&westNorthWestIr, 0.35);

  SensorGroup rightIr;
  rightIr.addSensor(&northEastIr,     0.65);
  rightIr.addSensor(&eastNorthEastIr, 0.35);

	while(true){
        frontIr.startMeasure();
        leftIr.startMeasure();
        rightIr.startMeasure();

        for(int i = 0; i < NUM_SAMPLES; i++) {
         robot.Read();
         frontIr.measure(); 
         leftIr.measure();
         rightIr.measure();
        }

        double front = frontIr.endMeasure();
        double left  = leftIr.endMeasure(); 
        double right = rightIr.endMeasure(); 

        printf("Updating:\nFront: %f\nLeft: %f\nRight: %f",
                front, left, right);
        run(&position,
                front,
                left,
                right
        );
    }

	return 0;
}
