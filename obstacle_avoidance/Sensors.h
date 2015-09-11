#ifndef SENSORS_H
#define SENSORS_H

#include <libplayerc++/playerc++.h>
#include "Sensor.h"
using namespace PlayerCc;

#define SENSOR_NUMBER 13 /* Update when updating list. */

class Sensors {
    private:
        Sensor *sensors[SENSOR_NUMBER];
        PlayerClient *robot;
        IrProxy *ir;

    public:
        Sensors(PlayerClient *robot, int filterStrength);
        ~Sensors(void);

        /* Updates values of all sensors. */
        void update(void);

        /* Return the latest value of all sensors. */
        double read(int sensor_position);
};

#endif
