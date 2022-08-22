#include <string>
#include <iostream>
#include <cmath>
#include <Eigen/Dense>
#include "../include/instrument.h"
#include "../include/material.h"


int main ()
{
    std::string fp_config = "/Users/jsallcock/fusion/ci/cispp/tests/config/single_delay_linear.yaml";
    Instrument inst(fp_config);
    inst.capture(465e-9, 1.e4);
}