#include "include/coherence.h"
#include "include/math.h"


namespace cispp {


std::complex<double> calculate_coherence(std::vector<double> wavelength, std::vector<double> spec_flux, double delay, double wl0)
{
    assert (wavelength.size() == spec_flux.size());

    std::vector<std::complex<double>> integrand(wavelength.size(),0);
    std::vector<std::complex<double>> wavelength_c(wavelength.size(),0);

    for (size_t i=0; i < wavelength.size(); i++) 
    {
        std::complex<double> exponent(0., delay + delay * (wl0 - wavelength[i]) / wavelength[i]);
        integrand[i] = spec_flux[i] * std::exp(exponent);

        std::complex<double> wl_c(wavelength[i]); 
        wavelength_c[i] = wl_c;
    }
    return cispp::trapz(wavelength_c, integrand);
}


} // namespace cispp