#include "include/math.h"

namespace cispp {

double wrap(double p)
{
    double period = 2 * M_PI;
    return fmod((p + period / 2), period) - (period / 2);
}

} // namespace cispp
