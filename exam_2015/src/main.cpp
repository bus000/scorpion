#include "worldMap.hpp"
#include "irSensors.hpp"
#include "driveCtl.hpp"
#include <unistd.h>
#include <opencv2/highgui/highgui.hpp>
#include "mapPresenter.hpp"
#include <cmath>
#include "camera.hpp"
#include "particleFilter.hpp"

void updateMap(IRSensors &sensors, WorldMap &map, Particle robot) {
    vector<Particle> obstacles = sensors.getObstaclePosition(robot);

    for (int i = 0; i < obstacles.size(); i++) {
        Particle obstacle = obstacles.at(i);
        map.markFrom(robot, obstacle);
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

//int main(int argc, char *argv[])
//{
//    //PlayerCc::PlayerClient robot("192.168.100.253");
//    PlayerCc::PlayerClient robot("192.168.240.129");
//    PlayerCc::Position2dProxy position(&robot);
//    robot.SetDataMode(PLAYER_DATAMODE_PULL);
//    robot.SetReplaceRule(true, PLAYER_MSGTYPE_DATA, -1);
//
//    WorldMap map(15, 15, 50);
//    //MapPresenter presenter(&map);
//    DriveCtl driveCtl(&robot, &position);
//    IRSensors sensors(&robot);
//
//    /* Remove later. */
//    driveCtl.setXPos(0.0);
//    driveCtl.setYPos(map.width() / 2);
//
//    Particle goal(driveCtl.getXPos() + 200, driveCtl.getYPos(), 0.0);
//    Particle robotPos(driveCtl.getXPos(), driveCtl.getYPos(), 0.0);
//
//    updateMap(sensors, map, robotPos);
//    vector<Particle> path = map.findPath(robotPos, goal);
//    //cv::namedWindow("window");
//    map.print(path, robotPos);
//
//    while (path.size() > 0) {
//        Particle nextStep = getNext(path, robotPos);
//        driveCtl.goToPos(nextStep.x(), nextStep.y());
//
//        robotPos.x(driveCtl.getXPos());
//        robotPos.y(driveCtl.getYPos());
//        robotPos.theta(driveCtl.toRadians(driveCtl.getYaw()));
//        //cv::Mat img = presenter.draw(robotPos);
//        //imshow("window", img);
//        //cv::waitKey(10);
//
//        updateMap(sensors, map, robotPos);
//        map.print(path, robotPos);
//        path.clear();
//        // Calculate new path
//        path = map.findPath(robotPos, goal);
//    }
//
//    printf("end\n");
//
//    return EXIT_SUCCESS;
//}

//int main(int argc, char **argv){
//    //PlayerCc::PlayerClient robot("192.168.240.129");
//    //PlayerCc::PlayerClient robot("192.168.100.253");
//
//    //PlayerCc::Position2dProxy position(&robot);
//    //robot.SetDataMode(PLAYER_DATAMODE_PULL);
//    //robot.SetReplaceRule(true, PLAYER_MSGTYPE_DATA, -1);
//
//    WorldMap map(15, 15, 50);
//    MapPresenter presenter(&map);
//    //DriveCtl driveCtl(&robot, &position);
//    //IRSensors sensors(&robot);
//
//    Camera camera(441.575145, 431.531452, 640.0);
//    vector<Particle> particles;
//    ParticleFilter filter(&particles, &map, 0.0,0.0);
//    filter.addRandomParticles(100000);
//    cv::namedWindow("window");
//
//    //map[30][40] = true;
//	//map[0][40] = true;
//	//map[0][0] = true;
//	//map[30][0] = true;
//    Particle command;
//
//    while(true){
//        Measurement meas = camera.measure(true);
//        filter.filter(meas, command);
//        command = Particle();
//        cv::Mat img = presenter.draw();
//
//        double rX = 0, rY = 0, rTheta = 0;
//        for(int i = 0; i < particles.size(); i++){
//            rX += particles[i].x();
//            rY += particles[i].y();
//            rTheta += particles[i].theta();
//            presenter.drawRobot(img, particles[i], cv::Scalar(0x0,0x0,0xff));
//        }
//        rX /= (double)particles.size();
//        rY /= (double)particles.size();
//        rTheta /= (double)particles.size();
//        Particle rPos(rX,rY,rTheta);
//        presenter.drawRobot(img, rPos, cv::Scalar(0x0,0x0,0x00));
//        cv::imshow("window", img);
//        cout << "x: " << rX << "\ny: " << rY << "\nTheta: " << rTheta << "\n";
//        if(rX > map.width() || rY > map.height() || rX < 0.0 || rY < 0.0)
//            continue;
//
//        if(meas.invalid){
//            //driveCtl.turn(15.0);
//            command = Particle(0.0, 0.0, M_PI/10.0);//driveCtl.toRadians(15.0));
//            continue;
//        }
//        //vector<Particle> path = map.findPath(rPos, meas.position);
//        //Particle next = getNext(path, rPos);
//        //command = Particle(rX - driveCtl.getXPos(),
//        //        rY - driveCtl.getYPos(),
//        //        rTheta-driveCtl.toRadians(driveCtl.getYaw()));
//
//        //driveCtl.setXPos(rX);
//        //driveCtl.setYPos(rY);
//        //driveCtl.setYaw(driveCtl.toRadians(rTheta));
//        //driveCtl.goToPos(meas.position.x(), meas.position.y());
//    }
//
//}

int main(int argc, char **argv){
    cv::namedWindow("window");
    std::vector<Particle> particles;
    WorldMap map(50, 50, 10);
    MapPresenter presenter(&map);

    ParticleFilter filter(&particles, &map, 0.0,0.0);
    filter.addRandomParticles(10000);
    Camera camera(441.575145, 431.531452, 640.0);
    map[30][40] = true;
	map[0][40] = true;
	map[0][0] = true;
	map[30][0] = true;

    while(true){
        Measurement meas = camera.measure(true);
        filter.filter(meas);
        cv::Mat img = presenter.draw();
        
        double rX, rY, rTheta = 0;
        for(int i = 0; i < particles.size(); i++){
            rX += particles[i].x();
            rY += particles[i].y();
            rTheta += particles[i].theta();
            presenter.drawRobot(img, particles[i], cv::Scalar(0x0,0x0,0xff));
        }
        rX /= (double)particles.size();
        rY /= (double)particles.size();
        rTheta /= (double)particles.size();
        Particle robot(rX,rY,rTheta);
        presenter.drawRobot(img, robot, cv::Scalar(0x0,0x0,0x00));  
        cv::imshow("window", img);
     }
}
