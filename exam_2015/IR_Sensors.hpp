#ifndef IR_SENSORS_HPP
#define IR_SENSORS_HPP

#include <libplayerc++/playerc++.h>

#define DEFAULT_FILTERSTRENGTH (10)

/* Sensors. */
#define IR_bn_ene (0)
#define IR_bn_wnw (1)
#define IR_bn_n   (2)
#define IR_bn_ne  (3)
#define IR_bn_nw  (4)
#define IR_te_nnw (5)
#define IR_te_nne (6)
#define IR_tw_nnw (7)
#define IR_tw_nne (8)
#define IR_bs_w   (9)
#define IR_bs_e   (10)
#define IR_bw_s   (11)
#define IR_be_s   (12)

/* Handle the inferred sensors on the robot. */
class IRSensors {
public:
    IRSensors(PlayerCc::PlayerClient *robot);
    IRSensors(PlayerCc::PlayerClient *robot, unsigned int filterStrength)
    ~IRSensors(void);

    /* Returns a list of particles in the directions obstacles are discovered.
     * If no obstacles is found, an empty vector is returned. The distance from
     * the robot to the obstacle can be approximated with the length of the
     * particle returned. */
    vector<particle> getObstacles(void);

    vector<particle> getObstaclePosition(particle robotPos);

private:
    unsigned int filterStrength;
    PlayerCc::PlayerClient *robot;
    PlayerCc::PlayerClient *irProxy;
};

#endif
