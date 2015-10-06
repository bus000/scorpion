#include <iostream>
#include <libplayerc++/playerc++.h>
#include "DriveCtl.hpp"
#include <unistd.h>

void init_robot(PlayerCc::PlayerClient **robot,
        PlayerCc::Position2dProxy **position){

    *robot = new PlayerCc::PlayerClient("192.168.100.253");
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
    c.drive(30.0);
    c.turn(180.0);
    c.drive(30.0);
    c.turn(90.0);
    
    return 0;
}
