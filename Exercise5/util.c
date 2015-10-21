#include "util.h"

ObservedLandmark colour(colour_prop cp)
{
    return cp.red > cp.green ? RedLandmark : GreenLandmark;
}
