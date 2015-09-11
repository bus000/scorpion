#ifndef SENSORS_H
#define SENSORS_H

#include <libplayerc++/playerc++.h>
#include "Sensor.h"

#define SENSOR_NUMBER 5 /* Update when updating list. */
enum sensor_position_t {
    FRONT, RIGHT, LEFT, BACK_RIGHT, BACK_LEFT
}

class Sensors {
    private:
        Sensor sensors[SENSOR_NUMBER];
        PlayerCC::PlayerClient;

    public:
        Sensors(PlayerCc::PlayerClient robot, int filterStrength);
        ~Sensors(void);

        /* Updates values of all sensors. */
        void update(void);

        /* Return the latest value of all sensors. */
        double read(sensor_position_t sensor_position)
}

#endif
