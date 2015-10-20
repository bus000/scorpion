#include <iostream>
#include <libplayerc++/playerc++.h>
#include "DriveCtl.hpp"
#include <unistd.h>

void init_robot(PlayerCc::PlayerClient **robot,
        PlayerCc::Position2dProxy **position){

    *robot = new PlayerCc::PlayerClient("192.168.100.253");
    //*robot = new PlayerCc::PlayerClient("localhost");
    (*robot)->SetDataMode(PLAYER_DATAMODE_PULL);
    (*robot)->SetReplaceRule(true, PLAYER_MSGTYPE_DATA,-1);
    *position = new PlayerCc::Position2dProxy(*robot);
}

int main(int argc, char **argv){
    //Do something
    PlayerCc::PlayerClient *robot;
    PlayerCc::Position2dProxy *position;
    init_robot(&robot, &position);

    DriveCtl c(robot, position);
    c.drive(100.0);
    std::cout << "pos: (" << c.getXPos() << ", ";
    std::cout << c.getYPos() << ")" << std::endl;
    std::cout << "yaw: " << c.getYaw() << std::endl << std::endl;
    std::cout << "yawed: " << c.getYawed() << std::endl << std::endl;
    std::cout << "driven: " << c.getDriven() << std::endl << std::endl;
    c.turnLeft(90.0);
    std::cout << "pos: (" << c.getXPos() << ", ";
    std::cout << c.getYPos() << ")" << std::endl;
    std::cout << "yaw: " << c.getYaw() << std::endl << std::endl;
    std::cout << "yawed: " << c.getYawed() << std::endl << std::endl;
    std::cout << "driven: " << c.getDriven() << std::endl << std::endl;
    c.drive(100.0);
    std::cout << "pos: (" << c.getXPos() << ", ";
    std::cout << c.getYPos() << ")" << std::endl;
    std::cout << "yaw: " << c.getYaw() << std::endl << std::endl;
    std::cout << "yawed: " << c.getYawed() << std::endl << std::endl;
    std::cout << "driven: " << c.getDriven() << std::endl << std::endl;
    c.goToPos(2.0,0.0);
    std::cout << "pos: (" << c.getXPos() << ", ";
    std::cout << c.getYPos() << ")" << std::endl;
    std::cout << "yaw: " << c.getYaw() << std::endl << std::endl;
    std::cout << "yawed: " << c.getYawed() << std::endl << std::endl;
    std::cout << "driven: " << c.getDriven() << std::endl << std::endl;

    usleep(1000000);
    
    return 0;
}
