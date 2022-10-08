#ifndef CISPP_CAMERA_H
#define CISPP_CAMERA_H
#include <string>
#include <vector>
#include <Eigen/Dense>
#include "include/component.h"


namespace cispp {


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
    std::string type {"monochrome"};
    double sensor_halfwidth;
    double sensor_halfheight;
    std::vector<double> pixel_centres_x;
    std::vector<double> pixel_centres_y;
    std::vector<double> pixel_lbounds_x;
    std::vector<double> pixel_lbounds_y;


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
     * @param type 
     */
    Camera
    (
        int sensor_format_x,
        int sensor_format_y,
        double pixel_size,
        int bit_depth,
        double quantum_efficiency,
        double epercount,
        double cam_noise,
        std::string type
    )
    : sensor_format_x(sensor_format_x),
      sensor_format_y(sensor_format_y), 
      pixel_size(pixel_size),
      bit_depth(bit_depth),
      quantum_efficiency(quantum_efficiency),
      epercount(epercount),
      cam_noise(cam_noise),
      type(type),
      sensor_halfwidth(0.5 * pixel_size * sensor_format_x),
      sensor_halfheight(0.5 * pixel_size * sensor_format_y),
      pixel_centres_x(get_pixel_centres_x()),
      pixel_centres_y(get_pixel_centres_y()),
      pixel_lbounds_x(get_pixel_lbounds_x()),
      pixel_lbounds_y(get_pixel_lbounds_y())
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
      cam_noise(2),
      sensor_halfwidth(0.5 * pixel_size * sensor_format_x),
      sensor_halfheight(0.5 * pixel_size * sensor_format_y),
      pixel_centres_x(get_pixel_centres_x()),
      pixel_centres_y(get_pixel_centres_y()),
      pixel_lbounds_x(get_pixel_lbounds_x()),
      pixel_lbounds_y(get_pixel_lbounds_y())
    {}

    /**
     * @brief Get x-position of pixel centres in metres
     * 
     * @return std::vector<double> 
     */
    std::vector<double> get_pixel_centres_x() const;

    /**
     * @brief Get x-position of pixel lower bounds in metres
     * 
     * @return std::vector<double> 
     */
    std::vector<double> get_pixel_lbounds_x() const;

    /**
     * @brief Get y-position of pixel centres in metres
     * 
     * @return std::vector<double> 
     */
    std::vector<double> get_pixel_centres_y() const;

    /**
     * @brief Get y-position of pixel lower bounds in metres
     * 
     * @return std::vector<double> 
     */
    std::vector<double> get_pixel_lbounds_y() const;

    /**
     * @brief Get x-index of pixel by x-position in metres
     * 
     * @param x 
     * @return size_t 
     */
    size_t get_pixel_idx_x(double x);

    /**
     * @brief Get y-index of pixel by y-position in metres
     * 
     * @param y 
     * @return size_t 
     */
    size_t get_pixel_idx_y(double y);

    /**
     * @brief Get pixelated phase mask value by xy-position in metres
     * 
     * @param x x-pixel index
     * @param y y-pixel index
     * @return double 
     */
    double get_pixelated_phase_mask(double x, double y);

    /**
     * @brief Get mueller matrix by xy-position in metres
     * 
     * @param x x-pixel index
     * @param y y-pixel index
     * @return Eigen::Matrix4d 
     */
    Eigen::Matrix4d GetMuellerMatrix(double x, double y);
};


}  // namespace cispp
#endif