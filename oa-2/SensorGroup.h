#ifndef SENSOR_GROUP_H
#define SENSOR_GROUP_H

#include "Sensor.h"

class WeightedSensor {
private:
  double weight;
  Sensor *sensor;

public:
  WeightedSensor(Sensor *sensor, double weight) {
    this->sensor = sensor;
    this->weight = weight;
  };

  ~WeightedSensor() {};

  double getWeight() { 
    return this->weight;
  };

  Sensor *sensor getSensor() {
    return this->sensor;
  };
}

class SensorGroup: public Sensor {
private:
  std::list<WeightedSensor*> sensors;

public:
  SensorGroup();
  ~SensorGroup();

  void addSensor(Sensor *sensor, double weight);
  double read();
}

#endif
