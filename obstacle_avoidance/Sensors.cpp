#include "Sensors.h"

Sensors::Sensors(PlayerClient *robot, int filterStrength) {
    IrProxy *ir = new IrProxy(robot);

    for (int i = 0; i < SENSOR_NUMBER; i++) {
        this->sensors[i] = new Sensor(ir, i, filterStrength);
    }

    this->ir = ir;
}

void Sensors::update(void) {
    robot->Read();

    for (int i = 0; i < SENSOR_NUMBER; i++) {
        this->sensors[i]->update();
    }
}

double Sensors::read(int sensor_position) {
    return this->sensors[sensor_position]->read();
}
