#include "../src/driveCtl.hpp"
#include "../src/particle.hpp"


int main(int argc, char *argv[])
{

    PlayerCc::PlayerClient robot("192.168.240.129");
    PlayerCc::Position2dProxy position(&robot);
    robot.SetDataMode(PLAYER_DATAMODE_PULL);
    robot.SetReplaceRule(true, PLAYER_MSGTYPE_DATA, -1);
    DriveCtl driveCtl(&robot, &position );

    driveCtl.gotoPose(Particle(100.0, 0.0, 0.0));
    driveCtl.gotoPose(Particle(100.0, 100.0, 0.0));
    driveCtl.gotoPose(Particle(0.0, 100.0, 0.0));
    driveCtl.gotoPose(Particle(0.0, 0.0, 0.0));

    return EXIT_SUCCESS;
}


