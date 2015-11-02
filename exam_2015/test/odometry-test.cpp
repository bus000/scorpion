#include "../src/driveCtl.hpp"
#include "../src/particle.hpp"
#include <iostream>

using namespace std;

bool test(Particle p, void *data){
    printf("(%f, %f) angle: %f\n", p.x(), p.y(), p.theta());
    
    return *((bool*)data);
}

int main(int argc, char *argv[])
{

    PlayerCc::PlayerClient robot("192.168.100.253");
    //PlayerCc::PlayerClient robot("localhost");

    PlayerCc::Position2dProxy position(&robot);

    robot.SetDataMode(PLAYER_DATAMODE_PULL);
    robot.SetReplaceRule(true, PLAYER_MSGTYPE_DATA, -1);

    DriveCtl driveCtl(&robot, &position );
   
    //driveCtl.drive(100.0);
//    driveCtl.turn(M_PI/2.0);
//    cout << "done\n";
//    driveCtl.turn(-1.0*M_PI);
//    cout << "done\n";
//    driveCtl.turn(7*M_PI/4);
//    cout << "done\n";

    //driveCtl.drive(150.0);
    //driveCtl.turn(M_PI);
    //driveCtl.drive(150.0);
    //driveCtl.turn(-1.0*M_PI);
    //

    //bool con = true;
    //driveCtl.gotoPose(Particle(150.0,50.0), (void*)&con, &test);
    //driveCtl.gotoPose(Particle(100.0,100), (void*)&con, &test);
    //driveCtl.gotoPose(Particle(100.0,0.0), (void*)&con, &test);
    //driveCtl.gotoPose(Particle(0.0,100.0), (void*)&con, &test);
    //con = false;
    //driveCtl.gotoPose(Particle(0.0,0.0), (void*)&con, &test);
    
    Particle pos(150,150,M_PI/4.0);
    driveCtl.setPose(pos);
    pos = driveCtl.pose();
    printf("(%f, %f) angle: %f\n", pos.x(), pos.y(), pos.theta());
    driveCtl.gotoPose(Particle(), NULL, NULL);

    /*
    driveCtl.gotoPose(Particle(100.0, 0.0, 0.0));
    driveCtl.gotoPose(Particle(100.0, 100.0, 0.0));
    driveCtl.gotoPose(Particle(0.0, 100.0, 0.0));
    driveCtl.gotoPose(Particle(0.0, 0.0, 0.0));
    */

    return EXIT_SUCCESS;
}


