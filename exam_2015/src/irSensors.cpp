#include "irSensors.hpp"
#include "particle.hpp"
#include "util.hpp"
#include <vector>
#include <libplayerc++/playerc++.h>
#include <math.h>

using namespace std;

IRSensors::IRSensors(PlayerCc::PlayerClient *robot)
{
    this->robot = robot;
    this->filterStrength = DEFAULT_FILTERSTRENGTH;
    this->irProxy = new PlayerCc::IrProxy(this->robot);

    for (int i = 0; i < this->filterStrength; i++)
        this->robot->Read();
}

IRSensors::IRSensors(PlayerCc::PlayerClient *robot, unsigned int filterStrength)
{
    this->robot = robot;
    this->filterStrength = filterStrength;
    this->irProxy = new PlayerCc::IrProxy(this->robot);

    for (int i = 0; i < this->filterStrength; i++)
        this->robot->Read();
}

vector<Particle> IRSensors::getObstacles(void)
{
    vector<Particle> result;
    double sensorData[SENSOR_NUM] = {0};

    for (int i = 0; i < this->filterStrength; i++) {
        this->robot->Read();
        for (int j = 0; j < SENSOR_NUM; j++)
            sensorData[j] += irProxy->GetRange(j);
    }

    for (int i = 0; i < SENSOR_NUM; i++) {
        double average = sensorData[i] / this->filterStrength;
        if (average > CUTOFF)
            continue;

        double distance = sensorValueToCM(average);
        Particle angle = this->sensorAngle(i);
        Particle position = this->sensorPosition(i);
        angle.move(position.x(), position.y(), 0.0);
        angle.scale(distance);

        result.push_back(angle);
    }

    return result;
}

double IRSensors::sensorValueToCM(double sensorValue)
{
    if (sensorValue > SENSOR_VALUE_FUNCTION_SHIFT)
        return (sensorValue + 1.06466) / 0.051711;
    else
        return (sensorValue - 0.0787) / 0.01389;
}

Particle IRSensors::sensorAngle(int sensor)
{
    switch (sensor) {
    case IrEastNorthEast:
        return Particle(sin(20.0 * M_PI / 180.0), cos(20.0 * M_PI / 180.0));
    case IrWestNorthWest:
        return Particle(sin(160.0 * M_PI / 180.0), cos(160.0 * M_PI / 180.0));
    case IrNorth:
        return Particle(sin(90.0 * M_PI / 180.0), cos(90.0 * M_PI / 180.0));
    case IrNorthEast:
        return Particle(sin(45.0 * M_PI / 180.0), cos(45.0 * M_PI / 180.0));
    case IrNorthWest:
        return Particle(sin(135.0 * M_PI / 180.0), cos(135.0 * M_PI / 180.0));
    case IrNarrowNorthNorthWest:
        return Particle(sin(105.0 * M_PI / 180.0), cos(105.0 * M_PI / 180.0));
    case IrNarrowNorthNorthEast:
        return Particle(sin(75.0 * M_PI / 180.0), cos(75.0 * M_PI / 180.0));
    case IrCenterNorthNorthWest:
        return Particle(sin(105.0 * M_PI / 180.0), cos(105.0 * M_PI / 180.0));
    case IrCenterNorthNorthEast:
        return Particle(sin(75.0 * M_PI / 180.0), cos(75.0 * M_PI / 180.0));
    case IrWest:
        return Particle(sin(180.0 * M_PI / 180.0), cos(180.0 * M_PI / 180.0));
    case IrEast:
        return Particle(sin(0.0 * M_PI / 180.0), cos(0.0 * M_PI / 180.0));
    case IrSouthLeft:
        return Particle(sin(-90.0 * M_PI / 180.0), cos(-90.0 * M_PI / 180.0));
    case IrSouthRight:
        return Particle(sin(-90.0 * M_PI / 180.0), cos(-90.0 * M_PI / 180.0));
    default:
        fprintf(stderr, "err: unknown IR sensor with ID %d\n", sensor);
        return Particle(0.0, 0.0);
    }
}

Particle IRSensors::sensorPosition(int sensor)
{
    switch (sensor) {
    case IrEastNorthEast:
        return Particle(10.0, 0.0);
    case IrWestNorthWest:
        return Particle(10.0, 0.0);
    case IrNorth:
        return Particle(10.0, 0.0);
    case IrNorthEast:
        return Particle(10.0, 0.0);
    case IrNorthWest:
        return Particle(10.0, 0.0);
    case IrNarrowNorthNorthWest:
        return Particle(0.0, 10.0);
    case IrNarrowNorthNorthEast:
        return Particle(0.0, 10.0);
    case IrCenterNorthNorthWest:
        return Particle(0.0, -10.0);
    case IrCenterNorthNorthEast:
        return Particle(0.0, -10.0);
    case IrWest:
        return Particle(-5.0, 0.0);
    case IrEast:
        return Particle(-5.0, 0.0);
    case IrSouthLeft:
        return Particle(0.0, -5.0);
    case IrSouthRight:
        return Particle(0.0, 5.0);
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
        if (areSame(0.0, cur.angleBetween(&part)))
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
