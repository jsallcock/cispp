#include <iostream>
#include "../include/camera.h"


int main()
{   
    int bit_depth = 12;
    int sensor_format_x = 2448;
    int sensor_format_y = 2048;
    double pixel_size = 3.45e-6;
    double quantum_efficiency = 0.35;
    double epercount = 0.46;
    double cam_noise = 2.5;
    std::string type = "monochrome";
    Camera camera(
        sensor_format_x, 
        sensor_format_y, 
        pixel_size, 
        bit_depth, 
        quantum_efficiency, 
        epercount, 
        cam_noise,
        type
    );


    // for (size_t i = 0; i < camera.sensor_format_x; i++)
    // {
    //     std::cout << i << std::endl;
    //     std::cout << camera.pixel_centres_x[i] << std::endl;
    //     std::cout << camera.get_pixel_idx_x(camera.pixel_centres_x[i]) << std::endl;
    //     std::cout << std::endl;
    // }   

    for (size_t i = 0; i < camera.sensor_format_y; i++)
    {
        std::cout << i << std::endl;
        std::cout << camera.pixel_centres_y[i] << std::endl;
        std::cout << camera.get_pixel_idx_y(camera.pixel_centres_y[i]) << std::endl;
        std::cout << std::endl;
    }   
}