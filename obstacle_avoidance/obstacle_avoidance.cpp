#include <libplayerc++/playerc++.h>
#include <unistd.h>
#include "Sensor.h"
using namespace PlayerCc;

#define FRONT 2

PlayerClient robot("192.168.240.129");
Position2dProxy position(&robot);
IrProxy ir(&robot);

int main(int argc, char *argv[]) {
	//while (true) {
		//robot.Read();

		//for (int i = 0; i < ir.GetCount(); i++) {
			//printf("%d: %f   ", i, ir.GetRange(i));
		//}
		//printf("\n");

		//if (ir.GetRange(FRONT) < 0.5) {
			//position.SetSpeed(-0.1, 0.0);
			//sleep(2);
		//}

		//position.SetSpeed(0.1, 0.0);

	//}

	robot.Read();
	Sensor *front = new Sensor(&ir, FRONT, 2);
	while (true) {
		robot.Read();
		if (front->read() < 0.5) {
			position.SetSpeed(-0.3, 0.0);
		}

		position.SetSpeed(0.3, 0.0);
	}

	return 0;
}
