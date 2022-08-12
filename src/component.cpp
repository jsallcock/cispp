#include <iostream>
#include <cmath>
#include <Eigen/Dense>

using Eigen::MatrixXd;
/* Definitions of interferometer components */


/**
 * @brief Mueller matrix for frame rotation
 * @param angle angle of rotation in radians, anti-clockwise from x-axis.
 * @return rotation matrix
 */
MatrixXd rotation_matrix(double angle)
{
    double a2 = 2 * angle;
    double s2 = sin(a2);
    double c2 = cos(a2);
    MatrixXd m(4,4);

    m << 1,   0,   0,   0, 
         0,  c2,  s2,   0,
         0, -s2,  c2,   0,
         0,   0,   0,   1;

    return m;
}

/**
 * @brief Linear polariser
 */
class Polariser
{
    double orientation;
    double tx1 {1};
    double tx2 {0};

    public:

    Polariser(double orientation)
    : orientation(orientation)
    {}

    Polariser(double orientation, double tx1, double tx2)
    : orientation(orientation), tx1(tx1), tx2(tx2)
    {}

    MatrixXd get_mueller_matrix()
    {
        double tx1_sq = pow(tx1,2);
        double tx2_sq = pow(tx2,2);
        double sum = (tx1_sq + tx2_sq) / 2;
        double diff = (tx1_sq - tx2_sq) / 2;
        double prod = tx1 * tx2;
        MatrixXd m(4,4);

        m <<  sum, diff,    0,    0, 
             diff,  sum,    0,    0,
                0,    0, prod,    0,
                0,    0,    0, prod;

        return rotation_matrix() * m * rotation_matrix();
    }
};


// class Retarder
// {
//     double orientation;

//     public:
//     MatrixXd get_mueller_matrix()
//     {

//     }
// }


/**
 * @brief 
 * 
 * @param wavelength wavelength of light ray (metres)
 * @param inc_angle incidence angle of the light ray (radians)
 * @param azim_angle azimuthal angle of the light ray (radians)
 * @param ne extraordinary refractive index of material at wavelength
 * @param no ordinary refractive index of material at wavelength
 * @param cut_angle crystal cut angle (radians)
 * @param thickness crystal thickness (metres)
 * @return double 
 */
double calc_delay_uniaxial_crystal(
    double wavelength, 
    double inc_angle, 
    double azim_angle, 
    double ne, 
    double no, 
    double cut_angle, 
    double thickness
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
    // just testing for now
    // double wavelength = 465e-9; 
    // double inc_angle = 0.; 
    // double azim_angle = 0.; 
    // double ne = 1.2; 
    // double no = 1.4;
    // double cut_angle = 45 * M_PI / 180; 
    // double thickness = 1e-2;
    // double delay = calc_delay_uniaxial_crystal(wavelength, inc_angle, azim_angle, ne, no, cut_angle, thickness);
    // std::cout << delay << std::endl;

    MatrixXd m = rotation_matrix(0);
    Polariser p = Polariser(0);
    std::cout << m << std::endl;
    std::cout << p.get_mueller_matrix() << std::endl;
}