
#ifndef CISPP_COMPONENT_H
#define CISPP_COMPONENT_H
#include <string>
#include <iostream>
#include <cmath>
#include <Eigen/Dense>
#include "include/material.h"


namespace cispp {

/**
 * @brief Mueller matrix for frame rotation
 * @param angle angle of rotation in radians, anti-clockwise from x-axis.
 * @return rotation matrix
 */
Eigen::Matrix4d get_rotation_matrix(double angle);


/**
 * @brief Interferometer component (abstract base class)
 * 
 * Base component is a homogeneous diattenuating retarder whose key properties (retardance [delay], x-transmittance, 
 * y-transmittance) have arbitrary dependence on wavelength / ray incidence angle / ray azimuthal angle.
 */
class Component
{
    public:

    double orientation {0}; 
    double tilt_x {0};
    double tilt_y {0};

    Component()
    {}

    Component(double orientation)
    : orientation(orientation)
    {}

    Component(double orientation, double tilt_x, double tilt_y)
    : orientation(orientation),
      tilt_x(tilt_x),
      tilt_y(tilt_y)
    {}

    virtual ~Component() = default;

    /**
     * @brief Transmittance for light ray linearly polarised in alignment with component axis
     * 
     * @param wavelength wavelength of light (metres)
     * @param inc_angle incidence angle of light (radians)
     * @param azim_angle azimuthal angle of light (radians)
     * @return double 
     */
    virtual double get_t1(double wavelength, double inc_angle, double azim_angle) = 0;

    /**
     * @brief Transmittance for light ray linearly polarised orthogonal to component axis
     * 
     * @param wavelength wavelength of light ray (metres)
     * @param inc_angle incidence angle of light ray (radians)
     * @param azim_angle azimuthal angle of light ray (radians)
     * @return double 
     */
    virtual double get_t2(double wavelength, double inc_angle, double azim_angle) = 0;

    /**
     * @brief Retardance in radians
     * 
     * @param wavelength wavelength of light ray (metres)
     * @param inc_angle incidence angle of light ray (radians)
     * @param azim_angle azimuthal angle of light ray (radians)
     * @return double 
     */
    virtual double get_delay(double wavelength, double inc_angle, double azim_angle) = 0;

    /**
    * @brief Calculate Mueller matrix for light ray
    * 
    * @param wavelength wavelength of light ray (metres)
    * @param inc_angle incidence angle of light ray (radians)
    * @param azim_angle azimuthal angle of light ray (radians) 
    * @return Matrix4d 
    */
    virtual Eigen::Matrix4d get_mueller_matrix(double wavelength, double inc_angle, double azim_angle);

    virtual bool is_polariser() {
        return false;
    }

    virtual bool is_retarder() {
        return false;
    }

    virtual bool is_quarterwaveplate() {
        return false;
    }
};


/**
 * @brief Ideal linear polariser
 *
 * Behaviour is independent of wavelength, ray incidence angle and ray azimuthal angle
 */
class Polariser: public Component
{
    public:

    Polariser()
    : Component()
    {}

    Polariser(double orientation)
    : Component(orientation)
    {}

    Polariser(double orientation, double tilt_x, double tilt_y)
    : Component(orientation, tilt_x, tilt_y)
    {}

    double get_t1(double wavelength, double inc_angle, double azim_angle) override {
        return 1;
    }

    double get_t2(double wavelength, double inc_angle, double azim_angle) override {
        return 0;
    }

    double get_delay(double wavelength, double inc_angle, double azim_angle) override {
        return 0;
    }

    Eigen::Matrix4d get_mueller_matrix(double wavelength, double inc_angle, double azim_angle) override;

    Eigen::Matrix4d get_mueller_matrix();

    bool is_polariser() override {
        return true;
    }
};


/**
 * @brief Ideal linear retarder (abstract base class)
 */
class Retarder: public Component
{
    public:

    Retarder()
    : Component()
    {}

    Retarder(double orientation)
    : Component(orientation)
    {}

    Retarder(double orientation, double tilt_x, double tilt_y)
    : Component(orientation, tilt_x, tilt_y)
    {}

    double get_t1(double wavelength, double inc_angle, double azim_angle) override {
        return 1;
    }

    double get_t2(double wavelength, double inc_angle, double azim_angle) override {
        return 1;
    }

    Eigen::Matrix4d get_mueller_matrix(double wavelength, double inc_angle, double azim_angle) override;

    bool is_retarder() override {
        return true;
    }

    void get_fringe_frequency(double )
};


/**
 * @brief Ideal quarter waveplate
 * 
 */
class QuarterWaveplate: public Retarder
{
    public: 

    QuarterWaveplate()
    : Retarder()
    {}

    QuarterWaveplate(double orientation)
    : Retarder(orientation)
    {}

    QuarterWaveplate(double orientation, double tilt_x, double tilt_y)
    : Retarder(orientation, tilt_x, tilt_y)
    {}

    double get_delay(double wavelength, double inc_angle, double azim_angle) override {
        return M_PI / 2;
    }

    bool is_quarterwaveplate() override {
        return true;
    }
};


/**
 * @brief Ideal half waveplate
 * 
 */
class HalfWaveplate: public Retarder
{
    public: 

    HalfWaveplate()
    : Retarder()
    {}

    HalfWaveplate(double orientation)
    : Retarder(orientation)
    {}

    HalfWaveplate(double orientation, double tilt_x, double tilt_y)
    : Retarder(orientation, tilt_x, tilt_y)
    {}

    double get_delay(double wavelength, double inc_angle, double azim_angle) override {
        return M_PI;
    }
};


/**
 * @brief Plane-parallel, uniaxial, birefringent crystal plate
 *
 */
class UniaxialCrystal: public Retarder
{
    public:

    double thickness;
    double cut_angle;
    MaterialProperties material{};

    /**
    * @brief Constructor specifying material properties by material name
    * 
    * @param orientation 
    * @param thickness plate thickness (metres)
    * @param cut_angle plate cut angle (radians)
    * @param material_name 
    */
    UniaxialCrystal
    (
        double orientation,
        double tilt_x,
        double tilt_y,
        double thickness, 
        double cut_angle,
        std::string material_name
    )
    : Retarder(orientation, tilt_x, tilt_y), 
      thickness(thickness), 
      cut_angle(cut_angle),
      material(get_material_properties(material_name))
    {}


    /**
     * @brief Constructor specifying material properties by MaterialProperties struct
     * 
     * @param orientation 
     * @param thickness plate thickness (metres)
     * @param cut_angle plate cut angle (radians)
     * @param material_properties 
     */
    UniaxialCrystal
    (
        double orientation,
        double tilt_x,
        double tilt_y,
        double thickness, 
        double cut_angle,
        MaterialProperties material_properties
    )
    : Retarder(orientation, tilt_x, tilt_y), 
      thickness(thickness), 
      cut_angle(cut_angle),
      material(material_properties)
    {}
    
    double get_delay(double wavelength, double inc_angle, double azim_angle) override;
};


/**
 * @brief Savart plate
 * 
 */
class SavartPlate: public Retarder
{
    double get_delay(double wavelength, double inc_angle, double azim_angle) override;
};


bool test_align90(std::unique_ptr<Component>& c1,  std::unique_ptr<Component>& c2);

bool test_align45(std::unique_ptr<Component>& c1,  std::unique_ptr<Component>& c2);


} // namespace cispp
#endif