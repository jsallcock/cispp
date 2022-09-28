#include <algorithm>
#include <vector>
#include "include/camera.h"
#include "include/component.h"


namespace cispp {


std::vector<double> cispp::Camera::get_pixel_centres_x() const
{
    std::vector<double> px(sensor_format_x);
    for (size_t i = 0; i < sensor_format_x; i++){
        px[i] = (i + 0.5) * pixel_size - sensor_halfwidth;
    }
    return px;
}


std::vector<double> cispp::Camera::get_pixel_lbounds_x() const
{
    std::vector<double> px(sensor_format_x);
    for (size_t i = 0; i < sensor_format_x; i++){
        px[i] = i * pixel_size - sensor_halfwidth;
    }
    return px;
}


std::vector<double> cispp::Camera::get_pixel_centres_y() const
{
    std::vector<double> py(sensor_format_y);
    for (size_t i = 0; i < sensor_format_y; i++){
        py[i] = (i + 0.5) * pixel_size - sensor_halfheight;
    }
    return py;
}


std::vector<double> cispp::Camera::get_pixel_lbounds_y() const
{
    std::vector<double> py(sensor_format_y);
    for (size_t i = 0; i < sensor_format_y; i++){
        py[i] = i * pixel_size - sensor_halfheight;
    }
    return py;
}


size_t cispp::Camera::get_pixel_idx_x(double x)
{
    std::vector<double>::iterator it;
    it = std::lower_bound(pixel_lbounds_x.begin(), pixel_lbounds_x.end(), x);
    return it - 1 - pixel_lbounds_x.begin();
}


size_t cispp::Camera::get_pixel_idx_y(double y)
{
    std::vector<double>::iterator it;
    it = std::lower_bound(pixel_lbounds_y.begin(), pixel_lbounds_y.end(), y);
    return it - 1 - pixel_lbounds_y.begin();
}


double cispp::Camera::get_pixelated_phase_mask(double x, double y)
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


Eigen::Matrix4d cispp::Camera::get_mueller_matrix(double x, double y)
{
    size_t ix = get_pixel_idx_x(x);
    size_t iy = get_pixel_idx_y(y);

    if (ix % 2 == 0){
        if (iy % 2 == 0){
            return Polariser(0).get_mueller_matrix();
        }
        else {
            return Polariser(3 * M_PI / 4).get_mueller_matrix();
        }
    }
    else {
        if (iy % 2 == 0){
            return Polariser(M_PI / 4).get_mueller_matrix();
        }
        else {
            return Polariser(M_PI / 2).get_mueller_matrix();
        }
    }
}


} // namespace cispp