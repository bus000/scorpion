#include "irSensors.hpp"
#include "particle.hpp"
#include "util.hpp"
#include <vector>
#include <libplayerc++/playerc++.h>

using namespace std;

IRSensors::IRSensors(PlayerCc::PlayerClient *robot)
{
    this->robot = robot;
    this->filterStrength = DEFAULT_FILTERSTRENGTH;
    this->irProxy = new PlayerCc::IrProxy(this->robot);
}

IRSensors::IRSensors(PlayerCc::PlayerClient *robot, unsigned int filterStrength)
{
    this->robot = robot;
    this->filterStrength = filterStrength;
    this->irProxy = new PlayerCc::IrProxy(this->robot);
}

vector<Particle> IRSensors::getObstacles(void)
{
    vector<Particle> result;
    double sensorData[SENSOR_NUM] = {0};

    for (int i = 0; i < this->filterStrength; i++) {
        this->robot->Read();
        for (int j = 0; j < SENSOR_NUM; j++) {
            sensorData[j] += irProxy->GetRange(j);
        }
    }

    for (int i = 0; i < SENSOR_NUM; i++) {
        int index;
        int cm = sensorValueToCM(sensorData[i]);
        Particle angle = this->sensorAngle(i);
        angle.scale(cm);

        if ((index = this->parGetIndex(result, angle)) != -1) {
            Particle par = result.at(index);
            result.at(index).x((par.x() + angle.x()) / 2);
            result.at(index).y((par.y() + angle.y()) / 2);
        } else {
            result.push_back(angle);
        }
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
        return Particle(0.7071067, 0.7071067);
    case IrWestNorthWest:
        return Particle(0.7071067, -0.7071067);
    case IrNorth:
        return Particle(1.0, 0.0);
    case IrNorthEast:
        return Particle(0.5, 0.866025);
    case IrNorthWest:
        return Particle(0.5, -0.866025);
    case IrNarrowNorthNorthWest:
        return Particle(1.0, 0.0);
    case IrNarrowNorthNorthEast:
        return Particle(1.0, 0.0);
    case IrCenterNorthNorthWest:
        return Particle(0.5, -0.866025);
    case IrCenterNorthNorthEast:
        return Particle(0.5, 0.866025);
    case IrWest:
        return Particle(0.0, -1.0);
    case IrEast:
        return Particle(0.0, 1.0);
    case IrSouthLeft:
        return Particle(-1.0, 0.0);
    case IrSouthRight:
        return Particle(-1.0, 0.0);
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

IRSensors::~IRSensors(void)
{
    delete this->irProxy;
}
