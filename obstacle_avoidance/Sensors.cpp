#include "Sensors.h"

Sensors::Sensors(PlayerCc::PlayerClient robot, int filterStrength) {
    PlayerCc::IrProxy ir(&robot);

    for (int i = 0; i < SENSOR_NUMBER; i++) {
        this->sensors[i] = new Sensor(ir, i, filterStrength);
    }

    this->ir = ir;
}

void Sensor::update(void) {
    robot.Read();

    for (int i = 0; i < SENSOR_NUMBER; i++) {
        this->sensors[i].update();
    }
}

double Sensor::read(sensor_position_t sensor_position) {
    return this->sensors[sensor_position].read();
}
