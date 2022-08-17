#include <string>
#include <iostream>
#include "../include/instrument.h"


int main ()
{
    // std::string fp_config = "./config/single_delay_linear.yaml";
    std::string fp_config = "/Users/jsallcock/fusion/ci/cispp/tests/config/single_delay_linear.yaml";
    Instrument inst(fp_config);
    std::cout << inst.interferometer.size() << '\n';
    // std::cout << inst.lens_2_focal_length << '\n';
    // std::cout << inst.lens_3_focal_length << '\n';
    // std::cout << inst.camera.sensor_format_x << '\n';
    // std::cout << inst.camera.sensor_format_y << '\n';
    // std::cout << inst.camera.pixel_size << '\n';
    // std::cout << inst.camera.bit_depth << '\n';
    // std::cout << inst.camera.quantum_efficiency << '\n';
    // std::cout << inst.camera.epercount << '\n';
    // std::cout << inst.camera.cam_noise << '\n';
}