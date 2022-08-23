#include <algorithm>
#include <vector>
#include "../include/camera.h"
#include "../include/component.h"


/**
 * @brief Get x-position of pixel centres in metres
 * 
 * @return std::vector<double> 
 */
std::vector<double> Camera::get_pixel_centres_x()
{
    std::vector<double> px(sensor_format_x);
    for (size_t i = 0; i < sensor_format_x; i++){
        px[i] = (i + 0.5) * pixel_size - sensor_halfwidth;
    }
    return px;
}


/**
 * @brief Get x-position of pixel lower bounds in metres
 * 
 * @return std::vector<double> 
 */
std::vector<double> Camera::get_pixel_lbounds_x()
{
    std::vector<double> px(sensor_format_x);
    for (size_t i = 0; i < sensor_format_x; i++){
        px[i] = i * pixel_size - sensor_halfwidth;
    }
    return px;
}


/**
 * @brief Get y-position of pixel centres in metres
 * 
 * @return std::vector<double> 
 */
std::vector<double> Camera::get_pixel_centres_y()
{
    std::vector<double> py(sensor_format_y);
    for (size_t i = 0; i < sensor_format_y; i++){
        py[i] = (i + 0.5) * pixel_size - sensor_halfheight;
    }
    return py;
}


/**
 * @brief Get y-position of pixel lower bounds in metres
 * 
 * @return std::vector<double> 
 */
std::vector<double> Camera::get_pixel_lbounds_y()
{
    std::vector<double> py(sensor_format_y);
    for (size_t i = 0; i < sensor_format_y; i++){
        py[i] = i * pixel_size - sensor_halfheight;
    }
    return py;
}


/**
 * @brief Get x-index of pixel by x-position in metres
 * 
 * @param x 
 * @return size_t 
 */
size_t Camera::get_pixel_idx_x(double x)
{
    std::vector<double>::iterator it;
    it = std::lower_bound(pixel_lbounds_x.begin(), pixel_lbounds_x.end(), x);
    return it - 1 - pixel_lbounds_x.begin();
}


/**
 * @brief Get y-index of pixel by x-position in metres
 * 
 * @param y 
 * @return size_t 
 */
size_t Camera::get_pixel_idx_y(double y)
{
    std::vector<double>::iterator it;
    it = std::lower_bound(pixel_lbounds_y.begin(), pixel_lbounds_y.end(), y);
    return it - 1 - pixel_lbounds_y.begin();
}


/**
* @brief Get pixelated phase mask value by pixel index
* 
* @param x x-pixel index
* @param y y-pixel index
* @return double 
*/
double Camera::get_pixelated_phase_mask(double x, double y)
{
    size_t ix = get_pixel_idx_x(x);
    size_t iy = get_pixel_idx_y(y);

    if (ix % 2 == 0){
        if (iy % 2 == 0){
            return 0.;
        }
        else {
            return 3 * M_PI / 2;
        }
    }
    else {
        if (iy % 2 == 0){
            return M_PI / 2;
        }
        else {
            return M_PI;
        }
    }
}

/**
 * @brief Get mueller matrix by pixel index
 * 
 * @param x x-pixel index
 * @param y y-pixel index
 * @return Eigen::Matrix4d 
 */
Eigen::Matrix4d Camera::get_mueller_matrix(double x, double y)
{
    size_t ix = get_pixel_idx_x(x);
    size_t iy = get_pixel_idx_y(y);

    if (ix % 2 == 0){
        if (iy % 2 == 0){
            return Polariser(0).get_mueller_matrix();
        }
        else {
            return Polariser(135).get_mueller_matrix();
        }
    }
    else {
        if (iy % 2 == 0){
            return Polariser(45).get_mueller_matrix();
        }
        else {
            return Polariser(90).get_mueller_matrix();
        }
    }
}