#include "../src/irSensors.hpp"
#include "../src/particle.hpp"
#include "../src/driveCtl.hpp"

using namespace PlayerCc;

int main(int argc, char const *argv[])
{
    PlayerClient robot("192.168.240.129");
    Position2dProxy position(&robot);
    robot.SetDataMode(PLAYER_DATAMODE_PULL);
    robot.SetReplaceRule(true, PLAYER_MSGTYPE_DATA, -1);
    IRSensors irSensors(&robot);
    DriveCtl driveController(&robot, &position);

    //while (true) {
        //vector<Particle> res = irSensors.getObstacles();
        //printf("number of sensors sensing something is %d\n", res.size());

        //for (int i = 0; i < res.size(); i++) {
            //Particle par = res.at(i);
            //printf("object spotted at (%f, %f), len: %f\n", par.x(), par.y(),
                    //sqrt(par.x() * par.x() + par.y() * par.y()));
        //}

        //cout << "Press enter to continue" << endl;
        //cin.ignore();
    //}

    vector<Particle> squarePath;

    squarePath.push_back(Particle(100.0,0.0,0.0));
    squarePath.push_back(Particle(100.0,100.0,0.0));
    squarePath.push_back(Particle(0.0,100.0,0.0));
    squarePath.push_back(Particle(0.0,0.0,0.0));

    for(int i = 1; i < 11; i++)
    {
        driveController.goToPos(i * 10.0,0.0);
        usleep(3000);
    }


    return EXIT_SUCCESS;
}
