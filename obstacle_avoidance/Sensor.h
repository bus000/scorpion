#ifndef SENSOR_H
#define SENSOR_H

#include <libplayerc++/playerc++.h>

#define MAX_SENSOR 1.6

class Sensor {
	private:
		double *measurements;
		int sensorID;
		int filterStrength;
		PlayerCc::IrProxy &ir;

	public:
		Sensor(PlayerCc::IrProxy &ir, int sensorID, int filterStrength);
        ~Sensor(void);

        /* Read a value from the sensor. */
		double read();

        /* Update the sensor value. */
        void update(void);
};

#endif
