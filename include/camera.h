#ifndef COMPONENT_H
#define COMPONENT_H
#include <vector>

class Camera
{
    public:

    int sensor_format_x;
    int sensor_format_y;
    double pixel_size;
    int bit_depth;
    double quantum_efficiency;
    double epercount;
    double cam_noise;

    /**
    */
    Camera
    (
        int sensor_format_x,
        int sensor_format_y,
        double pixel_size,
        int bit_depth,
        double quantum_efficiency,
        double epercount,
        double cam_noise
    )
    : sensor_format_x(sensor_format_x),
      sensor_format_y(sensor_format_y), 
      pixel_size(pixel_size),
      bit_depth(bit_depth),
      quantum_efficiency(quantum_efficiency),
      epercount(epercount),
      cam_noise(cam_noise)
    {}

    std::vector<double> get_pixel_positions_x();
    std::vector<double> get_pixel_positions_y();
    void capture();
};

#endif