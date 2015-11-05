#include "irSensors.hpp"
#include "particle.hpp"
#include "util.hpp"
#include <vector>
#include <libplayerc++/playerc++.h>
#include <math.h>

using namespace std;

IRSensors::IRSensors(PlayerCc::PlayerClient *robot, unsigned int filterStrength)
{
    this->robot = robot;
    this->filterStrength = filterStrength;
    this->irProxy = new PlayerCc::IrProxy(this->robot);


    // Making Preleminary Read to get rid of garbage data
    for(int i = 0; i < filterStrength; i ++)
    {
        robot->Read();
    }

}

bool IRSensors::obstacleInFront(){
    return false;
    vector<Particle> obstacles = this->getObstacles();

    for(int i = 0; i < obstacles.size(); i++){
        if(obstacles[i].length() < 25.0)
                return true;
    }

    return false;
}

Particle IRSensors::escapeVector() {
    double sensorData[SENSOR_NUM] = {0};
    double distance;
    double average;

    Particle escape(0, 0, 0);

    for (int i = 0; i < this->filterStrength; i++) {
        this->robot->Read();
        for (int j = 0; j < SENSOR_NUM; j++)
            sensorData[j] += irProxy->GetRange(j);
    }

    for (int i = 0; i < SENSOR_NUM; i++) {
        if (i == IR_te_nne || i == IR_te_nnw ||
            i == IR_tw_nne || i == IR_tw_nnw ||
            i == IR_bs_w   || i == IR_bs_e   ||
            i == IR_bw_s   || i == IR_be_s)
            continue;   

        average = sensorData[i] / (double) this->filterStrength;

        distance = sensorValueToCM(average);
        Particle angle = this->sensorAngle(i);
        angle.scale(distance);

        escape.add(angle);
    }

    return escape;
}

vector<Particle> IRSensors::getObstacles(void)
{
    vector<Particle> result;
    double sensorData[SENSOR_NUM] = {0};
    double distance;
    double average;

    for (int i = 0; i < this->filterStrength; i++) {
        this->robot->Read();
        for (int j = 0; j < SENSOR_NUM; j++)
            sensorData[j] += irProxy->GetRange(j);
    }

    for (int i = 0; i < SENSOR_NUM; i++) {
        if (i == IR_te_nne || i == IR_te_nnw ||
            i == IR_tw_nne || i == IR_tw_nnw ||
            i == IR_bs_w   || i == IR_bs_e   ||
            i == IR_bw_s   || i == IR_be_s)
            continue;   

        average = sensorData[i] / (double) this->filterStrength;
        if (average > CUTOFF)
            continue;

        distance = sensorValueToCM(average);
        Particle angle = this->sensorAngle(i);
        angle.scale(distance);

        result.push_back(angle);
    }

    return result;
}

double IRSensors::sensorValueToCM(double sensorValue)
{
    return (log(sensorValue) - log(0.111128921))/log(1.053970723);
}

Particle IRSensors::sensorAngle(int sensor)
{
    switch (sensor) {
    case IrEastNorthEast:
        return Particle(cos(-70.0 * M_PI / 180.0), sin(-70.0 * M_PI / 180.0));
    case IrWestNorthWest:
        return Particle(cos(70.0 * M_PI / 180.0), sin(70.0 * M_PI / 180.0));
    case IrNorth:
        return Particle(cos(0.0 * M_PI / 180.0), sin(0.0 * M_PI / 180.0));
    case IrNorthEast:
        return Particle(cos(-45.0 * M_PI / 180.0), sin(-45.0 * M_PI / 180.0));
    case IrNorthWest:
        return Particle(cos(45.0 * M_PI / 180.0), sin(45.0 * M_PI / 180.0));
    case IrNarrowNorthNorthWest:
        return Particle(cos(105.0 * M_PI / 180.0), sin(105.0 * M_PI / 180.0));
    case IrNarrowNorthNorthEast:
        return Particle(cos(75.0 * M_PI / 180.0), sin(75.0 * M_PI / 180.0));
    case IrCenterNorthNorthWest:
        return Particle(cos(105.0 * M_PI / 180.0), sin(105.0 * M_PI / 180.0));
    case IrCenterNorthNorthEast:
        return Particle(cos(75.0 * M_PI / 180.0), sin(75.0 * M_PI / 180.0));
    case IrWest:
        return Particle(cos(180.0 * M_PI / 180.0), sin(180.0 * M_PI / 180.0));
    case IrEast:
        return Particle(cos(0.0 * M_PI / 180.0), sin(0.0 * M_PI / 180.0));
    case IrSouthLeft:
        return Particle(cos(-90.0 * M_PI / 180.0), sin(-90.0 * M_PI / 180.0));
    case IrSouthRight:
        return Particle(cos(-90.0 * M_PI / 180.0), sin(-90.0 * M_PI / 180.0));
    default:
        fprintf(stderr, "err: unknown IR sensor with ID %d\n", sensor);
        return Particle(0.0, 0.0);
    }
}

Particle IRSensors::sensorPosition(int sensor)
{
    switch (sensor) {
    case IrEastNorthEast:
        return Particle(18.0,-8.0);
    case IrWestNorthWest:
        return Particle(18.0, 8.0);
    case IrNorth:
        return Particle(8.0, 0.0);
    case IrNorthEast:
        return Particle(13.0, 6.5);
    case IrNorthWest:
        return Particle(13.0,-6.5);
    case IrNarrowNorthNorthWest:
        return Particle(6.5, -18.0);
    case IrNarrowNorthNorthEast:
        return Particle(6.5, 18.0);
    case IrCenterNorthNorthWest:
        return Particle(-4.0, -19.0);
    case IrCenterNorthNorthEast:
        return Particle(-4.0, 19.0);
    case IrWest:
        return Particle(-15.0, -4.0);
    case IrEast:
        return Particle(-15.0, 4.0);
    case IrSouthLeft:
        return Particle(-10.0, -7.5);
    case IrSouthRight:
        return Particle(-10.0, 7.5);
    default:
        fprintf(stderr, "err: unknown IR sensor with ID %d\n", sensor);
        return Particle(0.0, 0.0);
    }
}

int IRSensors::parGetIndex(vector<Particle> parts, Particle part)
{
    double angleY;
    double angleArg;
    Particle cur;


    for(int i = 0; i < parts.size(); i++)
    {
        cur = parts.at(i);
        if (areSame(0.0, cur.angleBetween(part)))
            return i;
    }

    return -1;
}

vector<Particle> IRSensors::getObstaclePosition(Particle robotPos)
{
    vector<Particle> obstacles = this->getObstacles();

    for (int i = 0; i < obstacles.size(); i++) {
        obstacles.at(i).rotate(robotPos.theta());
        obstacles.at(i).move(robotPos.x(), robotPos.y(), 0.0);
    }

    return obstacles;
}

IRSensors::~IRSensors(void)
{
    delete this->irProxy;
}
