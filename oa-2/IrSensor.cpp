#include "IrSensor.h"

#define MAX_SENSOR 1.6

IrSensor::IrSensor(PlayerClient* client, int sensorID) {
  if (ir == NULL) {
    ir = new IrProxy(robot);
  }

	this->sensorID = sensorID;
  this->normalizer = MAX_SENSOR;
}

IrSensor::~IrSensor() {
  // NOP
}

double IrSensor::Read() {
  return ir->GetRange(this->sensorID);  
}

