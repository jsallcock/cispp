#include "yaml-cpp/yaml.h"
#include "../include/camera.h"


/* Definitions of interferometer components */



class Instrument
{
    double lens1_focal_length;
    double lens2_focal_length;
    double lens3_focal_length;
    // Camera camera;

    std::string type = "Mueller";

    public:
    std::string fp_config;

    Instrument(std::string fp_config)
    : fp_config(fp_config)
    {}

    void read_config(){}
    void write_config(){}
    void get_inc_angle(){}
    void get_azim_angle(){}
    void get_mueller_matrix(){}
    void get_instrument_type(){}
    void get_delay(){}
    void capture(){}

};