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

using std::vector;
using std::unique_ptr;
using std::string;


class Instrument
{

    public:
   
    string type = "mueller";
    double lens_1_focal_length;
    double lens_2_focal_length;
    double lens_3_focal_length;
    Camera camera;
    vector<unique_ptr<Component>> interferometer;
    string fp_config;

    Instrument(std::string fp_config);

    void write_config();

    double get_inc_angle(double x, double y, unique_ptr<Component>& component);

    double get_azim_angle(double x, double y, unique_ptr<Component>& component);

    Eigen::Matrix4d get_mueller_matrix(double x, double y, double wavelength);

    void image_to_file(string fpath, vector<unsigned short int>* image);

    void get_delay();

    std::string get_type();

    bool test_type_single_delay_linear();

    bool test_type_single_delay_pixelated();

    void capture(double wavelength, double flux, vector<unsigned short int>* image);

    void capture_mueller(double wavelength, double flux, vector<unsigned short int>* image);

    void capture_single_delay_linear(double wavelength, double flux, vector<unsigned short int>* image);

    void capture_single_delay_pixelated(double wavelength, double flux, vector<unsigned short int>* image);
};

#endif