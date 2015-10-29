#include "worldMap.hpp"
#include "irSensors.hpp"
#include "driveCtl.hpp"
#include <unistd.h>

void updateMap(IRSensors &sensors, WorldMap &map, Particle robot) {
    vector<Particle> obstacles = sensors.getObstaclePosition(curPos);

    for (int i = 0; i < obstacles.size(); i++) {
        Particle obstacle = obstacles.at(i);
        map.markAround(robot, obstacle, true);
    }
}

int main(int argc, char *argv[])
{
    PlayerCc::PlayerClient robot("192.168.240.129");
    PlayerCc::Position2dProxy position(&robot);
    robot.SetDataMode(PLAYER_DATAMODE_PULL);
    robot.SetReplaceRule(true, PLAYER_MSGTYPE_DATA, -1);

    WorldMap::WorldMap map(500, 500, 30, 30);
    DriveCtl::DriveCtl driveCtl(&robot, &position);
    IRSensors::IRSensors sensors(&robot);

    /* Remove later. */
    driveCtl.setXPos(0.0);
    driveCtl.setYPos(map.width() / 2);

    Particle goal(driveCtl.getXPos() + 250, driveCtl.getYPos(), 0.0);

    while (path.size() > 0) {
    }

    return EXIT_SUCCESS;
}
