#include "Sensor.h"

Sensor::Sensor(IrProxy *ir, int sensorID, int filterStrength) {
	this->ir = ir;
	this->sensorID = sensorID;
	this->filterStrength = filterStrength;
	this->measurements = new double[filterStrength];

	for (int i = 0; i < filterStrength; i++) {
		this->measurements[i] = MAX_SENSOR;
	}
}

double Sensor::read() {
	double sum = 0.0;

	for (int i = 0; i < this->filterStrength; i++)
		sum += this->measurements[i];

	return sum / this->filterStrength;
}

void Sensor::update(void) {
	for (int i = 0; i < this->filterStrength - 1; i++) {
		this->measurements[i] = this->measurements[i + 1];
	}
	this->measurements[this->filterStrength - 1] =
        this->ir->GetRange(this->sensorID);
}
