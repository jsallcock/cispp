
#ifndef CISPP_COMPONENT_H
#define CISPP_COMPONENT_H
#include <string>
#include <iostream>
#include <cmath>
#include <Eigen/Dense>
#include "include/material.h"


namespace cispp {


Eigen::Matrix4d get_rotation_matrix(double angle);

/**
 * @brief abstract base class for interferometer component
 * 
 */
class Component
{
    public:

    std::string name;
    double orientation {0};
    
    /**
     * @brief Construct a new Component object
     * 
     * @param orientation 
     */
    Component(double orientation)
    : orientation(orientation)
    {}

    virtual ~Component() = default;

    virtual Eigen::Matrix4d get_mueller_matrix(double wavelength, double inc_angle, double azim_angle) = 0;

    virtual double get_delay(double wavelength, double inc_angle, double azim_angle) = 0;
};


/**
 * @brief Linear (partial) polariser
 */
class Polariser: public Component
{
    public:

    double tx1 {1};
    double tx2 {0};

    Polariser(double orientation)
    : Component(orientation)
    {
        name = "Polariser";
    }

    Polariser(double orientation, double tx1, double tx2)
    : Component(orientation), 
      tx1(tx1), 
      tx2(tx2)
    {
        name = "Polariser";
    }

    Eigen::Matrix4d get_mueller_matrix(double wavelength, double inc_angle, double azim_angle) override;

    Eigen::Matrix4d get_mueller_matrix();

    double get_delay(double wavelength, double inc_angle, double azim_angle) override;
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
    {
        name = "Retarder";
    }

    Retarder(double orientation, double contrast_inst)
    : Component(orientation), contrast_inst(contrast_inst)
    {
        name = "Retarder";
    }

    Retarder(double orientation, double contrast_inst, double tilt_x, double tilt_y)
    : Component(orientation), contrast_inst(contrast_inst), tilt_x(tilt_x), tilt_y(tilt_y)
    {
        name = "Retarder";
    }

    // virtual double get_delay(double wavelength, double inc_angle, double azim_angle) = 0;

    Eigen::Matrix4d get_mueller_matrix(double wavelength, double inc_angle, double azim_angle) override;
};


/**
 * @brief Ideal waveplate imparts same delay regardless of ray wavelength or ray path. 
 */
class IdealWaveplate: public Retarder
{
    public:

    double delay;

    IdealWaveplate(double orientation, double delay)
    : Retarder(orientation), 
      delay(delay)
    {}
    
    double get_delay(double wavelength, double inc_angle, double azim_angle) override 
    {
        return delay;
    }
};

/**
 * @brief Ideal quarter waveplate
 * 
 */
class QuarterWaveplate: public IdealWaveplate
{
    public: 

    QuarterWaveplate(double orientation)
    : IdealWaveplate(orientation, M_PI / 2)
    {
        name = "QuarterWaveplate";
    }
};


/**
 * @brief Ideal half waveplate
 * 
 */
class HalfWaveplate: public IdealWaveplate
{
    public:
    
    HalfWaveplate(double orientation)
    : IdealWaveplate(orientation, M_PI)
    {
        name = "HalfWaveplate";
    }
};


/**
 * @brief Plane-parallel, uniaxial, birefringent crystal plate
 */
class UniaxialCrystal: public Retarder
{
    // privately store refractive indices calculated for the previous wavelength
    // avoids unnecessary re-calculation
    // double wavelength_last;
    // double ne_last;
    // double no_last;
    // actually this gave no performance benefit

    public:

    double thickness;
    double cut_angle;
    MaterialProperties material{};

    /**
    * @brief Constructor specifying material properties by material name
    * 
    * @param orientation 
    * @param thickness 
    * @param cut_angle 
    * @param material_name 
    */
    UniaxialCrystal
    (
        double orientation, 
        double thickness, 
        double cut_angle,
        std::string material_name
    )
    : Retarder(orientation), 
    thickness(thickness), 
    cut_angle(cut_angle * M_PI / 180),
    material(get_material_properties(material_name))
    {}
    
    double get_delay(double wavelength, double inc_angle, double azim_angle) override;
};


bool test_align90(std::unique_ptr<Component>& c1,  std::unique_ptr<Component>& c2);

bool test_align45(std::unique_ptr<Component>& c1,  std::unique_ptr<Component>& c2);


} // namespace cispp
#endif