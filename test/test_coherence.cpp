#include "include/coherence.h"
#include "include/maths.h"
#include "include/spectrum.h"


bool test_coherence_gaussian()
{
    double delay = 40000;
    double wl0 = 465e-9;
    double wlsigma = 0.05e-9;
    double flux = 1000;
    size_t nbins = 10000;
    size_t nsigma = 10;
    double wld = 464e-9;
    cispp::Spectrum spec = cispp::gaussian(wl0, wlsigma, flux, nbins, nsigma);
    std::complex<double> coherence_n = cispp::calculate_coherence(spec.wavelength, spec.s0, delay, wld);
    std::complex<double> coherence_a = cispp::coherence_gaussian(wl0, wlsigma, flux, delay, wld);

    double flux_n = cispp::trapz(spec.wavelength, spec.s0);
    double contrast_n = std::abs(coherence_n) / flux_n;
    double phase_n = std::arg(coherence_n);
    
    double contrast_a = std::abs(coherence_a) / flux;
    double phase_a = std::arg(coherence_a);

    const double tol = 1e-8;

    const double err_c = std::abs((contrast_n - contrast_a));
    const double err_p = std::abs(cispp::wrap(phase_n - phase_a));

    return (err_c < tol && err_p < tol);
}



int main() {

    std::cout << "test_coherence_gaussian: ";
    if (test_coherence_gaussian()){
        std::cout << "passed" << '\n';
    } else {
        std::cout << "failed" << '\n';
    }
    
    return 0;
}