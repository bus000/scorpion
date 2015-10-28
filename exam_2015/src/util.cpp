#include "util.hpp"

#define EPSILON (0.001)


ObservedLandmark colour(colour_prop cp)
{
    return cp.red > cp.green ? RedLandmark : GreenLandmark;
}

bool AreSame(double a, double b)
{
        return fabs(a - b) < EPSILON;

}
