
#ifndef COMPONENT_H
#define COMPONENT_H
#include <iostream>
#include <cmath>
#include <Eigen/Dense>

using Eigen::Matrix;


Matrix<double,4,4> rotation_matrix(double angle);


/**
 * @brief Linear polariser
 */
class Polariser
{
    public:
    double orientation {0};
    double tx1 {1};
    double tx2 {0};

    Polariser(double orientation)
    : orientation(orientation)
    {}

    Polariser(double orientation, double tx1, double tx2)
    : orientation(orientation), tx1(tx1), tx2(tx2)
    {}

    Matrix<double,4,4> get_mueller_matrix();
};


/**
 * @brief Linear retarder base class
 */
class Retarder
{
    public:
    double orientation {0};
    double contrast_inst {1};
    double tilt_x {0};
    double tilt_y {0};

    Retarder()
    {}

    Retarder(double orientation)
    : orientation(orientation)
    {}

    Retarder(double orientation, double contrast_inst)
    : orientation(orientation), contrast_inst(contrast_inst)
    {}

    Retarder(double orientation, double contrast_inst, double tilt_x, double tilt_y)
    : orientation(orientation), contrast_inst(contrast_inst), tilt_x(tilt_x), tilt_y(tilt_y)
    {}
};


/**
 * @brief Plane-parallel, uniaxial, birefringent crystal plate
 */
class UniaxialCrystal: public Retarder
{
    double cut_angle;
    double thickness;
    std::string material;
    double ne = 1.2;
    double no = 1.4;
    double sellAe;
    double sellBe;

    public:

    UniaxialCrystal(double thickness, double cut_angle, std::string material)
    : thickness(thickness), cut_angle(cut_angle), material(material)
    {}
    
    double get_delay(double wavelength, double inc_angle, double azim_angle);
    Matrix<double,4,4> get_mueller_matrix(double wavelength, double inc_angle, double azim_angle);
};

#endif