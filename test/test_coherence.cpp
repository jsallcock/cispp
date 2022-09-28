#include "include/coherence.h"



int main() {
    std::vector<double> wavelength { 465e-9, 466e-9, 467e-9 };
    std::vector<double> spec_flux { 100, 200, 300 };
    double delay = 8000;
    cispp::calculate_coherence(wavelength, spec_flux, delay);
    return 0;
}