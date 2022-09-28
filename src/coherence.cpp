#include "include/coherence.h"
#include "include/math.h"


namespace cispp {


std::complex<double> calculate_coherence(std::vector<double> wavelength, std::vector<double> spec_flux, double delay)
{
    assert (wavelength.size() == spec_flux.size());
    
    std::complex<double> idelay(0., delay);
    std::complex<double> exp = std::exp(idelay);

    std::vector<std::complex<double>> integrand(wavelength.size(),0);
    std::vector<std::complex<double>> wavelength_c(wavelength.size(),0);

    for (size_t i; i < wavelength.size(); i++) 
    {
        integrand[i] = spec_flux[i] * exp;
    }
    return cispp::trapz(wavelength, integrand);
}


} // namespace cispp