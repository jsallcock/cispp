#include "include/coherence.h"
#include "include/maths.h"


namespace cispp {


std::complex<double> calculate_coherence(std::vector<double> wavelength, std::vector<double> spec_flux, double delay, double wld)
{
    assert (wavelength.size() == spec_flux.size());

    std::vector<std::complex<double>> integrand(wavelength.size(),0);
    std::vector<std::complex<double>> wlc(wavelength.size(),0);  // complex wavelength

    for (size_t i=0; i < wavelength.size(); i++) 
    {
        std::complex<double> exponent(0., delay + delay * (wld - wavelength[i]) / wavelength[i]);
        std::complex<double> wlc_i(wavelength[i]); 
        integrand[i] = spec_flux[i] * std::exp(exponent);
        wlc[i] = wlc_i;
    }
    return cispp::trapz(wlc, integrand);
}


std::complex<double> coherence_gaussian(double wl0, double wlsigma, double flux, double delay, double wld)
{
    const double f0 = cispp::SPEED_OF_LIGHT / wl0;
    const double fd = cispp::SPEED_OF_LIGHT / wld;
    const double fsigma = - cispp::SPEED_OF_LIGHT * wlsigma / ((wlsigma + wl0) * wl0);
    const std::complex<double> contrast(std::exp(-0.5 * std::pow(fsigma * delay / fd, 2)));
    const std::complex<double> iphase(0, delay + delay * ((f0 - fd) / fd));
    return flux * contrast * std::exp(iphase); 
}


} // namespace cispp