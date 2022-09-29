#include "include/coherence.h"
#include "include/math.h"



int main() {
    double wl0 = 466e-9;
    std::vector<double> wavelength { 465.9e-9, 466e-9, 466.1e-9 };
    std::vector<double> spec_flux { 100, 200, 300 };
    double delay = 80000;
    std::complex<double> coherence = cispp::calculate_coherence(wavelength, spec_flux, delay, wl0);
    double flux = cispp::trapz(wavelength, spec_flux);
    double contrast = std::abs(coherence) / flux;
    std::cout << "coherence = " << coherence << std::endl;
    std::cout << "contrast = " << contrast << std::endl;
    std::cout << "flux = " << flux << std::endl; 
    return 0;
}