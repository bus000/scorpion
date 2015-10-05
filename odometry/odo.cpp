#include <libplayerc++/playerc++.h>
#include <unistd.h>
#include "Sensors.h"
#include <math.h>
#include <stdio.h>
#include <time.h>
using namespace PlayerCc;

#define TURN_SPEED (0.3)
//#define MAGIC (1.16)
#define MAGIC (1.0)

PlayerClient robot("192.168.240.129");
Position2dProxy position(&robot);
double xpos;
double ypos;
double yaw;

Sensors sensors(&robot, 2);

void getOdo()
{
    sensors.update();
    ypos = position.GetYPos();
    xpos = position.GetXPos();
    yaw = position.GetYaw();
    printf("xpos: %f     ypos = %f   yaw = %f\n", xpos, ypos, yaw);
}

bool double_cmp(double a, double b, double epsilon)
{
    return fabs(a - b) < epsilon;
}

bool transpose(double x, double y)
{
    double start_yaw, end_yaw, x_pos, y_pos, turn;
    double forward = sqrt(x*x + y*y) / MAGIC;

    if (y < 0) return false;

    getOdo();
    start_yaw = yaw;

    if (x > 0) {
        turn = -((0.5 * M_PI) - (y / x));
    } else if (x < 0) {
        turn = (0.5 * M_PI) - (y / -x);
    } else {
        turn = 0;
    }

    if (start_yaw + turn > M_PI) {
        end_yaw = -M_PI + (turn - (M_PI - start_yaw));
    } else if (start_yaw + turn < -M_PI) {
        end_yaw = M_PI + (turn + (M_PI + start_yaw));
    } else {
        end_yaw = start_yaw + turn;
    }

    /* Determine When to Step turning */
    if (x > 0)
        position.SetSpeed(0.0, -TURN_SPEED);
    else
        position.SetSpeed(0.0, TURN_SPEED);

    while (!double_cmp(yaw, end_yaw, 0.01))
        getOdo();

    position.ResetOdometry();
    position.SetSpeed(0.0, 0.0);
    getOdo();
    sleep(1);
    position.SetSpeed(0.1, 0.0);

    /* Forward. */
    double x_fabs = fabs(x) / MAGIC;
    double y_fabs = fabs(y) / MAGIC;
    if (end_yaw >= 0 && end_yaw <= (0.5 * M_PI)) {
        while (xpos < x_fabs && ypos < y_fabs)
            getOdo();
    } else if (end_yaw > 0) {
        while (xpos > x_fabs && ypos < y_fabs)
            getOdo();
    } else if (end_yaw < 0 && end_yaw >= -(0.5 * M_PI)) {
        while (xpos < x_fabs && ypos > y_fabs)
            getOdo();
    } else {
        while (xpos > x_fabs && ypos > y_fabs)
            getOdo();
    }

    position.SetSpeed(0.0, 0.0);
}

int main(int argc, char *argv[]) {
    /* Set mode to push so TCP connection doesn't flood. */
    robot.SetDataMode(PLAYER_DATAMODE_PULL);
    robot.SetReplaceRule(true, PLAYER_MSGTYPE_DATA, -1);

    transpose(-1, 1);

    return 0;
}
