#include <string>
#include <iostream>
#include <cmath>
#include <Eigen/Dense>
#include "include/component.h"
#include "include/material.h"


namespace cispp {


Eigen::Matrix4d get_rotation_matrix(double angle)
{
    double a2 = 2 * angle;
    double s2 = sin(a2);
    double c2 = cos(a2);
    Eigen::Matrix4d m;

    m << 1,   0,   0,   0, 
         0,  c2,  s2,   0,
         0, -s2,  c2,   0,
         0,   0,   0,   1;

    return m;
}


Eigen::Matrix4d Component::get_mueller_matrix(double wavelength, double inc_angle, double azim_angle)
{
    double delay = get_delay(wavelength, inc_angle, azim_angle);
    double t1 = get_t1(wavelength, inc_angle, azim_angle);
    double t2 = get_t2(wavelength, inc_angle, azim_angle);
    double sum = (t1 + t2) / 2;
    double diff = (t1 - t2) / 2;
    double prod = 2 * sqrt(t1 * t2);
    double psd = prod * sin(delay);
    double pcd = prod * cos(delay);
    Eigen::Matrix4d m;

    m <<  sum, diff,    0,    0, 
         diff,  sum,    0,    0,
            0,    0,  pcd,  psd,
            0,    0, -psd,  pcd;

    Eigen::Matrix4d rot = get_rotation_matrix(orientation);
    return rot.transpose() * m * rot;
}


Eigen::Matrix4d Polariser::get_mueller_matrix()
{
    Eigen::Matrix4d m;
    m << 0.5, 0.5,   0,   0, 
         0.5, 0.5,   0,   0,
           0,   0,   0,   0,
           0,   0,   0,   0;

    Eigen::Matrix4d rot = get_rotation_matrix(orientation);
    return rot.transpose() * m * rot;
}


Eigen::Matrix4d Polariser::get_mueller_matrix(double wavelength, double inc_angle, double azim_angle) {
    return get_mueller_matrix();
}


Eigen::Matrix4d Retarder::get_mueller_matrix(double wavelength, double inc_angle, double azim_angle)
{
    double delay = get_delay(wavelength, inc_angle, azim_angle);
    double sd = sin(delay);
    double cd = cos(delay);
    Eigen::Matrix4d m;

    m <<   1,   0,   0,   0, 
           0,   1,   0,   0,
           0,   0,  cd,  sd,
           0,   0, -sd,  cd;

    Eigen::Matrix4d rot = get_rotation_matrix(orientation);
    return rot.transpose() * m * rot;
}


double UniaxialCrystal::get_delay(double wavelength, double inc_angle, double azim_angle)
{
    std::pair<double, double> neno = get_refractive_indices(wavelength, material);
    double ne = neno.first; 
    double no = neno.second;

    double s_inc = sin(inc_angle);
    double s_cut = sin(cut_angle);
    double c_cut = cos(cut_angle);
    double s_azim = sin(azim_angle);
    double c_azim = cos(azim_angle);
    double s_inc2 = pow(s_inc, 2);
    double s_cut2 = pow(s_cut, 2);
    double c_cut2 = pow(c_cut, 2);
    double no2 = pow(no, 2);
    double ne2 = pow(ne, 2);
    double p = ne2 * s_cut2 + no2 * c_cut2;

    double term_1 = sqrt(no2 - s_inc2);
    double term_2 = (no2 - ne2) * (s_cut * c_cut * c_azim * s_inc) / p;
    double term_3 = - no * sqrt((ne2 * p) - ((ne2 - (ne2 - no2) * c_cut2 * pow(s_azim,2)) * s_inc2)) / p;

    return 2 * M_PI * (thickness / wavelength) * (term_1 + term_2 + term_3);
}


/**
 * @brief test that two components have orientations that differ by 90 or 0 degrees
 * 
 * @param c1
 * @param c2 
 * @return true 
 * @return false 
 */
bool test_align90(std::unique_ptr<Component>& c1,  std::unique_ptr<Component>& c2)
{
    return abs(fmod(c1->orientation - c2->orientation, M_PI / 2)) == 0.;
}


/**
 * @brief test that two components have orientations that differ by +/- 45 degrees
 * 
 * @param c1
 * @param c2 
 * @return true 
 * @return false 
 */
bool test_align45(std::unique_ptr<Component>& c1,  std::unique_ptr<Component>& c2)
{
    return abs(fmod(c1->orientation - c2->orientation, M_PI / 2)) == M_PI / 4;
}


} // namespace cispp