#include <string>
#include <iostream>
#include <cmath>
#include <Eigen/Dense>
#include "include/component.h"
#include "include/material.h"


namespace cispp {


Eigen::Matrix4d GetRotationMatrix(double angle)
{
    const double s = sin(2 * angle);
    const double c = cos(2 * angle);

    Eigen::Matrix4d m;
    m << 1,  0,  0,  0, 
         0,  c,  s,  0,
         0, -s,  c,  0,
         0,  0,  0,  1;

    return m;
}


Eigen::Matrix4d Component::GetMuellerMatrix(double wavelength, double incidence_angle, double azimuthal_angle)
{
    const double delay = GetDelay(wavelength, incidence_angle, azimuthal_angle);
    const double t1 = GetT1(wavelength, incidence_angle, azimuthal_angle);
    const double t2 = GetT2(wavelength, incidence_angle, azimuthal_angle);
    const double sum = (t1 + t2) / 2;
    const double diff = (t1 - t2) / 2;
    const double sind = 2 * sqrt(t1 * t2) * sin(delay);
    const double cosd = 2 * sqrt(t1 * t2) * cos(delay);

    Eigen::Matrix4d m;
    m <<   sum,  diff,     0,     0, 
          diff,   sum,     0,     0,
             0,     0,  cosd,  sind,
             0,     0, -sind,  cosd;

    Eigen::Matrix4d rot = GetRotationMatrix(orientation);
    return rot.transpose() * m * rot;
}


Eigen::Matrix4d Polariser::GetMuellerMatrix()
{
    Eigen::Matrix4d m;
    m << 0.5, 0.5,   0,   0, 
         0.5, 0.5,   0,   0,
           0,   0,   0,   0,
           0,   0,   0,   0;

    Eigen::Matrix4d rot = GetRotationMatrix(orientation);
    return rot.transpose() * m * rot;
}


Eigen::Matrix4d Polariser::GetMuellerMatrix(double wavelength, double incidence_angle, double azimuthal_angle) {
    return GetMuellerMatrix();
}


Eigen::Matrix4d Retarder::GetMuellerMatrix(double wavelength, double incidence_angle, double azimuthal_angle)
{
    double delay = GetDelay(wavelength, incidence_angle, azimuthal_angle);
    double s = sin(delay);
    double c = cos(delay);
    Eigen::Matrix4d m;

    m <<   1,  0,  0,  0, 
           0,  1,  0,  0,
           0,  0,  c,  s,
           0,  0, -s,  c;

    Eigen::Matrix4d rot = GetRotationMatrix(orientation);
    return rot.transpose() * m * rot;
}


double UniaxialCrystal::GetDelay(double wavelength, double incidence_angle, double azimuthal_angle)
{
    std::pair<double, double> neno = GetRefractiveIndices(wavelength, material);
    const double ne = neno.first; 
    const double no = neno.second;
    const double s_inc = sin(incidence_angle);
    const double s_cut = sin(cut_angle);
    const double c_cut = cos(cut_angle);
    const double s_azim = sin(azimuthal_angle);
    const double c_azim = cos(azimuthal_angle);
    const double s_inc2 = pow(s_inc, 2);
    const double s_cut2 = pow(s_cut, 2);
    const double c_cut2 = pow(c_cut, 2);
    const double no2 = pow(no, 2);
    const double ne2 = pow(ne, 2);
    const double p = ne2 * s_cut2 + no2 * c_cut2;

    const double term_1 = sqrt(no2 - s_inc2);
    const double term_2 = (no2 - ne2) * (s_cut * c_cut * c_azim * s_inc) / p;
    const double term_3 = - no * sqrt((ne2 * p) - ((ne2 - (ne2 - no2) * c_cut2 * pow(s_azim,2)) * s_inc2)) / p;

    return 2 * M_PI * (thickness / wavelength) * (term_1 + term_2 + term_3);
}


bool TestAlign90(std::unique_ptr<Component>& c1,  std::unique_ptr<Component>& c2)
{
    return std::abs(fmod(c1->orientation - c2->orientation, M_PI / 2)) == 0.;
}


bool TestAlign45(std::unique_ptr<Component>& c1,  std::unique_ptr<Component>& c2)
{
    return std::abs(fmod(c1->orientation - c2->orientation, M_PI / 2)) == M_PI / 4;
}


} // namespace cispp