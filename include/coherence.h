#pragma once

#include <iostream>
#include <vector>
#include <complex>
#include "include/maths.h"
#include "include/spectrum.h"


namespace cispp {

/**
 * @brief Calculate the temporal coherence of a given spectrum, as measured by a 2-beam interferometer with given delay
 * 
 * @param wavelength wavelength in metres
 * @param spectral_flux photon spectral flux in photons/metre
 * @param delay interferometer delay in radians
 * @param wld wavelength to which the interferometer delay corresponds, in metres
 * @return temporal coherence in photons. 
 */
std::complex<double> calculate_coherence(std::vector<double> wavelength, std::vector<double> spec_flux, double delay, double wld);


/**
 * @brief calculate temporal coherence of a Gaussian at a given delay
 * 
 * @param wl0 
 * @param wlsigma 
 * @param flux 
 * @return std::complex<double> 
 */
std::complex<double> coherence_gaussian(double wl0, double wlsigma, double flux, double delay, double wld);


} // namespace cispp
