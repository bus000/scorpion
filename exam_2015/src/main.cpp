#include "worldMap.hpp"
#include "irSensors.hpp"
#include "driveCtl.hpp"
#include <unistd.h>
#include <opencv2/highgui/highgui.hpp>
#include "mapPresenter.hpp"
#include <cmath>

void updateMap(IRSensors &sensors, WorldMap &map, Particle robot) {
    vector<Particle> obstacles = sensors.getObstaclePosition(robot);

    map.decreaseProb(5.0);

    for (int i = 0; i < obstacles.size(); i++) {
        Particle obstacle = obstacles.at(i);
        Particle relativePosition(obstacle.x(), obstacle.y());
        relativePosition.sub(robot);
        double probability = (1.0 / relativePosition.length()) * 100.0;
        map.markFrom(robot, obstacle, (int)probability);
    }
}

Particle getNext(vector<Particle> path, Particle robotPos)
{
    path.insert(path.begin(), robotPos);
    vector<Particle> interpolatedPath = interpolatePath(path, 3);
    interpolatedPath.erase(interpolatedPath.begin());
    Particle nextStep = interpolatedPath.at(0);

    return nextStep;
}

int main(int argc, char *argv[])
{
    //PlayerCc::PlayerClient robot("192.168.100.253");
    PlayerCc::PlayerClient robot("192.168.240.129");
    PlayerCc::Position2dProxy position(&robot);
    robot.SetDataMode(PLAYER_DATAMODE_PULL);
    robot.SetReplaceRule(true, PLAYER_MSGTYPE_DATA, -1);

    WorldMap map(15, 15, 50);
    //MapPresenter presenter(&map);
    DriveCtl driveCtl(&robot, &position);
    IRSensors sensors(&robot);

    /* Remove later. */
    driveCtl.setXPos(0.0);
    driveCtl.setYPos(map.width() / 2);

    Particle goal(driveCtl.getXPos() + 200, driveCtl.getYPos(), 0.0);
    Particle robotPos(driveCtl.getXPos(), driveCtl.getYPos(), 0.0);

    updateMap(sensors, map, robotPos);
    vector<Particle> path = map.findPath(robotPos, goal);
    //cv::namedWindow("window");
    map.print(path, robotPos);

    while (path.size() > 0) {
        Particle nextStep = getNext(path, robotPos);
        driveCtl.goToPos(nextStep.x(), nextStep.y());

        robotPos.x(driveCtl.getXPos());
        robotPos.y(driveCtl.getYPos());
        robotPos.theta(driveCtl.toRadians(driveCtl.getYaw()));
        //cv::Mat img = presenter.draw(robotPos);
        //imshow("window", img);
        //cv::waitKey(10);

        updateMap(sensors, map, robotPos);
        map.print(path, robotPos);
        path.clear();
        // Calculate new path
        path = map.findPath(robotPos, goal);
    }

    printf("end\n");

    return EXIT_SUCCESS;
}

//int main(int argc, char **argv){
//    WorldMap map(40, 30, 10);
//    MapPresenter presenter(&map);
//
//    cv::namedWindow("window");
//
//    map.field(4,4,true);
//    map.field(8,4,true);
//    map.field(8,8,true);
//    map.field(4,8,true);
//    //map.field(1,1,true);
//    cv::Mat img = presenter.draw(Particle(12.6, 10.8, M_PI*1.25));
//
//    while(true){
//        imshow("window", img);
//
//        cv::waitKey(30);
//    }
//
//    return 0;
//}
