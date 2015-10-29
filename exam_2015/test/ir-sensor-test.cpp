#include "../src/irSensors.hpp"
#include "../src/particle.hpp"
#include "../src/driveCtl.hpp"

using namespace PlayerCc;

int main(int argc, char const *argv[])
{
    PlayerClient robot("192.168.240.129");
    Position2dProxy position(&robot);
    IRSensors irSensors(&robot);
    DriveCtl driveController(&robot, &position);


    vector<Particle> res = irSensors.getObstacles();
    printf("number of sensors sensing something is %d\n", res.size());

    for (int i = 0; i < res.size(); i++) {
        Particle par = res.at(i);
        printf("object spotted at (%f, %f), len: %f\n", par.x(), par.y(),
                sqrt(par.x() * par.x() + par.y() * par.y()));}


    cout << "Press enter to continue" << endl;

    cin.ignore();

    driveController.turn(360.0);

    return EXIT_SUCCESS;
}
