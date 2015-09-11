#include <libplayerc++/playerc++.h>
#include <unistd.h>
#include "Sensors.h"
#include <math.h>
#include <stdio.h>
using namespace PlayerCc;

#define FRONT_SPEED 0.5
#define FRONT_SPEED_MOD 0.6
#define TURN_SPEED 1.0
#define TURN_SPEED_MOD 1.6

void run(Position2dProxy *position, double sf, double sl, double sr) {
    double speed = FRONT_SPEED * (sf - FRONT_SPEED_MOD);
    double turn = (((sl - TURN_SPEED_MOD) * M_PI) +
           ((sr - TURN_SPEED_MOD) * (- M_PI)))*TURN_SPEED;
    position->SetSpeed(speed, turn);
}

PlayerClient robot("192.168.240.129");
Position2dProxy position(&robot);
IrProxy ir(&robot);

int main(int argc, char *argv[]) {
	Sensors sensors(&robot, 4);
	while(true){
        sensors.update();
        double front = sensors.read(IR_bn_n),
        double left = sensors.read(IR_bn_nw)
        double right = sensors.read(IR_bn_ne)

        print_f("Updating:\nFront: %f\nLeft: %f\nRight: %f",
                front, left, right);
        run(&position,
                front,
                left,
                right
        );
    }

	return 0;
}
