#include <libplayerc++/playerc++.h>
#include <unistd.h>
#include "Sensors.h"
#include <math.h>
#include <stdio.h>
#include <time.h>
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

#define FRONT_SPEED 0.15
#define FRONT_SPEED_MOD 0.4
#define TURN_SPEED 0.25
#define TURN_SPEED_MOD 1.6
#define SPEED_COUNTER 3
#define M_P_ODO 1.16

PlayerClient robot("192.168.240.129");
Position2dProxy position(&robot);
double xpos;
double ypos;
double yaw;

Sensors sensors(&robot, 2);

void getOdo(){
    sensors.update();
	ypos = position.GetYPos();
	xpos = position.GetXPos();
	yaw = position.GetYaw();
	printf("xpos: %f	 ypos = %f	 yaw = %f\n", xpos, ypos, yaw);

}


void wait(int secs) {
    time_t stop = time(NULL) + secs;

    while (time(NULL) < stop)
    {
        sensors.update();
	    getOdo(); 
    }
}


void goTo(double x, double y){
    
    position.ResetOdometry();   
    double turn;   
    double forward = sqrt(x*x+y*y)/M_P_ODO;
    
    if(x > .0)
    {
        turn = -(.5 * M_PI - atan(y/x));
        position.SetSpeed(0.0,-0.3);
    }
    else
    {
        turn = .5 * M_PI - atan(y/x);
        position.SetSpeed(0.0, 0.3);
    }

    
    printf("Turn: %f  Forward: %f\n", turn, forward);
    
    
    while(fabs(yaw) < fabs(turn))
        getOdo();      
 
    position.ResetOdometry();   
    position.SetSpeed(0.3,0.0);

    while(xpos < forward)
        getOdo();
     
    position.ResetOdometry();
    
    if(x > .0)
        position.SetSpeed(0.0,0.3);
    else
        position.SetSpeed(0.0,-0.3);
    
    printf("\n%f\n", fabs(yaw));

    while(fabs(yaw) < fabs(turn))
    {   
        printf("Turning Back\n");   
        getOdo();
    }
    
    position.SetSpeed(0.0,0.0);

}


void rotation(){
    position.SetSpeed(.0,-.3);

    while(yaw < .1)
        getOdo();

    position.SetSpeed(.0,.0);

    getOdo();
    position.SetOdometry(.0,.0,.0);
    getOdo(); 
}


void frontAvoid(Position2dProxy *position, double front) {
    position->SetSpeed(0, 0);
    wait(1);
    position->SetSpeed(-1*FRONT_SPEED, 0);
    wait(2);
    position->SetSpeed(0, 0);
    wait(1);
}

void run(Position2dProxy *position, double sf, double sl, double sr) {
    static int speedCount = SPEED_COUNTER;
    double speed = FRONT_SPEED * (sf - FRONT_SPEED_MOD);
    double turn = (((sl - TURN_SPEED_MOD) * M_PI) +
           ((sr - TURN_SPEED_MOD) * (- M_PI)))*TURN_SPEED;

    if((sf - FRONT_SPEED_MOD) <= 0.0 && --speedCount == 0){
        speedCount = SPEED_COUNTER;
        printf("\nCalling frontAvoid");
        frontAvoid(position, sf);
    }else{
        position->SetSpeed(speed, turn);
    }
}

int main(int argc, char *argv[]) {
    //PlayerClient robot("localhost");
    robot.SetDataMode(PLAYER_DATAMODE_PULL);
    robot.SetReplaceRule(true, PLAYER_MSGTYPE_DATA, -1);

    sensors.update();
    double front = sensors.read(IR_bn_n);
    double left = 1.6;
    double right = 1.6;
    getOdo();
    //goTo(1.0,2.0);
    rotation();
	
	

		

	return 0;
}
