#ifndef IR_SENSOR_H
#define IR_SENSOR_H

#include "Sensor.h"
#include <libplayerc++/playerc++.h>
using namespace PlayerCc;

class IrSensor: public Sensor {
public:
  static PlayerCc::IrProxy *ir;
  double read();
  IrSensor(PlayerClient* client, int sensorID);
  ~IrSensor(void);
};

#endif
