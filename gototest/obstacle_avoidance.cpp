#include <libplayerc++/playerc++.h>
#include <unistd.h>
#include "Sensors.h"
#include <math.h>
#include <stdio.h>
#include <time.h>
using namespace PlayerCc;

PlayerClient robot("192.168.240.129");
Position2dProxy position(&robot);

Sensors sensors(&robot, 2);

void wait(int secs) {
    time_t stop = time(NULL) + secs;

    while (time(NULL) < stop)
        sensors.update();
}

void frontAvoid(Position2dProxy *position, double front) {
    position->SetSpeed(0, 0);
    wait(1);
    position->SetSpeed(-1*FRONT_SPEED, 0);
    wait(1);
    position->SetSpeed(0, M_PI/2);
    wait(1);
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
	player_pose_t destination = (3.0,2.0,1.0);
    player_pose_t vel = (0.15,0,0);
    position.GoTo(destination,vel);

	return 0;
}
