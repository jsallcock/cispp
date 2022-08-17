#ifndef INSTRUMENT_H
#define INSTRUMENT_H
#include <vector>
#include <string>
#include <stdexcept>
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
    std::vector<Component*> interferometer;
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

        YAML::Node node = config["interferometer"];
        std::cout << node.size() << '\n';

        // loop over interferometer components
        for (std::size_t i = 0; i < node.size(); i++) {

            if (node[i]["LinearPolariser"]){

                Polariser pol(node[i]["LinearPolariser"]["orientation"].as<double>());
                interferometer.push_back(&pol);
            }

            else if (node[i]["UniaxialCrystal"]){

                double thickness = node[i]["UniaxialCrystal"]["thickness"].as<double>();
                double cut_angle = node[i]["UniaxialCrystal"]["cut_angle"].as<double>();
                std::string material = node[i]["UniaxialCrystal"]["material"].as<std::string>();
                double orientation = node[i]["UniaxialCrystal"]["orientation"].as<double>();

                UniaxialCrystal uniaxial_crystal(thickness, cut_angle, material);
                uniaxial_crystal.orientation = orientation;
                interferometer.push_back(&uniaxial_crystal);
            }
            
            else { 
                throw std::logic_error("Interferometer component was not understood.");
            }
        }


        // for (YAML::const_iterator it=primes.begin();it!=primes.end();++it) {
        //     std::cout << it->as<int>() << "\n";
        // }


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