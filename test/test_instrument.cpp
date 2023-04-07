#include <string>
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <chrono>
#include <filesystem>
#include <Eigen/Dense>

#include "include/component.h"
#include "include/instrument.h"
#include "include/material.h"
#include "include/paths.h"
#include "include/spectrum.h"


/**
 * @brief 
 * 
 * @return true 
 * @return false 
 */
bool Test2ImagesSame(std::vector<unsigned short int> &im1, std::vector<unsigned short int> &im2)
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
 * @brief test that an instrument's overridden Capture method gives identical output to the full Mueller matrix calculation   
 * 
 * @param instname 
 * @param specname 
 * @return true 
 * @return false 
 */
bool TestCaptureInstVsMueller(std::string instname, std::string specname)
{
    std::filesystem::path rootPath = cispp::getRootPath();
    std::string fp_config = (((rootPath / "test") / "config") / (instname + ".yaml"));
    auto inst = cispp::LoadInstrument(fp_config);
    auto inst_m = cispp::LoadInstrument(fp_config, true);  // force_mueller
    std::vector<unsigned short int> image(inst->camera.sensor_format_x * inst->camera.sensor_format_y);
    std::vector<unsigned short int> image_m(inst->camera.sensor_format_x * inst->camera.sensor_format_y);
        
    auto start = std::chrono::high_resolution_clock::now();
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

    if (specname == "Monochrome")
    {
        double wavelength = 465e-9;
        double flux = 500;

        start = std::chrono::high_resolution_clock::now();
        inst->Capture(wavelength, flux, &image);
        stop = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
        std::cout << "duration = " << duration.count() * 1e-6 << " s" << std::endl;

        start = std::chrono::high_resolution_clock::now();
        inst_m->Capture(wavelength, flux, &image_m);    
        stop = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
        std::cout << "duration = " << duration.count() * 1e-6 << " s" << std::endl;
    }
    else if (specname == "Spectrum")
    {
        double wl0 = 465e-9;
        double wlsigma = 0.1e-9;
        double flux = 500;
        size_t nbins = 15;
        size_t nsigma = 4;
        cispp::Spectrum spec = cispp::gaussian(wl0, wlsigma, flux, nbins, nsigma);  
        
        start = std::chrono::high_resolution_clock::now();
        inst->Capture(spec.wavelength, spec.s0, &image);
        stop = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
        std::cout << "duration = " << duration.count() * 1e-6 << " s" << std::endl;

        start = std::chrono::high_resolution_clock::now();
        inst_m->Capture(spec.wavelength, spec.s0, &image_m);
        stop = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
        std::cout << "duration = " << duration.count() * 1e-6 << " s" << std::endl;
    }

    inst->SaveImage("TestCapture" + specname + instname + ".pbm", &image);
    inst_m->SaveImage("TestCapture" + specname + instname + "ForceMueller.pbm", &image_m);

    return Test2ImagesSame(image, image_m);
}


int main ()
{
    std::vector<std::string> instnames { "SingleDelayLinear", "SingleDelayPixelated" };
    std::vector<std::string> specnames { "Monochrome", "Spectrum" };
    std::cout << '\n';

    for (const std::string& instname: instnames)
    {
        for (const std::string& specname: specnames)
        {
            std::cout << "TestCapture" + specname + instname + ":\n";
            if (TestCaptureInstVsMueller(instname, specname))
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