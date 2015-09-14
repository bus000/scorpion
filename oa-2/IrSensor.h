#ifndef IR_SENSOR_H
#define IR_SENSOR_H

#include "Sensor.h"
#include <libplayerc++/playerc++.h>
using namespace PlayerCc;

class IrSensor: public Sensor {
private:
  static PlayerCc::IrProxy *ir = NULL;

public:
  double read();
  IrSensor(IrProxy *ir, int sensorID, int filterStrength);
  ~IrSensor(void);
};

#endif
