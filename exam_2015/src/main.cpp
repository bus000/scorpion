#include "worldMap.hpp"
#include "irSensors.hpp"
#include "driveCtl.hpp"
#include <unistd.h>

void updateMap(IRSensors &sensors, WorldMap &map, Particle robot) {
    vector<Particle> obstacles = sensors.getObstaclePosition(robot);

    for (int i = 0; i < obstacles.size(); i++) {
        Particle obstacle = obstacles.at(i);
        map.markAround(robot, obstacle);
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
    Particle robotPos(driveCtl.getXPos(), driveCtl.getYPos(), 0.0);

    vector<Particle> path = map.findPath(robotPos, goal);

    while (path.size() > 0) {
        robotPos.x(driveCtl.getXPos());
        robotPos.y(driveCtl.getYPos());

        // Find obstacles
        updateMap(sensors, map, robotPos);

        // Calculate new path
        path = map.findPath(robotPos, goal);

        Particle nextStep = path.at(0);
        driveCtl.goToPos(nextStep.x(), nextStep.y());

        /* Update robot position. */
        robotPos.x(driveCtl.getXPos());
        robotPos.y(driveCtl.getYPos());

        cout << "\033[2J\033[1;1H";
        map.print(path, robotPos);
    }

    return EXIT_SUCCESS;
}
