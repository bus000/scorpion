#include "measurement.hpp"
#include "util.hpp"

measurement::measurement(camera &cam, IplImage &im)
{
    double tmp_angle;
    colour_prop cp;

    object::type ID = cam.get_object(&im, cp, this->distance, tmp_angle);

    this->angle = (180 / M_PI) * tmp_angle;

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
