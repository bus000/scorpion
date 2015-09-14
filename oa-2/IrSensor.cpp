#include "IrSensor.h"
#include "stdio.h"

#define MAX_SENSOR 1.6

PlayerCc::IrProxy *IrSensor::ir = NULL;

IrSensor::IrSensor(PlayerClient* client, int sensorID) {
  if (IrSensor::ir == NULL) {
    IrSensor::ir = new IrProxy(client);
    printf("IrSensor registered at %p", IrSensor::ir);
  }

	this->sensorID = sensorID;
  this->normalizer = MAX_SENSOR;
}

IrSensor::~IrSensor() {
  // NOP
}

double IrSensor::read() {
  return IrSensor::ir->GetRange(this->sensorID);  
}

