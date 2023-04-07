#include "include/material.h"

#include <cmath>
#include <cstdlib>
#include <filesystem>
#include <stdexcept>
#include <string>

#include "yaml-cpp/yaml.h"

namespace cispp {

MaterialProperties GetMaterialProperties(std::string material_name)
{
    MaterialProperties mp = {};
    mp.name = material_name;
    
    std::filesystem::path filePath = std::getenv("CISPP_ROOT");
    filePath /= "data";
    filePath /= "material.yaml";
    YAML::Node data = YAML::LoadFile(filePath);
    //std::filesystem::path implicitly convertable to std::basic_string

    if (!data[material_name]["sellmeier_coefficients"]){
        throw std::logic_error("invalid material_name");
    }

    std::string alphabet = "ABCDEF";
    for (int i=0; i<alphabet.size(); i++)
    {
        std::string key(1, alphabet[i]);
        if (data[material_name]["sellmeier_coefficients"][0][key + "e"])
        {
            mp.sellmeier_e.push_back(data[material_name]["sellmeier_coefficients"][0][key + "e"].as<double>());
            mp.sellmeier_o.push_back(data[material_name]["sellmeier_coefficients"][0][key + "o"].as<double>());
        }
    }   
    return mp;
}


std::pair<double, double> GetRefractiveIndices(double wavelength, MaterialProperties &mp)
{
    const size_t nce = mp.sellmeier_e.size();
    const size_t nco = mp.sellmeier_o.size();
    const double wl_um2 = pow(wavelength * 1e6, 2); 
    assert (nce == nco);
    double ne, no;
    
    switch(nce)
    {
        case 4:
            ne = SellmeierEqn(
                wl_um2, 
                mp.sellmeier_e[0], 
                mp.sellmeier_e[1], 
                mp.sellmeier_e[2], 
                mp.sellmeier_e[3]
            );
            no = SellmeierEqn(
                wl_um2, 
                mp.sellmeier_o[0], 
                mp.sellmeier_o[1], 
                mp.sellmeier_o[2], 
                mp.sellmeier_o[3]
            );
            break;
        
        case 5:
            ne = SellmeierEqn(
                wl_um2, 
                mp.sellmeier_e[0], 
                mp.sellmeier_e[1], 
                mp.sellmeier_e[2], 
                mp.sellmeier_e[3],
                mp.sellmeier_e[4]
            );
            no = SellmeierEqn(
                wl_um2, 
                mp.sellmeier_o[0], 
                mp.sellmeier_o[1], 
                mp.sellmeier_o[2], 
                mp.sellmeier_o[3],
                mp.sellmeier_o[4]
            );
            break;
        
        case 6:
            ne = SellmeierEqn(
                wl_um2, 
                mp.sellmeier_e[0], 
                mp.sellmeier_e[1], 
                mp.sellmeier_e[2], 
                mp.sellmeier_e[3],
                mp.sellmeier_e[4],
                mp.sellmeier_e[5]
            );
            no = SellmeierEqn(
                wl_um2, 
                mp.sellmeier_o[0], 
                mp.sellmeier_o[1], 
                mp.sellmeier_o[2], 
                mp.sellmeier_o[3],
                mp.sellmeier_o[4],
                mp.sellmeier_o[5]
            );
            break;

        default:
            throw std::logic_error("input not understood");
    }
    return std::pair<double, double>(ne, no);
}


double GetKappa(double wavelength, cispp::MaterialProperties &mp)
{
    const double dwl = 1.e-10;

    std::pair<double, double> neno = GetRefractiveIndices(wavelength, mp);
    std::pair<double, double> neno_p = GetRefractiveIndices(wavelength + dwl, mp);
    std::pair<double, double> neno_m = GetRefractiveIndices(wavelength - dwl, mp);
    
    double biref = neno.first - neno.second;
    double biref_p = neno_p.first - neno_p.second;
    double biref_m = neno_m.first - neno_m.second;
    double biref_deriv = (biref_p - biref_m) / (2 * dwl);

    return 1 - (wavelength / biref) * biref_deriv;
}


/**
 * @brief 4-parameter Sellmeier equation
 * 
 * @param wl_um2 wavelength in microns, squared
 * @param A 
 * @param B 
 * @param C 
 * @param D 
 * @return double refractive index
 */
double SellmeierEqn(double wl_um2, double A, double B, double C, double D)
{
    return sqrt(A + (B / (wl_um2 + C)) + (D * wl_um2));
}


/**
 * @brief 5-parameter Sellmeier equation
 * 
 * @param wl_um2 wavelength in microns, squared
 * @param A 
 * @param B 
 * @param C 
 * @param D 
 * @param E 
 * @return double refractive index
 */
double SellmeierEqn(double wl_um2, double A, double B, double C, double D, double E)
{
    return sqrt(A + (B / (wl_um2 + C)) + (D / (wl_um2 + E)));
}


/**
 * @brief 6-parameter Sellmeier equation
 * 
 * @param wl_um2  wavelength in microns, squared
 * @param A 
 * @param B 
 * @param C 
 * @param D 
 * @param E 
 * @param F 
 * @return double refractive index
 */
double SellmeierEqn(double wl_um2, double A, double B, double C, double D, double E, double F)
{
    return sqrt((A * wl_um2 / (wl_um2 - B)) + (C * wl_um2 / (wl_um2 - D)) + (E * wl_um2 / (wl_um2 - F)) + 1);
}


} // namespace cispp