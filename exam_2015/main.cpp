#include "IR_Sensors.hpp"

int main(int argc, char const *argv[])
{
    PlayerClient robot("192.168.240.129");
    Position2dProxy position(&robot);

    IR_Sensors irSensors(&robot);

    while (true) {
        particle res = irSensors.getObstacles();
        printf("(%d, %d)\n", res.xPos, res.yPos);
    }

    return EXIT_SUCCESS;
}
