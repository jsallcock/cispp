#include <iostream>
#include "include/component.h"


int main()
{
    cispp::Polariser p(0);
    // UniaxialCrystal c();
    std::cout << p.orientation << std::endl;
    // std::cout << p.get_mueller_matrix() << std::endl;

    return 1;
}