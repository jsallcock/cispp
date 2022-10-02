#include <cmath>
#include "include/spectrum.h"


namespace cispp {


cispp::Spectrum gaussian(double wl0, double sigma, double flux, size_t nbins, size_t nsigma)
{
    std::vector<double> wl(nbins, 0.);
    std::vector<double> s0(nbins, 0.);

    const double dwl = 2 * nsigma * sigma / nbins;
    const double wlmin = wl0 - nsigma * sigma;
    const double c = (sigma + wl0) / sigma;
    const double norm = flux * c * wl0 / std::sqrt(2 * M_PI);

    for (size_t i=0; i<nbins; i++)
    {
        double wl_i = wlmin + i * dwl;
        wl[i] = wl_i;
        s0[i] = std::pow(wl_i, -2) * norm * std::exp(-0.5 * std::pow(((wl0 - wl_i) / wl_i) * c, 2));
    }
    return cispp::Spectrum(wl, s0);
}


} // namespace cispp