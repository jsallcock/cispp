#ifndef CISPP_INSTRUMENT_H
#define CISPP_INSTRUMENT_H
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
    Instrument(std::string fp_config);

    void write_config();

    /**
     * @brief Incidence angle in radians of ray through interferometer component
     * 
     * @param x x position on sensor plane in metres
     * @param y y position on sensor plane in metres
     * @param component unique pointer to component
     * @return double 
     */
    double get_inc_angle(double x, double y, unique_ptr<cispp::Component>& component);

    /**
     * @brief Azimuthal angle in radians of ray through interfereometer component
     * 
     * @param x x position on sensor plane in metres
     * @param y y position on sensor plane in metres
     * @param component unique pointer to component
     * @return double 
     */
    double get_azim_angle(double x, double y, unique_ptr<cispp::Component>& component);

    /**
     * @brief Total Mueller matrix for instrument
     * 
     * @param x x position on sensor plane in metres
     * @param y y position on sensor plane in metres
     * @param wavelength wavelength of ray
     * @return Eigen::Matrix4d 
     */
    Eigen::Matrix4d get_mueller_matrix(double x, double y, double wavelength);

    /**
     * @brief save captured image to .PPM image file
     * 
     * @param fpath filepath (.PPM)
     * @param image pointer to image vector (row-major order)
     */
    void save_image(string fpath, vector<unsigned short int>* image);

    void get_delay();


    /**
     * @brief Determine instrument type based on configuraiton
     * 
     * @return std::string 
     */
    std::string get_type();

    bool test_type_single_delay_linear();

    bool test_type_single_delay_pixelated();

    // ca

    void capture(double wavelength, double flux, vector<unsigned short int>* image);

    void capture_mueller(double wavelength, double flux, vector<unsigned short int>* image);

    void capture_single_delay_linear(double wavelength, double flux, vector<unsigned short int>* image);

    void capture_single_delay_pixelated(double wavelength, double flux, vector<unsigned short int>* image);
};


} // namespace cispp
#endif