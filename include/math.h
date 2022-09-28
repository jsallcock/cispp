#include <cmath>
#include <vector>
#include <iostream>


namespace cispp {


/**
 * @brief 
 * 
 * @param x 
 * @param y 
 * @return double 
 */
// double trapz(std::vector<double> x, std::vector<double> y);
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


} // namespace cispp

