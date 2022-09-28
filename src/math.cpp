#include "include/math.h"

namespace cispp {


double trapz(std::vector<double> x, std::vector<double> y)
{
    assert (x.size() == y.size());
    double out = 0;
    for (size_t i = 1; i < x.size(); i++)
    {
        out += 0.5 * (y[i-1] + y[i]) * (x[i] - x[i-1]);
    } 
    return out;
}


} // namespace cispp