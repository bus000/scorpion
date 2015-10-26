#include "measurement.hpp"
#include "util.hpp"

measurement::measurement(camera &cam, IplImage *im)
{
    colour_prop cp;

    object::type ID = cam.get_object(im, cp, this->distance, this->angle);

    if (ID == object::none) {
        this->landmark = NoLandmark;
    } else {
        this->landmark = colour(cp);

        switch (this->landmark) {
        case RedLandmark:
            this->position = particle(0, 300);
            break;
        case GreenLandmark:
            this->position = particle(0,0);
            break;
        default:
            fprintf(stderr, "error: unknown landmark observed\n");
        }
    }
}

measurement::~measurement(void)
{
}
