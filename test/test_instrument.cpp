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


int main (int argc, char **argv)
{
    auto fp_root = std::filesystem::absolute(std::filesystem::path(argv[0])).parent_path().parent_path();
    // std::string fp_config = (((fp_root / "test") / "config") / "single_delay_linear.yaml").string();
    std::string fp_config = (((fp_root / "test") / "config") / "single_delay_pixelated.yaml").string();

    cispp::Instrument inst(fp_config);
    std::cout << "inst.type : " << inst.type << std::endl;
    std::vector<unsigned short int> image(inst.camera.sensor_format_x * inst.camera.sensor_format_y);

    auto start = std::chrono::high_resolution_clock::now();
    inst.capture(465e-9, 500, &image);
    inst.image_to_file("out.ppm", &image);
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "duration = " << duration.count() * 1e-6 << " seconds" << std::endl;
    return 1;
}