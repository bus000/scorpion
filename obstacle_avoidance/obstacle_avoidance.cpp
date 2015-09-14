#include <libplayerc++/playerc++.h>
#include <unistd.h>
#include "Sensors.h"
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

void run(Position2dProxy *position, double sf, double sl, double sr) {
    double speed = FRONT_SPEED * (sf - FRONT_SPEED_MOD);
    double turn = (((sl - TURN_SPEED_MOD) * M_PI) +
           ((sr - TURN_SPEED_MOD) * (- M_PI)))*TURN_SPEED;
    position->SetSpeed(speed, turn);
}

bool frontAvoid(Position2dProxy *position, double front) {
    if((front-0.05) <= FRONT_SPEED_MOD) {
        position->SetSpeed(-1*FRONT_SPEED/2, 0);
        sleep(1);
        position->SetSpeed(0, M_PI/2);
        sleep(1);
        position->SetSpeed(0, 0);
        return true;
    }

    return false;
}

int main(int argc, char *argv[]) {
    //PlayerClient robot("localhost");
    PlayerClient robot("192.168.240.129");
    Position2dProxy position(&robot);

	Sensors sensors(&robot, 1);
	while(true){
        sensors.update();
        double front = sensors.read(IR_bn_n);
        double left = sensors.read(IR_bn_wnw);
        double right = sensors.read(IR_bn_ene);

        printf("Updating:\nFront: %f\nLeft: %f\nRight: %f",
                front, left, right);

        if(frontAvoid(&position, front))
            continue;

        run(&position,
                front,
                left,
                right
        );
    }

	return 0;
}
