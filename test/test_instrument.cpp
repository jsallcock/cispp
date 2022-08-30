#include <string>
#include <iostream>
#include <cmath>
#include <vector>
#include <chrono>
#include <filesystem>
#include <Eigen/Dense>
#include "include/component.h"
#include "include/instrument.h"
#include "include/material.h"


/**
 * @brief test single_delay_linear type is consistent with the general Mueller matrix model
 * 
 * @param fp_root 
 * @return true 
 * @return false 
 */
bool test_single_delay_linear(std::filesystem::path fp_root)
{
    std::string fp_config = (((fp_root / "test") / "config") / "single_delay_linear.yaml").string();

    cispp::Instrument inst(fp_config);
    cispp::Instrument inst_mueller(fp_config);
    inst_mueller.type = "mueller";
    double wavelength = 465e-9;
    double flux = 500;

    std::vector<unsigned short int> image(inst.camera.sensor_format_x * inst.camera.sensor_format_y);
    std::vector<unsigned short int> image_mueller(inst.camera.sensor_format_x * inst.camera.sensor_format_y);

    auto start = std::chrono::high_resolution_clock::now();
    inst.capture(wavelength, flux, &image);
    inst.save_image("single_delay_linear.ppm", &image);
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "single_delay_linear: duration = " << duration.count() * 1e-6 << " s" << std::endl;

    start = std::chrono::high_resolution_clock::now();
    inst_mueller.capture(wavelength, flux, &image_mueller);
    inst_mueller.save_image("single_delay_linear_mueller.ppm", &image_mueller);
    stop = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "mueller: duration = " << duration.count() * 1e-6 << " s" << std::endl;

    for (size_t j = 0; j < inst.camera.sensor_format_y; j++)
    {
        size_t idx_col = j * inst.camera.sensor_format_x;
        for (size_t i = 0; i < inst.camera.sensor_format_x; i++)
        {
            if (image[i + idx_col] != image_mueller[i + idx_col])
            {
                return false;
            }
        }
    }
    
    return true;
}


bool test_single_delay_pixelated(std::filesystem::path fp_root)
{
    std::string fp_config = (((fp_root / "test") / "config") / "single_delay_pixelated.yaml").string();

    cispp::Instrument inst(fp_config);
    cispp::Instrument inst_mueller(fp_config);
    inst_mueller.type = "mueller";
    double wavelength = 465e-9;
    double flux = 500;

    std::vector<unsigned short int> image(inst.camera.sensor_format_x * inst.camera.sensor_format_y);
    std::vector<unsigned short int> image_mueller(inst.camera.sensor_format_x * inst.camera.sensor_format_y);

    auto start = std::chrono::high_resolution_clock::now();
    inst.capture(wavelength, flux, &image);
    inst.save_image("single_delay_pixelated.ppm", &image);
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "single_delay_pixelated: duration = " << duration.count() * 1e-6 << " s" << std::endl;

    start = std::chrono::high_resolution_clock::now();
    inst_mueller.capture(wavelength, flux, &image_mueller);
    inst_mueller.save_image("single_delay_pixelated_mueller.ppm", &image_mueller);
    stop = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "mueller: duration = " << duration.count() * 1e-6 << " s" << std::endl;

    for (size_t j = 0; j < inst.camera.sensor_format_y; j++)
    {
        size_t idx_col = j * inst.camera.sensor_format_x;
        for (size_t i = 0; i < inst.camera.sensor_format_x; i++)
        {
            if (image[i + idx_col] != image_mueller[i + idx_col])
            {
                return false;
            }
        }
    }
    
    return true;
}


int main (int argc, char **argv)
{
    auto fp_root = std::filesystem::absolute(std::filesystem::path(argv[0])).parent_path().parent_path();

    // if (test_single_delay_linear(fp_root)){
    //     std::cout << "test_single_delay_linear: passed" << '\n';
    // }
    // else {
    //     std::cout << "test_single_delay_linear: failed" << '\n';
    // };

    //  if (test_single_delay_pixelated(fp_root)){
    //     std::cout << "test_single_delay_pixelated: passed" << '\n';
    // }
    // else {
    //     std::cout << "test_single_delay_pixelated: failed" << '\n';
    // };

    std::string fp_config = (((fp_root / "test") / "config") / "single_delay_pixelated.yaml").string();
    cispp::Instrument inst(fp_config);

    double wavelength = 465e-9;
    double flux = 500;

    std::vector<unsigned short int> image(inst.camera.sensor_format_x * inst.camera.sensor_format_y);
    inst.capture(wavelength, flux, &image);
    inst.save_image("test.ppm", &image);

    return 0;
}