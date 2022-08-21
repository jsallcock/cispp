#include <string>
#include <iostream>
#include <cmath>
#include <stdexcept>
#include "yaml-cpp/yaml.h"
#include "../include/material.h"


/**
 * @brief get material properties by string
 * 
 * @return MaterialProperties 
 */
MaterialProperties get_material_properties(std::string material_name)
{
    MaterialProperties matprop = {};
    matprop.name = material_name;
    // YAML::Node data = YAML::LoadFile("../data/material.yaml");
    YAML::Node data = YAML::LoadFile("/Users/jsallcock/fusion/ci/cispp/data/material.yaml");

    bool success = true;

    if (!data[material_name]["sellmeier_coefficients"]){
        throw std::logic_error("invalid material_name");
    }
  
    try{
        matprop.sellAe = data[material_name]["sellmeier_coefficients"][0]["Ae"].as<double>();
    }
    catch(YAML::TypedBadConversion<double>){
        throw std::logic_error("invalid material_name");
    }


                matprop.sellBe = data[material_name]["sellmeier_coefficients"][0]["Be"].as<double>();
                matprop.sellCe = data[material_name]["sellmeier_coefficients"][0]["Ce"].as<double>();
                matprop.sellDe = data[material_name]["sellmeier_coefficients"][0]["De"].as<double>();
                matprop.sellEe = data[material_name]["sellmeier_coefficients"][0]["Ee"].as<double>();
                matprop.sellFe = data[material_name]["sellmeier_coefficients"][0]["Fe"].as<double>();
                matprop.sellAo = data[material_name]["sellmeier_coefficients"][0]["Ao"].as<double>();
                matprop.sellBo = data[material_name]["sellmeier_coefficients"][0]["Bo"].as<double>();
                matprop.sellCo = data[material_name]["sellmeier_coefficients"][0]["Co"].as<double>();
                matprop.sellDo = data[material_name]["sellmeier_coefficients"][0]["Do"].as<double>();
                matprop.sellEo = data[material_name]["sellmeier_coefficients"][0]["Eo"].as<double>();
                break;
            }
        else 
        {   
            success = false;
            break;
        } 
    }
    if (success == false){
        throw std::logic_error("invalid material_name");
    }
   
    return matprop;
}


void get_refractive_index_e(double wavelength, std::string material)
{}

void get_refractive_index_o(double wavelength, std::string material)
{}

/**
 * @brief 
 * 
 * @param wl_um 
 * @param sellA 
 * @param sellB 
 * @param sellC 
 * @param sellD 
 * @return double 
 */
double sellmeier_eqn(double wl_um, double A, double B, double C, double D)
{
    double wl_um2 = pow(wl_um, 2);
    return sqrt(A + (B / (wl_um2 + C)) + (D * wl_um2));
}


/**
 * @brief 
 * 
 * @param wl_um 
 * @param sellA 
 * @param sellB 
 * @param sellC 
 * @param sellD 
 * @return double 
 */
double sellmeier_eqn(double wl_um, double A, double B, double C, double D, double E)
{
    double wl_um2 = pow(wl_um, 2);
    return sqrt(A + (B / (wl_um2 + C)) + (D / (wl_um2 + E)));
}


/**
 * @brief 
 * 
 * @param wl_um 
 * @param A 
 * @param B 
 * @param C 
 * @param D 
 * @param E 
 * @param F 
 * @return double 
 */
double sellmeier_eqn(double wl_um, double A, double B, double C, double D, double E, double F)
{
    double wl_um2 = pow(wl_um, 2);
    return sqrt((A * wl_um2 / (wl_um2 - B)) + (C * wl_um2 / (wl_um2 - D)) + (E * wl_um2 / (wl_um2 - F)) + 1);
}

