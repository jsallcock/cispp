#ifndef CISPP_MATH_H
#define CISPP_MATH_H

#include <cmath>
#include <vector>
#include <iostream>


namespace cispp {


/**
 * @brief numerical integration using the trapezoidal rule
 * 
 * @param x 
 * @param y 
 * @return 
 */
template <typename T>
T trapz(std::vector<T> x, std::vector<T> y)
{
    assert (x.size() == y.size());
    T out = 0;
    for (size_t i = 1; i < x.size(); i++)
    {
        out += 0.5 * (y[i-1] + y[i]) * (x[i] - x[i-1]);
    } 
    return out;
}


/**
 * @brief Wrap phase angle into (- pi, pi] radian interval
 * 
 * @param p 
 * @return double 
 */
double wrap(double p);


} // namespace cispp
#endif

