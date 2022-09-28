

/**
 * @brief container for Stokes-resolved spectrum
 * 
 */
struct Spectrum 
{
    vector<double> wavelength;
    vector<double> s0;
    vector<double> s1;
    vector<double> s2;
    vector<double> s3;  
}



Spectrum get_gaussian_singlet();

Spectrum get_gaussian_multiplet();

