
#ifndef COMPONENT_H
#define COMPONENT_H
#include <string>
#include <iostream>
#include <cmath>
#include <Eigen/Dense>


Eigen::Matrix4d rotation_matrix(double angle);


class Component
{
    public:
    double orientation {0};

    Component(double orientation)
    : orientation(orientation)
    {}

    virtual Eigen::Matrix4d get_mueller_matrix(double wavelength, double inc_angle, double azim_angle) = 0;

    virtual ~Component() = default;
};

/**
 * @brief Linear polariser
 */
class Polariser: public Component
{
    public:
    double tx1 {1};
    double tx2 {0};

    Polariser(double orientation)
    : Component(orientation)
    {}

    Polariser(double orientation, double tx1, double tx2)
    : Component(orientation), tx1(tx1), tx2(tx2)
    {}

    Eigen::Matrix4d get_mueller_matrix(double wavelength, double inc_angle, double azim_angle);
};


/**
 * @brief Linear retarder base class
 */
class Retarder: public Component
{
    public:
    double contrast_inst {1};
    double tilt_x {0};
    double tilt_y {0};

    Retarder(double orientation)
    : Component(orientation)
    {}

    Retarder(double orientation, double contrast_inst)
    : Component(orientation), contrast_inst(contrast_inst)
    {}

    Retarder(double orientation, double contrast_inst, double tilt_x, double tilt_y)
    : Component(orientation), contrast_inst(contrast_inst), tilt_x(tilt_x), tilt_y(tilt_y)
    {}
};


/**
 * @brief Idealised waveplate, imparting constant delay regardless of ray wavelength or ray path. 
 */
class IdealWaveplate: public Retarder
{
    double delay;

    public:

    IdealWaveplate(double orientation, double delay)
    : Retarder(orientation), delay(delay)
    {}
    
    double get_delay(){
        return delay;
    }

    Eigen::Matrix4d get_mueller_matrix(double wavelength, double inc_angle, double azim_angle);
};


/**
 * @brief Plane-parallel, uniaxial, birefringent crystal plate
 */
class UniaxialCrystal: public Retarder
{
    double thickness;
    double cut_angle;
    std::string material;
    double ne = 1.2;
    double no = 1.4;
    double sellAe;
    double sellBe;

    public:

    UniaxialCrystal(double orientation, double thickness, double cut_angle, std::string material)
    : Retarder(orientation), 
      thickness(thickness), 
      cut_angle(cut_angle), 
      material(material)
    {}
    
    double get_delay(double wavelength, double inc_angle, double azim_angle);
    Eigen::Matrix4d get_mueller_matrix(double wavelength, double inc_angle, double azim_angle);
};



#endif