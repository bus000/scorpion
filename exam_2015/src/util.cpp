#include "util.hpp"
#include <math.h>

bool areSame(double a, double b)
{
    return fabs(a - b) < EPSILON;
}
