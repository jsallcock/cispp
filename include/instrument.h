#pragma once

#include <memory>
#include <vector>
#include <string>
#include <stdexcept>
#include <cmath>
#include <Eigen/Dense>
#include <iostream>
#include <fstream>

#include "yaml-cpp/yaml.h"

#include "include/camera.h"
#include "include/component.h"

using std::vector;
using std::unique_ptr;
using std::string;


namespace cispp {


class Instrument
{
    public:
   
    string type = "mueller";
    double lens_1_focal_length;
    double lens_2_focal_length;
    double lens_3_focal_length;
    cispp::Camera camera;
    vector<unique_ptr<cispp::Component>> components;
    string fp_config;

    /**
     * @brief Construct Instrument from .YAML config file
     * 
     * @param fp_config 
     */
    Instrument(std::filesystem::path fp_config);

    virtual ~Instrument() = default;

    static cispp::Camera ParseNodeCamera(YAML::Node nd_camera);

    static void ParseNodeComponents(YAML::Node nd_components, vector<unique_ptr<cispp::Component>>& components);

    void write_config();

    /**
     * @brief Incidence angle in radians of ray through interferometer component
     * 
     * @param x x position on sensor plane in metres
     * @param y y position on sensor plane in metres
     * @param component unique pointer to component
     * @return double 
     */
    double GetIncidenceAngle(double x, double y, unique_ptr<cispp::Component>& component);

    /**
     * @brief Azimuthal angle in radians of ray through interfereometer component
     * 
     * @param x x position on sensor plane in metres
     * @param y y position on sensor plane in metres
     * @param component unique pointer to component
     * @return double 
     */
    double GetAzimuthalAngle(double x, double y, unique_ptr<cispp::Component>& component);

    /**
     * @brief Total Mueller matrix for instrument
     * 
     * @param x x position on sensor plane in metres
     * @param y y position on sensor plane in metres
     * @param wavelength wavelength of ray
     * @return Eigen::Matrix4d 
     */
    Eigen::Matrix4d GetMuellerMatrix(double x, double y, double wavelength);

    /**
     * @brief save Captured image to .PGM (portable GrayMap) file
     * 
     * @param fpath filepath (.PPM)
     * @param image pointer to image vector (row-major order)
     */
    void SaveImage(string fpath, vector<unsigned short int>* image);

    void GetDelay();

    /**
     * @brief Capture interferogram for a uniform scene of monochromatic, unpolarised light (Mueller model)
     * 
     * @param wavelength wavelength of light in metres
     * @param flux photon flux
     * @param image pointer to image vector (row-major order)
     */
    virtual void Capture(double wavelength, double flux, vector<unsigned short int>* image);

    /**
     * @brief Capture interferogram for a uniform scene of monochromatic, unpolarised light (Mueller model)
     * 
     * @param wavelength wavelength of light in metres
     * @param flux photon flux
     * @param image pointer to image vector (row-major order)
     */
    virtual void Capture(vector<double>& wavelength, vector<double>& spec_flux, vector<unsigned short int>* image);
};


class InstrumentSingleDelayLinear: public Instrument
{
    public:

    InstrumentSingleDelayLinear(std::filesystem::path fp_config)
    : Instrument(fp_config)
    {
        type = "single_delay_linear";
    }

    static bool TestType(const YAML::Node node);

    void Capture(double wavelength, double flux, vector<unsigned short int>* image) override;

    void Capture(vector<double>& wavelength, vector<double>& spec_flux, vector<unsigned short int>* image) override;
};


class InstrumentSingleDelayPixelated: public Instrument
{
    public:

    InstrumentSingleDelayPixelated(std::filesystem::path fp_config)
    : Instrument(fp_config)
    {
        type = "single_delay_pixelated";
    }

    static bool TestType(const YAML::Node node);

    void Capture(double wavelength, double flux, vector<unsigned short int>* image) override;

    void Capture(vector<double>& wavelength, vector<double>& spec_flux, vector<unsigned short int>* image) override;
};

/**
 * @brief factory method for loading a CIS instrument
 * 
 * @param fp_config 
 * @param force_mueller 
 * @return Instrument 
 */
unique_ptr<cispp::Instrument> LoadInstrument(std::filesystem::path fp_config, bool force_mueller=false);


} // namespace cispp
