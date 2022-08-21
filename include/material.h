#ifndef MATERIAL_H
#define MATERIAL_H
#include <string>
#include <cmath>



struct MaterialProperties
{
    std::string name {};

    double sellAe {};
    double sellBe {};
    double sellCe {};
    double sellDe {};
    double sellEe {};
    double sellFe {};

    double sellAo {};
    double sellBo {};
    double sellCo {};
    double sellDo {};
    double sellEo {};
    double sellFo {};
};

MaterialProperties get_material_properties(std::string material_name);

void get_refractive_index_e(double wavelength, std::string material);

void get_refractive_index_o(double wavelength, std::string material);

double sellmeier_eqn(double wl_um, double A, double B, double C, double D);

double sellmeier_eqn(double wl_um, double A, double B, double C, double D, double E);

double sellmeier_eqn(double wl_um, double A, double B, double C, double D, double E, double F);

#endif