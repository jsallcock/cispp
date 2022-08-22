#ifndef MATERIAL_H
#define MATERIAL_H
#include <string>
#include <vector>
#include <cmath>



struct MaterialProperties
{
    std::string name {};
    std::vector<double> sellmeier_e;
    std::vector<double> sellmeier_o;
};

MaterialProperties get_material_properties(std::string material_name);

std::pair<double, double> get_refractive_indices(double wavelength, MaterialProperties &mp);

double sellmeier_eqn(double wl_um2, double A, double B, double C, double D);

double sellmeier_eqn(double wl_um2, double A, double B, double C, double D, double E);

double sellmeier_eqn(double wl_um2, double A, double B, double C, double D, double E, double F);

#endif