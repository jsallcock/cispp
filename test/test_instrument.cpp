#include <string>
#include <iostream>
#include <cmath>
#include <vector>
#include <chrono>
#include <filesystem>
#include <Eigen/Dense>

#include "include/component.h"
#include "include/instrument.h"
#include "include/material.h"
#include "include/spectrum.h"


/**
 * @brief 
 * 
 * @return true 
 * @return false 
 */
bool test_images_same(std::vector<unsigned short int> &im1, std::vector<unsigned short int> &im2)
{
    assert (im1.size() == im2.size());
    for (size_t i = 0; i < im1.size(); i++)
    {
        if (im1[i] != im2[i])
        {
            return false;
        }
    }
    return true;
}


/**
 * @brief test that an instrument's overridden capture method gives identical output to the full Mueller matrix calculation   
 * 
 * @param instname 
 * @param specname 
 * @return true 
 * @return false 
 */
bool test_capture_inst_vs_mueller(std::filesystem::path fp_root, std::string instname, std::string specname)
{
    std::string fp_config = (((fp_root / "test") / "config") / (instname + ".yaml"));
    auto inst = cispp::load_instrument(fp_config);
    auto inst_m = cispp::load_instrument(fp_config, true);  // force_mueller
    std::vector<unsigned short int> image(inst->camera.sensor_format_x * inst->camera.sensor_format_y);
    std::vector<unsigned short int> image_m(inst->camera.sensor_format_x * inst->camera.sensor_format_y);
        

    if (specname == "monochrome")
    {
        double wavelength = 465e-9;
        double flux = 500;
        inst->capture(wavelength, flux, &image);
        inst_m->capture(wavelength, flux, &image_m);    
    }
    else if (specname == "spectrum")
    {
        double wl0 = 465e-9;
        double wlsigma = 0.1e-9;
        double flux = 500;
        size_t nbins = 15;
        size_t nsigma = 4;
        cispp::Spectrum spec = cispp::gaussian(wl0, wlsigma, flux, nbins, nsigma);  
        inst->capture(spec.wavelength, spec.s0, &image);
        inst_m->capture(spec.wavelength, spec.s0, &image_m);
    }

    inst->save_image("test_capture_" + specname + "_" + instname + ".pbm", &image);
    inst_m->save_image("test_capture_" + specname + "_" + instname + "_FORCE_MUELLER.pbm", &image_m);

    return test_images_same(image, image_m);

    // auto start = std::chrono::high_resolution_clock::now();
    // inst->capture(wavelength, s_in, &image);
    // auto stop = std::chrono::high_resolution_clock::now();
    // auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    // std::cout << instname << ": duration = " << duration.count() * 1e-6 << " s" << std::endl;

    // start = std::chrono::high_resolution_clock::now();
    // inst_m->capture(wavelength, s_in, &image_m);
    // stop = std::chrono::high_resolution_clock::now();
    // duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    // std::cout << "mueller: duration = " << duration.count() * 1e-6 << " s" << std::endl;
}


int main (int argc, char **argv)
{
    auto fp_root = std::filesystem::absolute(std::filesystem::path(argv[0])).parent_path().parent_path();


    std::vector<std::string> instnames { "single_delay_linear", "single_delay_pixelated" };
    std::vector<std::string> specnames { "monochrome", "spectrum" };

    for (size_t i=0; i < instnames.size(); i++)
    {
        for (size_t j=0; j < specnames.size(); j++)
        {
            std::string instname = instnames[i];
            std::string specname = specnames[j];
            std::cout << "test_capture_" + specname + "_" + instname + ":\n";
            if (test_capture_inst_vs_mueller(fp_root, instname, specname))
            {
                std::cout << "passed";
            }
            else 
            {
                std::cout << "failed";
            }
            std::cout << "\n\n\n";
        }
    }
    return 0;
}