#pragma once

#include <iostream>
#include <vector>
#include "include/maths.h"


namespace cispp {


constexpr double SPEED_OF_LIGHT = 2.99792458e8;


/**
 * @brief generic container for Stokes-resolved spectrum
 * 
 */
struct Spectrum 
{
    std::vector<double> wavelength;
    std::vector<double> s0;
    std::vector<double> s1;
    std::vector<double> s2;
    std::vector<double> s3;

    Spectrum(std::vector<double> wavelength, std::vector<double> s0)
    : wavelength(wavelength),
      s0(s0)
    {
        assert (wavelength.size() == s0.size());
        size_t n = wavelength.size();
        std::vector<double> s1(n, 0.);
        std::vector<double> s2(n, 0.);
        std::vector<double> s3(n, 0.);
    }
};


/**
 * @brief Gaussian singlet spectrum, area-normalised to flux
 * 
 * @param wl0 central wavelength in metres
 * @param wlsigma standard deviation wavelenght in metres
 * @param flux 
 * @param nbins number of wavelength bins on which to evaluate spectrum 
 * @param nsigma number of standard deviations from wl0 to extend the wavelength grid
 * @return Spectrum 
 */
Spectrum gaussian(double wl0, double wlsigma, double flux, size_t nbins, size_t nsigma);


Spectrum get_gaussian_multiplet();


} // namespace cispp
