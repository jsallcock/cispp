#ifndef INSTRUMENT_H
#define INSTRUMENT_H
#include "yaml-cpp/yaml.h"
#include "../include/camera.h"


class Instrument
{
    // Camera camera;

    std::string type = "Mueller";

    public:
    double lens_1_focal_length;
    double lens_2_focal_length;
    double lens_3_focal_length;
    Camera camera;
    std::string fp_config;

    /**
     * @brief Construct new Instrument object from .YAML config file
     * 
     * @param fp_config 
     */
    Instrument(std::string fp_config)
    : fp_config(fp_config)
    {
        YAML::Node config = YAML::LoadFile(fp_config);

        lens_1_focal_length = config["lens_1_focal_length"].as<double>();
        lens_2_focal_length = config["lens_2_focal_length"].as<double>();
        lens_3_focal_length = config["lens_3_focal_length"].as<double>();

        camera = Camera(
            config["camera"]["sensor_format"][0].as<int>(),  // sensor_format_x
            config["camera"]["sensor_format"][1].as<int>(),  // sensor_format_y
            config["camera"]["pixel_size"].as<double>(),
            config["camera"]["bit_depth"].as<int>(),
            config["camera"]["qe"].as<double>(),
            config["camera"]["epercount"].as<double>(),
            config["camera"]["cam_noise"].as<double>()
        );

        
    }

    void read_config(){}
    void write_config(){}
    void get_inc_angle(){}
    void get_azim_angle(){}
    void get_mueller_matrix(){}
    void get_instrument_type(){}
    void get_delay(){}
    void capture(){}

};

#endif