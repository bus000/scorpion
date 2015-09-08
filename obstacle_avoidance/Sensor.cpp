#include "Sensor.h"
using namespace PlayerCc;

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
	for (int i = 0; i < this->filterStrength - 1; i++) {
		this->measurements[i] = this->measurements[i + 1];
	}
	this->measurements[this->filterStrength - 1] = this->ir->GetRange(this->sensorID);

	double sum = 0.0;
	for (int i = 0; i < this->filterStrength; i++) {
		printf("%f ", this->measurements[i]);
		sum += this->measurements[i];
	}
	printf("\n");
	//sleep(1);

	printf("average: %f\n", sum / this->filterStrength);
	return sum / this->filterStrength;
}
