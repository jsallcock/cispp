#include "include/spectrum.h"


int main()
{
    double wl0 = 465e-9;
    double wlsigma = 0.1e-9;
    double flux = 1000;
    size_t nbins = 1000;
    size_t nsigma = 6;
    cispp::Spectrum spec = cispp::gaussian(wl0, wlsigma, flux, nbins, nsigma);
    double flux_n = cispp::trapz(spec.wavelength, spec.s0);
    std::cout << "flux = " << flux << '\n';
    std::cout << "flux_n = " << flux_n << '\n';
    return 0;
}