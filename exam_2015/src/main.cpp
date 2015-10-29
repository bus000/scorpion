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

        WorldMap::WorldMap map(500, 500, 30, 30);
        DriveCtl::DriveCtl driveCtl(&robot, &position);
        IRSensors::IRSensors sensors(&robot);

        /* Remove later. */
        driveCtl.setXPos(0.0);
        driveCtl.setYPos(map.width() / 2);

        Particle curPos(driveCtl.getXPos(), driveCtl.getYPos(), 0.0);
        Particle goal(driveCtl.getXPos() + 250, driveCtl.getYPos(), 0.0);

        vector<Particle> obstacles = sensors.getObstaclePosition(curPos);
        for (int i = 0; i < obstacles.size(); i++) {
            Particle obstacle = obstacles.at(i);
            map.markAround(curPos, obstacle);
        }

        vector<Particle> path = map.findPath(curPos, goal);
        while (path.size() > 0) {
            Particle next = path.at(0);

            map.print(path, curPos);

            printf("robotpos (%f, %f)\n", driveCtl.getXPos(), driveCtl.getYPos());
            printf("next (%f, %f)\n", next.x(), next.y());

            sleep(1);
            driveCtl.goToPos(next.x(), next.y());

            curPos = Particle(driveCtl.getXPos(), driveCtl.getYPos(), 0.0);

            /* Obstacles. */
            obstacles = sensors.getObstaclePosition(curPos);
            for (int i = 0; i < obstacles.size(); i++) {
                Particle obstacle = obstacles.at(i);
                map.markAround(curPos, obstacle);
            }

            /* Calculate new path. */
            path = map.findPath(curPos, goal);
        }

        for (int i = 0; i < path.size(); i++) {
            Particle partGoal = path.at(i);
            printf("next step (%f, %f)\n", partGoal.x(), partGoal.y());
            driveCtl.goToPos(partGoal.x(), partGoal.y());
        }

        //for (int i = 0; i < 10; i++) {
            //driveCtl.goToPos(20 * (i + 1), driveCtl.getYPos());
            //Particle curPos(driveCtl.getXPos(), driveCtl.getYPos(), driveCtl.getYaw());
            ////vector<Particle> observed = sensors.getObstaclePosition(curPos);
            ////for (int j = 0; j < observed.size(); j++) {
                ////Particle observedObject = observed.at(j);
                ////printf("observed (%f, %f)\n", observedObject.x(), observedObject.y());
                ////map.fieldCM(observedObject.x(), observedObject.y(), true);
            ////}

            //sleep(1);
        //}

        return EXIT_SUCCESS;
    }
