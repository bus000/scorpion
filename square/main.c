#include <stdio.h>
#include <libplayerc/playerc.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>

const double velocity = 0.2; // m/s
const double acceleration = 20; // cm/s^2
const double angular_velocity = 0.5; // radian/s
const double angular_acceleration = M_PI/2; //radian/s^2

void rotate(playerc_position2d_t *position, double degrees, double speed) {
	//double sec = abs(degrees * M_PI / 180 / speed);
	//double sec = degrees * 0.0174532925 / speed;
	double sec = degrees * (M_PI/180) / speed;

	playerc_position2d_set_cmd_vel(position, 0, 0, speed, 0);
	usleep((int) (sec * 1000000));
	playerc_position2d_set_cmd_vel(position, 0, 0, 0, 0);
}

void move(playerc_position2d_t *position, double distance, double speed) {
	double sec = distance / speed;

	playerc_position2d_set_cmd_vel(position, speed, 0, 0, 1);
	printf("Before sleep %f\n", sec);
	usleep((int) (sec * 1000000));
	printf("After sleep\n");
	playerc_position2d_set_cmd_vel(position, 0, 0, 0, 0);
}

int main(int argc, char **argv) {
	playerc_client_t *client;
	client = playerc_client_create(NULL, "192.168.240.129", 6665);
	playerc_position2d_t *position2d;
	int i;

	if(playerc_client_connect(client)){
		printf("Failed to create client\n");
		return -1;
	}

	position2d = playerc_position2d_create(client, 0);
	if(playerc_position2d_subscribe(position2d, PLAYERC_OPEN_MODE)){
		printf("Failed to create proxy\n");
		return -1;
	}

	playerc_position2d_enable(position2d, 1);
	for (i = 0; i < 4; i++) {
		move(position2d, 1, velocity);
		usleep(1000000);
		rotate(position2d, 90, angular_velocity);
		usleep(1000000);
	}

	playerc_position2d_unsubscribe(position2d);
	playerc_position2d_destroy(position2d);
	playerc_client_disconnect(client);
	playerc_client_destroy(client);

	return 0;
}
