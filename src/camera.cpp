#include <vector>
#include "../include/camera.h"

/**
 * @brief x-coordinates of camera pixel centres in metres
 * 
 * @return std::vector<double> 
 */
std::vector<double> Camera::get_pixel_positions_x()
{
    double centre_pos_x = pixel_size * sensor_format_x / 2;
    std::vector<double> ppx(sensor_format_x);
    for (size_t i = 0; i < sensor_format_x; i++)
    {
        ppx[i] = (i + 0.5) * pixel_size - centre_pos_x;
    }
    return ppx;
}

/**
 * @brief y-coordinates of camera pixel centres in metres
 * 
 * @return std::vector<double> 
 */
std::vector<double> Camera::get_pixel_positions_y()
{
    double centre_pos_y = pixel_size * sensor_format_y / 2;
    std::vector<double> ppy(sensor_format_y);
    for (size_t i = 0; i < sensor_format_y; i++)
    {
        ppy[i] = (i + 0.5) * pixel_size - centre_pos_y;
    }
    return ppy;
}