#include "measurement.h"
#include "state.hpp"

measurement::measurement(camera &cam, IplImage &im, particle position)
{
    colour_prop cp;

    object::type ID = cam.get_object(im, cp, this->distance, this->angle);
    this->position = position;

    if (ID == object::none)
        this->landmark = NoLandmark;
    else
        this->landmark = colour(cp);
}
