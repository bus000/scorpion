#ifndef _STATE_MACHINE_
#define _STATE_MACHINE_
#include <vector>
#include "particles.h"
#include "particleFilter.hpp"
#include "DriveCtl.hpp"
#include "measurement.hpp"

using namespace std;

typedef enum {
    STATE_INITIAL,
    STATE_FOUND_BOTH,
    STATE_PANIC
} state_t;

class StateMachine {
public:
    StateMachine(DriveCtl *driveCtl, vector<particle> *particles);
    ~StateMachine();
    void run(vector<measurement> meas);
private:
    DriveCtl *driveCtl;
    particleFilter *filter;
    state_t currentState;
    vector<particle> *particles;

    //for the initial state
    bool foundRed;
    bool foundGreen;
    int initialTotalYawed;

    void updateParticleFilter(measurement meas);

    //State functions
    state_t run_initial(vector<measurement> meas);
    state_t run_found_both(vector<measurement> meas);
};

#endif
