#include <iostream>
#include "include/material.h"


int main()
{
    double wavelength = 465e-9;
    std::string material_name("a-BBO"); 
    cispp::MaterialProperties mp = cispp::get_material_properties(material_name);
    std::pair<double,double> nn = get_refractive_indices(wavelength, mp);
    std::cout << nn.first << ' ' << nn.second << std::endl;
}