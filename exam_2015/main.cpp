#include "IR_Sensors.hpp"
#include "particles.hpp"

using namespace PlayerCc;

int main(int argc, char const *argv[])
{
    PlayerClient robot("192.168.240.129");
    Position2dProxy position(&robot);
    IRSensors irSensors(&robot);

    while (true) {
        vector<particle> res = irSensors.getObstacles();
        if (res.size() > 0) {
            particle part = res.at(0);
            printf("(%f, %f)\n", part.x, part.y);
        }
    }

    return EXIT_SUCCESS;
}
