#ifndef CISPP_COHERENCE_H
#define CISPP_COHERENCE_H

#include <iostream>
#include <vector>
#include <complex> 


namespace cispp {

/**
 * @brief 
 * 
 * @param wavelength 
 * @param spectral_flux 
 * @param delay 
 * @return double 
 */
std::complex<double> calculate_coherence(std::vector<double> wavelength, std::vector<double> spec_flux, double delay);


} // namespace cispp
#endif