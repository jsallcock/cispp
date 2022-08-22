#include <string>
#include <iostream>
#include <cmath>
#include <Eigen/Dense>
#include "../include/component.h"
#include "../include/material.h"


/**
 * @brief Mueller matrix for frame rotation
 * @param angle angle of rotation in degrees, anti-clockwise from x-axis.
 * @return rotation matrix
 */
Eigen::Matrix4d rotation_matrix(double angle)
{
    double a2 = 2 * angle * M_PI / 180;
    double s2 = sin(a2);
    double c2 = cos(a2);
    Eigen::Matrix4d m;

    m << 1,   0,   0,   0, 
         0,  c2,  s2,   0,
         0, -s2,  c2,   0,
         0,   0,   0,   1;

    return m;
}


/**
 * @brief Calculate Mueller matrix for light ray
 * 
 * @param wavelength wavelength of light (metres)
 * @param inc_angle incidence angle of the light (radians)
 * @param azim_angle azimuthal angle of the light (radians)
 * @return Eigen::Matrix4d 
 */
Eigen::Matrix4d Polariser::get_mueller_matrix(double wavelength, double inc_angle, double azim_angle)
{
    double tx1_sq = pow(tx1,2);
    double tx2_sq = pow(tx2,2);
    double sum = (tx1_sq + tx2_sq) / 2;
    double diff = (tx1_sq - tx2_sq) / 2;
    double prod = tx1 * tx2;
    Eigen::Matrix4d m;

    m <<  sum, diff,    0,    0, 
         diff,  sum,    0,    0,
            0,    0, prod,    0,
            0,    0,    0, prod;

    Eigen::Matrix4d rotmat = rotation_matrix(orientation);
    return rotmat.transpose() * m * rotmat;
};


/**
* @brief Calculate Mueller matrix for light ray
* 
* @param wavelength wavelength of light (metres)
* @param inc_angle incidence angle of the light (radians)
* @param azim_angle azimuthal angle of the light (radians) 
* @return Matrix4d 
*/
Eigen::Matrix4d Retarder::get_mueller_matrix(double wavelength, double inc_angle, double azim_angle)
{
    double delay = get_delay(wavelength, inc_angle, azim_angle);
    double cdelay = contrast_inst * cos(delay);
    double sdelay = contrast_inst * sin(delay);

    Eigen::Matrix4d m;
    m << 1,       0,       0,       0,
         0,       1,       0,       0,
         0,       0,  cdelay,  sdelay,
         0,       0, -sdelay,  cdelay;

    Eigen::Matrix4d rotmat = rotation_matrix(orientation);
    return rotmat.transpose() * m * rotmat;
}


/**
* @brief Calculate interferometer delay in radians between O and E light rays
* 
* @param wavelength wavelength of light (metres)
* @param inc_angle incidence angle of the light (radians)
* @param azim_angle azimuthal angle of the light (radians)
* @return interferometer delay in radians
*/
double UniaxialCrystal::get_delay(double wavelength, double inc_angle, double azim_angle)
{
    std::pair<double, double> neno = get_refractive_indices(wavelength, material);
    double ne = neno.first; 
    double no = neno.second;

    double s_inc_angle = sin(inc_angle);
    double s_cut_angle = sin(cut_angle);
    double c_cut_angle = cos(cut_angle);
    double s_azim_angle = sin(azim_angle);
    double c_azim_angle = cos(azim_angle);
    double s_inc_angle2 = pow(s_inc_angle, 2);
    double s_cut_angle2 = pow(s_cut_angle, 2);
    double c_cut_angle2 = pow(c_cut_angle, 2);
    double no2 = pow(no, 2);
    double ne2 = pow(ne, 2);
    double p = ne2 * s_cut_angle2 + no2 * c_cut_angle2;

    double term_1 = sqrt(no2 - s_inc_angle2);
    double term_2 = (no2 - ne2) * (s_cut_angle * c_cut_angle * c_azim_angle * s_inc_angle) / p;
    double term_3 = - no * sqrt((ne2 * p) - ((ne2 - (ne2 - no2) * c_cut_angle2 * pow(s_azim_angle,2)) * s_inc_angle2)) / p;

    return 2 * M_PI * (thickness / wavelength) * (term_1 + term_2 + term_3);
}


/**
 * @brief test that two components have orientations that are either parallel or orthogonal
 * 
 * @param c1
 * @param c2 
 * @return true 
 * @return false 
 */
bool test_align90(std::unique_ptr<Component>& c1,  std::unique_ptr<Component>& c2)
{
    if (fmod(c1->orientation - c2->orientation, 90) == 0.){
        return true;
    }
    else {
        return false;
    }
}


// /**
//  * @brief test that two components have orientations that differ by 45 degrees
//  * 
//  * @param c1
//  * @param c2 
//  * @return true 
//  * @return false 
//  */
// bool test_align90(std::unique_ptr<Component>& c1,  std::unique_ptr<Component>& c2)
// {
//     if (fmod(c1->orientation - c2->orientation, 45) == 0.){
//         return true;
//     }
//     else {
//         return false;
//     }
// }