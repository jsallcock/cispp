#ifndef INSTRUMENT_H
#define INSTRUMENT_H
#include <memory>
#include <vector>
#include <string>
#include <stdexcept>
#include <cmath>
#include <Eigen/Dense>
#include <iostream>
#include <fstream>
#include "yaml-cpp/yaml.h"
#include "../include/camera.h"
#include "../include/component.h"


class Instrument
{
    std::string type = "Mueller";

    public:
   
    double lens_1_focal_length;
    double lens_2_focal_length;
    double lens_3_focal_length;
    Camera camera;
    std::vector<std::unique_ptr<Component>> interferometer;
    std::string fp_config;

    Instrument(std::string fp_config);

    void write_config();

    double get_inc_angle(double x, double y, std::unique_ptr<Component>& component);

    double get_azim_angle(double x, double y, std::unique_ptr<Component>& component);

    Eigen::Matrix4d get_mueller_matrix(double x, double y, double wavelength);

    void capture(double wavelength, double flux);

    void get_instrument_type();

    void get_delay();
};

#endif