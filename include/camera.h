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
     * @brief Construct a new Camera object
     * 
     * @param sensor_format_x 
     * @param sensor_format_y 
     * @param pixel_size 
     * @param bit_depth 
     * @param quantum_efficiency 
     * @param epercount 
     * @param cam_noise 
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

    /**
     * @brief Construct a new Camera object
     * 
     */
    Camera()
    : sensor_format_x(1000),
      sensor_format_y(1000), 
      pixel_size(3.45e-6),
      bit_depth(8),
      quantum_efficiency(0.5),
      epercount(1),
      cam_noise(2)
    {}

    std::vector<double> get_pixel_positions_x();
    std::vector<double> get_pixel_positions_y();
    void capture();
};

#endif