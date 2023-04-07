#include <iostream>
#include "include/maths.h"


bool test_trapz()
{
    std::vector<double> x{ 0, 1, 2, 6, 7, 8, 10 };
    std::vector<double> y{ 0, 1, 2, 6, 7, 8, 10 };
    return (cispp::trapz(x, y) == 50);
}


bool test_wrap()
{
    const double p = 2.25 * M_PI;
    const double tol = 1e-10;
    return (std::abs(cispp::wrap(p) / M_PI - 0.25) < tol);
}


int main (int argc, char **argv)
{
    std::cout << test_trapz() << '\n';
    std::cout << test_wrap() << '\n';
}