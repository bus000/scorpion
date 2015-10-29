#include "worldMap.hpp"
#include "irSensors.hpp"
#include "driveCtl.hpp"
#include <unistd.h>

int main(int argc, char *argv[])
{
    PlayerCc::PlayerClient robot("192.168.240.129");
    PlayerCc::Position2dProxy position(&robot);
    robot.SetDataMode(PLAYER_DATAMODE_PULL);
    robot.SetReplaceRule(true, PLAYER_MSGTYPE_DATA, -1);

    WorldMap::WorldMap map(10, 10, 10, 10);
    DriveCtl::DriveCtl driveCtl(&robot, &position);
    IRSensors::IRSensors sensors(&robot);

    for (int i = 0; i < 10; i++) {
        driveCtl.goToPos(10 * (i + 1), 0);
        vector<Particle> observed = sensors.getObstacles();
        for (int j = 0; j < observed.size(); j++) {
            printf("object spotted at (%f, %f)\n", observed.at(j).x(),
                    observed.at(j).y());
        }

        sleep(1);
    }

    return EXIT_SUCCESS;
}
