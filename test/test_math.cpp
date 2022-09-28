#include <iostream>
#include "include/math.h"




int main (int argc, char **argv)
{
    std::vector<double> x{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 10 };
    std::vector<double> y{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 10 };
    double area = cispp::trapz(x, y);
    std::cout << area << std::endl;
    return 0;
}