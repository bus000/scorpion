#include "IR_Sensors.hpp"
#include <vector>
#include <libplayerc++/playerc++.h>

IRSensors::IRSensors(PlayerCc::PlayerClient *robot)
{
    this->robot = robot;
    this->filterStrength = DEFAULT_FILTERSTRENGTH;
    this->irProxy = new IrProxy(robot);
}

IRSensors::IRSensors(PlayerCc::PlayerClient *robot, unsigned int filterStrength)
{
    this->robot = robot;
    this->filterStrength = filterStrength;
    this->irProxy = new IrProxy(robot);
}

vector<particle> IRSensors::getObstacles(void)
{
    vector<particle> result;
    double sensorData = 0.0;


    for (int i = 0; i < this->filterStrength; i++) {
        this->robot->Read();
        sensorData += this->irProxy.GetRange(IR_bn_n);
    }

    if (sensorData < 1.5)
        result.push_back(particle((sensorData / this->filterStrength), 0.0,
                    0.0));

    return result;
}

IRSensors::~IRSensors(void)
{
    delete this->irProxy;
}
