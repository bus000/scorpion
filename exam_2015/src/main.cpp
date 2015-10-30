#include "worldMap.hpp"
#include "irSensors.hpp"
#include "driveCtl.hpp"
#include <unistd.h>
#include <opencv2/highgui/highgui.hpp>
#include "mapPresenter.hpp"
#include <cmath>

void updateMap(IRSensors &sensors, WorldMap &map, Particle robot) {
    vector<Particle> obstacles = sensors.getObstaclePosition(robot);

    for (int i = 0; i < obstacles.size(); i++) {
        Particle obstacle = obstacles.at(i);
        map.markFrom(robot, obstacle);
    }
}

int main(int argc, char *argv[])
{
    PlayerCc::PlayerClient robot("192.168.100.253");
    //PlayerCc::PlayerClient robot("192.168.240.129");
    PlayerCc::Position2dProxy position(&robot);
    robot.SetDataMode(PLAYER_DATAMODE_PULL);
    robot.SetReplaceRule(true, PLAYER_MSGTYPE_DATA, -1);

    WorldMap map(50, 50, 10);
    MapPresenter presenter(&map);
    DriveCtl driveCtl(&robot, &position);
    IRSensors sensors(&robot);

    /* Remove later. */
    driveCtl.setXPos(0.0);
    driveCtl.setYPos(map.width() / 2);

    Particle goal(driveCtl.getXPos() + 250, driveCtl.getYPos(), 0.0);
    Particle robotPos(driveCtl.getXPos(), driveCtl.getYPos(), 0.0);

    vector<Particle> path = map.findPath(robotPos, goal);
    cv::namedWindow("window");

    while (path.size() > 0) {
        // Find obstacles
        updateMap(sensors, map, robotPos);

        path.clear();
        // Calculate new path
        path = map.findPath(robotPos, goal);

        Particle nextStep = path.at(0);
        cout << "\033[2J\033[1;1H";
        map.print(path, robotPos);
        driveCtl.goToPos(nextStep.x(), nextStep.y());

        robotPos.x(driveCtl.getXPos());
        robotPos.y(driveCtl.getYPos());
        robotPos.theta(driveCtl.toRadians(driveCtl.getYaw()));
        cv::Mat img = presenter.draw(robotPos);
        imshow("window", img);
        //cout << "\033[2J\033[1;1H";
        //map.print(path, robotPos);
        cv::waitKey(10);
    }

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
