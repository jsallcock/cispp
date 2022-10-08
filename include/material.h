#ifndef CISPP_MATERIAL_H
#define CISPP_MATERIAL_H
#include <string>
#include <vector>
#include <cmath>


namespace cispp {


struct MaterialProperties
{
    std::string name {};
    std::vector<double> sellmeier_e;
    std::vector<double> sellmeier_o;
};


/**
 * @brief get default material properties by material name
 * 
 * @return MaterialProperties 
 */
cispp::MaterialProperties GetMaterialProperties(std::string material_name);

std::pair<double, double> GetRefractiveIndices(double wavelength, cispp::MaterialProperties &mp);

double GetKappa(double wavelength, cispp::MaterialProperties &mp);

double SellmeierEqn(double wl_um2, double A, double B, double C, double D);

double SellmeierEqn(double wl_um2, double A, double B, double C, double D, double E);

double SellmeierEqn(double wl_um2, double A, double B, double C, double D, double E, double F);


} // namespace cispp
#endif