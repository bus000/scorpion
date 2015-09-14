#include "SensorGroup.h"

double SensorGroup::totalWeight() {
  double sum = 0.0;

  for(std::list<WeightedSensor*>::iterator list_iter = 
        this->sensors.begin(); 
      list_iter != this->sensors.end(); list_iter++)
  {
    WeightedSensor *sensor = *list_iter;
    
    sum += sensor->getWeight();
  }

  return sum;
}  

void SensorGroup::addSensor(Sensor* sensor, double weight) {
  WeightedSensor *wsensor = new WeightedSensor(sensor, weight);
  this->sensors.push_front(wsensor);
}

double SensorGroup::read() {
  if (this->sensors.empty()) {
    return 0;
  }

  double total = this->totalWeight();
  double final = 0.0;

  for(std::list<WeightedSensor*>::iterator list_iter = 
        this->sensors.begin(); 
      list_iter != this->sensors.end(); list_iter++)
  {
    WeightedSensor *sensor = *list_iter;

    double reading = sensor->getSensor()->read();
    double weight  = sensor->getWeight();

    final += reading * (weight / total);
  }

  return final;
}
