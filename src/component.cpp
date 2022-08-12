#include <iostream>
#include <cmath>

/* Definitions of interferometer components */


double calc_delay_uniaxial_crystal(
  double wavelength,  // of the light ray (metres)
  double inc_angle,   // incidence angle of the light ray (radians)
  double azim_angle,  // azimuthal angle of the light ray (radians)
  double ne,          // extraordinary refractive index of material at wavelength
  double no,          // ordinary refractive index of material at wavelength
  double cut_angle,   // crystal cut angle (radians)
  double thickness    // crystal thickness (metres)
  )
{
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
  double term_2 = (no2 - ne2) * (sin(cut_angle) * c_cut_angle * c_azim_angle * s_inc_angle) / p;
  double term_3 = - no * sqrt((ne2 * p) - ((ne2 - (ne2 - no2) * c_cut_angle2 * pow(s_azim_angle,2)) * s_inc_angle2)) / p;

  return 2 * M_PI * (thickness / wavelength) * (term_1 + term_2 + term_3);
}


int main()
{
  double wavelength = 465e-9; 
  double inc_angle = 0.; 
  double azim_angle = 0.; 
  double ne = 1.2; 
  double no = 1.4;
  double cut_angle = 45 * M_PI / 180; 
  double thickness = 1e-2;
  double delay = calc_delay_uniaxial_crystal(wavelength, inc_angle, azim_angle, ne, no, cut_angle, thickness);
  std::cout << delay << std::endl;
}