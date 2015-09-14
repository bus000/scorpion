#ifndef SENSOR_H
#define SENSOR_H

#include <libplayerc++/playerc++.h>
using namespace PlayerCc;

#define MAX_SENSOR 1.6

class Sensor {
protected:
  int sensorID;
  int readings;
  double sum;
  double normalizer;

public:
  virtual double read() {};
  
  Sensor(){
    this->sensorID   = 0;
    this->readings   = 0;
    this->sum        = 0;
    this->normalizer = 1.0;
  };

  /* Starts a measurement */
  void startMeasure(){
    this->sum      = 0.0;
    this->readings = 0;
  };

  /* Update the sensor value. */
  void measure() {
    double reading = this->read();

    this->readings += 1;
    this->sum      += reading;
  };

  /* Ends a measurement */
  double endMeasure() {
    return (this->sum / this->readings) / this->normalizer;
  };
};

#endif
