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

vector<particle> IRSensors::getObstacles(void)
{
    vector<particle> result;
    double sensorData = 0.0;

    for (int i = 0; i < this->filterStrength; i++) {
        this->robot->Read();
        sensorData += this->irProxy->GetRange(IR_bn_n);
    }

    if ((sensorData / this->filterStrength) < 1.5)
        result.push_back(
                particle(sensorValueToCM((sensorData / this->filterStrength)),
                    0.0, 0.0));

    return result;
}

double IRSensors::sensorValueToCM(double sensorValue)
{
    if (sensorValue > SENSOR_VALUE_FUNCTION_SHIFT)
        return (sensorValue + 1.06466) / 0.051711;
    else
        return (sensorValue - 0.0787) / 0.01389;
}


int IRSensors::parGetIndex(vector<Particle> parts, Particle part)
{
    double angleY;
    double angleArg;


    for(int i = 0; i < parts.size(); i++)
    {
        angleY = getParticleAngle(parts.at[i]);
        angleArg = getParticleAngle(part);

        if(AreSame(angleY,angleArg))
            return 1;
    }

    return -1;
}

double IRSensors::getParticleAngle(Particle part1, Particle part2)
{
    return atan2(part1.x(), part1.y()) - atan2(part2.x(), part2.y());
}

IRSensors::~IRSensors(void)
{
    delete this->irProxy;
}
