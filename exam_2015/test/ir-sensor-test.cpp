#include "../src/irSensors.hpp"
#include "../src/particles.hpp"

using namespace PlayerCc;

int main(int argc, char const *argv[])
{
    PlayerClient robot("192.168.240.129");
    Position2dProxy position(&robot);
    IRSensors irSensors(&robot);

    for(int i = 0; ; i++) {
        vector<particle> res = irSensors.getObstacles();
        if (res.size() > 0) {
            particle part = res.at(0);
            printf("(%f, %f) len: %f\n", part.x, part.y, sqrt(part.x * part.x +
                        part.y * part.y));
        }
    }

    return EXIT_SUCCESS;
}
