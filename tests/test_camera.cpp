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
    Camera cam(sensor_format_x, sensor_format_y, pixel_size, bit_depth, quantum_efficiency, epercount, cam_noise);

    std::vector<double> ppx = cam.get_pixel_positions_x();
    for (size_t i = 0; i < ppx.size(); i++)
    {
        std::cout << ppx[i] << std::endl;
    }
    
}