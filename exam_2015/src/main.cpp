#include "worldMap.hpp"
#include "irSensors.hpp"
#include "driveCtl.hpp"
#include <unistd.h>
#include <opencv2/highgui/highgui.hpp>
#include "mapPresenter.hpp"
#include <cmath>
#include "camera.hpp"
#include "particleFilter.hpp"
#include "mapPresenter.hpp"

#define LANDMARK_STOP 115.0

Particle landmark1(200.0, 200.0);
Particle landmark2(200.0, 500.0);
Particle landmark3(600.0, 200.0);
Particle landmark4(600.0, 500.0);

double variance(Particle robot, vector<Particle> &particles){
    double variance = 0;
    for(int i = 0; i < particles.size(); i++){
        Particle diff = particles[i];
        diff.sub(robot);
        variance += diff.length()/particles.size();
    }
    return variance;
}

struct testData {
    Camera *camera;
    ParticleFilter *filter;
    vector<Particle> *particles;
    Particle prevPose;
    MapPresenter *presenter;
    Particle *landmark;
    Particle *robot;
    IRSensors *irSensor;
    //Internal data
    vector<Particle> landmarksSeen;
    bool inFront;
    bool inLocalize;
};

unsigned long timing(){
    struct timeval tv;
    gettimeofday(&tv,NULL);
    return 1000000 * tv.tv_sec + tv.tv_usec;
}

void draw(MapPresenter *presenter, vector<Particle> *particles, Particle robot){
    cv::Mat img = presenter->draw();
    double rX = 0, rY = 0, rTheta = 0;
    for(int i = 0; i < particles->size(); i++){
        presenter->drawRobot(img, particles->at(i), cv::Scalar(0x0,0x0,0xff));
    }

    presenter->drawRobot(img, robot, cv::Scalar(0x0,0x0,0x00));  
    //printf("Timing is: %u\n", timing()-t);
    cv::imshow("window", img);
}

bool driveAndMeasure(Particle command, void *_data){
    unsigned long t = timing();
    testData *data = (testData*)_data;
    Measurement meas;
    meas.invalid = true;
    data->inFront = false;
    if(data->camera->hasMeasurement())
        meas = data->camera->getMeasurement();

    data->filter->filter(meas, command);

    draw(data->presenter, data->particles, data->filter->believe);
    
    if(!meas.invalid && meas.measurement.length() < LANDMARK_STOP &&
            data->landmark->compareCoord(meas.position))
        return false;

    if(data->irSensor->obstacleInFront() && command.theta() == 0.0){
        cout << "InFront" << endl;
        data->inFront = true;
        return false;
    }

    return true;
}

//bool turnToLandmark(Particle command, void *_data){
//    testData *data = (testData*)_data;
//
//    Measurement meas;
//    meas.invalid = true;
//    if(data->camera->hasMeasurement())
//        meas = data->camera->getMeasurement();
//
//    data->filter->filter(meas, command);
//    draw(data->presenter, data->particles, data->filter->believe);
//    if(!meas.invalid){
//        if(data->landmark.x() == meas.position.x() &&
//                data->landmark.y() == meas.position.y()){
//
//            data->reachLandmark = true;
//            return false;
//        }
//    }
//    return true;
//}

void localize(testData *data, DriveCtl *driveCtl){
    data->inLocalize = true;
    cout << "in search mode" << endl;
    for(int i = 0; i < 2; i++){
        driveCtl->turn(M_PI, (void*)data, &driveAndMeasure);
    }
    data->inLocalize = false;
    cout << "out of search mode" << endl;
}

bool deltaTest(Particle delta, void *_data){
    cout << "delta: (" << delta.x() << ", " << delta.y() <<  ")" << endl;
    testData *data = (testData*)_data;

    Measurement meas;
    meas.invalid = true;
    if(data->camera->hasMeasurement())
        meas = data->camera->getMeasurement();

    data->filter->filter(meas, delta);
    draw(data->presenter, data->particles, data->filter->believe);
    return true;
}

int main(int argc, char **argv){
    PlayerCc::PlayerClient robot("192.168.100.254");
    //PlayerCc::PlayerClient robot("localhost");

    PlayerCc::Position2dProxy position(&robot);
    robot.SetDataMode(PLAYER_DATAMODE_PULL);
    robot.SetReplaceRule(true, PLAYER_MSGTYPE_DATA, -1);
    IRSensors irSensor(&robot, 5);

    vector<Particle> landmarks;
    landmarks.push_back(landmark1);
    landmarks.push_back(landmark2);
    landmarks.push_back(landmark3);
    landmarks.push_back(landmark4);

    WorldMap map(16, 14, 50, landmarks);
    MapPresenter presenter(&map);
    DriveCtl driveCtl(&robot, &position);

    Camera camera(1089.094385, 1042.647751, 640.0, true);
    vector<Particle> particles;
    ParticleFilter filter(&particles, &map, 0.0,0.0);
    filter.addRandomParticles(10000);
    cv::namedWindow("window");

    testData data;
    data.camera = &camera;
    data.filter = &filter;
    data.particles = &particles;
    data.presenter = &presenter;
    data.irSensor = &irSensor;

    ///////////////////////////////////////////////////////////////////
   // driveCtl.turn(3.0*M_PI/4.0, &data, &deltaTest);
   // driveCtl.drive(200.0, &data, &deltaTest);

   // return 0;
    ///////////////////////////////////////////////////////////////////

    map.decreaseProb(5);

    int targetNo = 1;

    bool dropLocalize = false;
    while(true){
        Particle target = landmarks[targetNo];
        data.landmark = &target;
        data.landmarksSeen.clear();
        localize(&data, &driveCtl);

        dropLocalize = false;

        Particle believe = filter.believe;
        Particle diff = target;
        diff.sub(believe);

        if(diff.length() > LANDMARK_STOP){
            diff.normalize();
            diff.scale(150.0);
            diff.add(believe);
            driveCtl.setPose(believe);
            cout << "diff: (" << diff.x() << ", " << diff.y() << ")" << endl;
            driveCtl.gotoPose(diff, (void*)&data, &driveAndMeasure);
            while(data.inFront){
                Particle esc = irSensor.escapeVector();
                double sign = 1.0;
                if(esc.angle() < 0.0)
                    sign = -1.0;

                driveCtl.turn(M_PI/2.0*sign, (void*)&data, &driveAndMeasure);
                driveCtl.drive(40.0, (void*)&data, &driveAndMeasure);
            }
        }else{
            if(targetNo == 3){
                //Never gonna happen
                cout << "hahah" << endl;
                exit(0);
            }else{
                targetNo++;
            }
        }
    }

    return 0;
}
