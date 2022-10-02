#include "include/coherence.h"
#include "include/math.h"
#include "include/spectrum.h"



int main() {
    double delay = 80000;
    double wl0 = 465e-9;
    double wlsigma = 0.1e-9;
    double flux = 1000;
    size_t nbins = 1000;
    size_t nsigma = 5;
    cispp::Spectrum spec = cispp::gaussian(wl0, wlsigma, flux, nbins, nsigma);
    std::complex<double> coherence_n = cispp::calculate_coherence(spec.wavelength, spec.s0, delay, wl0);
    std::complex<double> coherence_a = cispp::coherence_gaussian(wl0, wlsigma, flux, delay, wl0);

    double flux_n = cispp::trapz(spec.wavelength, spec.s0);
    double contrast_n = std::abs(coherence_n) / flux_n;
    double phase_n = std::arg(coherence_n);
    std::cout << "coherence_n = " << coherence_n << std::endl;
    std::cout << "contrast_n = " << contrast_n << std::endl;
    std::cout << "flux_n = " << flux_n << std::endl; 
    std::cout << "flux = " << flux << std::endl; 
    
    return 0;
}