#include <string>
#include <iostream>
#include <cmath>
#include <chrono>
#include <Eigen/Dense>
#include "../include/component.h"
#include "../include/instrument.h"
#include "../include/material.h"


int main ()
{
    std::string fp_config = "/Users/jsallcock/fusion/ci/cispp/tests/config/single_delay_linear.yaml";
    Instrument inst(fp_config);

    auto start = std::chrono::high_resolution_clock::now();
    inst.capture(465e-9, 1.e4);
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

    std::cout << "duration = " << duration.count() * 1e-6 << " seconds" << std::endl;
}