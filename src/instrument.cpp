#include "include/instrument.h"

#include "include/material.h"
#include "include/camera.h"
#include "include/maths.h"

#include "yaml-cpp/yaml.h"

using std::vector;
using std::unique_ptr;
using std::string;


namespace cispp {


Instrument::Instrument(std::filesystem::path fp_config)
: fp_config(fp_config)
{
    YAML::Node nd_config = YAML::LoadFile(fp_config);
    YAML::Node nd_camera = nd_config["camera"];
    YAML::Node nd_components = nd_config["interferometer"];

    lens_1_focal_length = nd_config["lens_1_focal_length"].as<double>();
    lens_2_focal_length = nd_config["lens_2_focal_length"].as<double>();
    lens_3_focal_length = nd_config["lens_3_focal_length"].as<double>();

    camera = ParseNodeCamera(nd_camera);
    ParseNodeComponents(nd_components, components);
}


cispp::Camera Instrument::ParseNodeCamera(YAML::Node nd_camera)
{
    cispp::Camera cam(
        nd_camera["sensor_format"][0].as<int>(),  // sensor_format_x
        nd_camera["sensor_format"][1].as<int>(),  // sensor_format_y
        nd_camera["pixel_size"].as<double>(),
        nd_camera["bit_depth"].as<int>(),
        nd_camera["qe"].as<double>(),
        nd_camera["epercount"].as<double>(),
        nd_camera["cam_noise"].as<double>(),
        nd_camera["type"].as<string>("monochrome")
    );
    return cam;
}

void Instrument::ParseNodeComponents(YAML::Node nd_components, vector<unique_ptr<cispp::Component>>& components)
{
    for (size_t i = 0; i < nd_components.size(); i++)
    {
        YAML::Node nd_comp = nd_components[i];
        if (nd_comp["LinearPolariser"])
        {
            YAML::Node node = nd_comp["LinearPolariser"];
            double orientation = node["orientation"].as<double>(0) * M_PI / 180;
            auto ptr = std::make_unique<cispp::Polariser>(orientation);
            components.push_back(std::move(ptr));
        }

        else if (nd_comp["UniaxialCrystal"])
        {
            YAML::Node node = nd_comp["UniaxialCrystal"];
            double orientation = node["orientation"].as<double>() * M_PI / 180;
            double tilt_x = node["tilt_x"].as<double>(0);
            double tilt_y = node["tilt_y"].as<double>(0);
            double thickness = node["thickness"].as<double>();
            double cut_angle = node["cut_angle"].as<double>() * M_PI / 180;
            std::string material = node["material"].as<std::string>();
            
            if (node["sellmeier_coefs"]) {
                MaterialProperties mp = {};
                mp.name = material;

                std::string alphabet = "ABCDEF";
                for (int j=0; j<alphabet.size(); j++)
                {
                    std::string key(1, alphabet[i]);
                    if (node["sellmeier_coefs"][key + "e"])
                    {
                        double e = node["sellmeier_coefs"][key + "e"].as<double>();
                        double o = node["sellmeier_coefs"][key + "o"].as<double>();
                        mp.sellmeier_e.push_back(e);
                        mp.sellmeier_o.push_back(o);
                    }
                }   
            }  
            else {
                auto ptr = std::make_unique<cispp::UniaxialCrystal>(
                    orientation, 
                    tilt_x,
                    tilt_y,
                    thickness, 
                    cut_angle, 
                    material
                );
                components.push_back(std::move(ptr));   
            }       
        }

        else if (nd_comp["QuarterWaveplate"])
        {
            YAML::Node node = nd_comp["QuarterWaveplate"];
            double orientation = node["orientation"].as<double>(0) * M_PI / 180;
            auto ptr = std::make_unique<cispp::QuarterWaveplate>(orientation);
            components.push_back(std::move(ptr));
        }

        else { 
            throw std::logic_error("Interferometer component was not understood.");
        }
    }
}


double Instrument::GetIncidenceAngle(double x, double y, unique_ptr<cispp::Component>& component)
{
    double x0 = lens_3_focal_length * tan(component->tilt_x);
    double y0 = lens_3_focal_length * tan(component->tilt_y);
    return atan2(sqrt(pow(x - x0, 2) + pow(y - y0, 2)), lens_3_focal_length);
}


double Instrument::GetAzimuthalAngle(double x, double y, unique_ptr<cispp::Component>& component)
{
    double x0 = lens_3_focal_length * tan(component->tilt_x);
    double y0 = lens_3_focal_length * tan(component->tilt_y);
    return atan2(y - y0, x - x0) + M_PI - (component->orientation);
}


Eigen::Matrix4d Instrument::GetMuellerMatrix(double x, double y, double wavelength)
{
    Eigen::Matrix4d mtot;
    for (size_t i = 0; i < components.size(); i++)
    {  
        double inc_angle = GetIncidenceAngle(x, y, components[i]);
        double azim_angle = GetAzimuthalAngle(x, y, components[i]);
        Eigen::Matrix4d m = components[i]->GetMuellerMatrix(wavelength, inc_angle, azim_angle);
        if (i==0){
            mtot = m;
        }
        else {
            mtot *= m; 
        }
    }
    if (camera.type == "monochrome_polarised"){
        mtot *= camera.GetMuellerMatrix(x, y);
    }
    return mtot;
}


void Instrument::SaveImage(string fpath, vector<unsigned short int>* image)
{
    std::ofstream file;
    file.open(fpath);
    file << "P2\n" << camera.sensor_format_x << " " << camera.sensor_format_y << "\n255\n";
    for (size_t j = 0; j < camera.sensor_format_y; j++)
    {
        size_t idx_col = j * camera.sensor_format_x;
        for (size_t i = 0; i < camera.sensor_format_x - 1; i++)
        {
            unsigned short int counts = (*image)[i + idx_col];
            file << counts << ' ';
        }
        unsigned short int counts = (*image)[camera.sensor_format_x - 1 + idx_col];
        file << counts << '\n';
    }
    file.close();
}


void Instrument::Capture(double wavelength, double flux, vector<unsigned short int>* image)
{
    assert((*image).size() == camera.sensor_format_x * camera.sensor_format_y);
    
    Eigen::Vector4d stokes_in;
    Eigen::Vector4d stokes_out;
    stokes_in << flux, 0, 0, 0;

    for (size_t iy = 0; iy < camera.sensor_format_y; iy++)
    {
        size_t icol = iy * camera.sensor_format_x;
        double y = camera.pixel_centres_y[iy];
        
        for (size_t ix = 0; ix < camera.sensor_format_x; ix++)
        {
            double x = camera.pixel_centres_x[ix];
            stokes_out = GetMuellerMatrix(x, y, wavelength) * stokes_in;
            (*image)[ix + icol] = static_cast<unsigned short int>(stokes_out[0]);
        }
    }
}


void Instrument::Capture(vector<double>& wavelength, vector<double>& spec_flux, vector<unsigned short int>* image)
{
    assert((*image).size() == camera.sensor_format_x * camera.sensor_format_y);
    assert(wavelength.size() == spec_flux.size());

    Eigen::Vector4d stokes_in;
    stokes_in(1) = 0;
    stokes_in(2) = 0;
    stokes_in(3) = 0;
    Eigen::Vector4d stokes_out;
    vector<double> stokes_out0(wavelength.size(), 0.);

    for (size_t iy = 0; iy < camera.sensor_format_y; iy++)
    {
        size_t icol = iy * camera.sensor_format_x;
        double y = camera.pixel_centres_y[iy];
        for (size_t ix = 0; ix < camera.sensor_format_x; ix++)
        {
            double x = camera.pixel_centres_x[ix];
            
            for (size_t iwl=0; iwl < wavelength.size(); iwl++) 
            {
                double wl = wavelength[iwl];
                stokes_in(0) = spec_flux[iwl];
                stokes_out0[iwl] = (GetMuellerMatrix(x, y, wl) * stokes_in)(0);
            }

            (*image)[ix + icol] = static_cast<unsigned short int>(cispp::trapz(wavelength, stokes_out0));
        }
    }
}


bool InstrumentSingleDelayLinear::TestType(const YAML::Node node)
{
    vector<unique_ptr<cispp::Component>> components_test;
    Instrument::ParseNodeComponents(node["interferometer"], components_test);
    cispp::Camera cam = ParseNodeCamera(node["camera"]);

    size_t n = components_test.size();
    if (n > 2 &&
        cam.type == "monochrome" &&
        components_test[0]->IsIdealPolariser() && 
        components_test[n-1]->IsIdealPolariser() &&
        TestAlign90(components_test[0], components_test[n-1]))
    {
        size_t rcount = 0;
        for (size_t i=1; i<n-1; i++)
        {
            if (components_test[i]->IsIdealRetarder() && 
                TestAlign45(components_test[i], components_test[0])) 
            {
                rcount++;
            }
        }
        if (rcount == n-2) {
            return true;
        }
    }
    return false;
}


void InstrumentSingleDelayLinear::Capture(double wavelength, double flux, vector<unsigned short int>* image)
{
    for (size_t iy = 0; iy < camera.sensor_format_y; iy++) 
    {
        size_t icol = iy * camera.sensor_format_x;
        double y = camera.pixel_centres_y[iy];
        for (size_t ix = 0; ix < camera.sensor_format_x; ix++) 
        {
            double x = camera.pixel_centres_x[ix];
            double inc_angle = GetIncidenceAngle(x, y, components[1]);
            double azim_angle = GetAzimuthalAngle(x, y, components[1]);
            double delay = components[1]->GetDelay(wavelength, inc_angle, azim_angle);
            (*image)[ix + icol] = static_cast<unsigned short int>((flux / 4) * (1 + cos(delay)));
        }
    }
}


void InstrumentSingleDelayLinear::Capture(vector<double>& wavelength, vector<double>& spec_flux, vector<unsigned short int>* image)
{
    assert((*image).size() == camera.sensor_format_x * camera.sensor_format_y);
    assert(wavelength.size() == spec_flux.size());

    Eigen::Vector4d stokes_in;
    stokes_in(1) = 0;
    stokes_in(2) = 0;
    stokes_in(3) = 0;
    Eigen::Vector4d stokes_out;
    vector<double> stokes_out0(wavelength.size(), 0.);

    for (size_t iy = 0; iy < camera.sensor_format_y; iy++)
    {
        size_t icol = iy * camera.sensor_format_x;
        double y = camera.pixel_centres_y[iy];
        for (size_t ix = 0; ix < camera.sensor_format_x; ix++)
        {
            double x = camera.pixel_centres_x[ix];
            double inc_angle = GetIncidenceAngle(x, y, components[1]);
            double azim_angle = GetAzimuthalAngle(x, y, components[1]);
            
            for (size_t iwl=0; iwl < wavelength.size(); iwl++) 
            {
                double wl = wavelength[iwl];
                double delay = components[1]->GetDelay(wl, inc_angle, azim_angle); 
                stokes_out0[iwl] = (spec_flux[iwl] / 4) * (1 + cos(delay));
            }
            (*image)[ix + icol] = static_cast<unsigned short int>(cispp::trapz(wavelength, stokes_out0));
        }
    }
}


bool InstrumentSingleDelayPixelated::TestType(const YAML::Node node)
{
    vector<unique_ptr<cispp::Component>> components_test;
    Instrument::ParseNodeComponents(node["interferometer"], components_test);
    cispp::Camera cam = ParseNodeCamera(node["camera"]);

    size_t n = components_test.size();
    if (n > 2 &&
        cam.type == "monochrome_polarised" &&
        components_test[0]->IsIdealPolariser() && 
        components_test[n-1]->IsIdealQuarterWaveplate() &&
        TestAlign90(components_test[0], components_test[n-1]))
    {
        size_t rcount = 0;
        for (size_t i=1; i<n-1; i++)
        {
            if (components_test[i]->IsIdealRetarder() && 
                TestAlign45(components_test[i], components_test[0])) {
                rcount++;
            }
        }
        if (rcount == n-2) {
            return true;
        }
    }
    return false;
}


void InstrumentSingleDelayPixelated::Capture(double wavelength, double flux, vector<unsigned short int>* image)
{
    for (size_t j = 0; j < camera.sensor_format_y; j++)
    {
        size_t idx_col = j * camera.sensor_format_x;
        double y = camera.pixel_centres_y[j];
        for (size_t i = 0; i < camera.sensor_format_x; i++)
        {
            double x = camera.pixel_centres_x[i];
            double inc_angle = GetIncidenceAngle(x, y, components[1]);
            double azim_angle = GetAzimuthalAngle(x, y, components[1]);
            double delay = components[1]->GetDelay(wavelength, inc_angle, azim_angle);
            double mask = camera.GetPixelatedPhaseMask(x, y);
            (*image)[i + idx_col] = static_cast<unsigned short int>((flux / 4) * (1 + cos(delay + mask)));
        }
    }
}


void InstrumentSingleDelayPixelated::Capture(vector<double>& wavelength, vector<double>& spec_flux, vector<unsigned short int>* image)
{
    assert((*image).size() == camera.sensor_format_x * camera.sensor_format_y);
    assert(wavelength.size() == spec_flux.size());

    Eigen::Vector4d stokes_in;
    stokes_in(1) = 0;
    stokes_in(2) = 0;
    stokes_in(3) = 0;
    Eigen::Vector4d stokes_out;
    vector<double> stokes_out0(wavelength.size(), 0.);

    for (size_t iy = 0; iy < camera.sensor_format_y; iy++)
    {
        size_t icol = iy * camera.sensor_format_x;
        double y = camera.pixel_centres_y[iy];
        for (size_t ix = 0; ix < camera.sensor_format_x; ix++)
        {
            double x = camera.pixel_centres_x[ix];
            double inc_angle = GetIncidenceAngle(x, y, components[1]);
            double azim_angle = GetAzimuthalAngle(x, y, components[1]);
            double mask = camera.GetPixelatedPhaseMask(x, y);

            for (size_t iwl=0; iwl < wavelength.size(); iwl++) 
            {
                double wl = wavelength[iwl];
                double delay = components[1]->GetDelay(wl, inc_angle, azim_angle); 
                stokes_out0[iwl] = (spec_flux[iwl] / 4) * (1 + cos(delay + mask));
            }
            (*image)[ix + icol] = static_cast<unsigned short int>(cispp::trapz(wavelength, stokes_out0));
        }
    }
}


unique_ptr<cispp::Instrument> LoadInstrument(std::filesystem::path fp_config, bool force_mueller)
{
    const YAML::Node node = YAML::LoadFile(fp_config);

    if (!force_mueller && InstrumentSingleDelayLinear::TestType(node)) {
        return std::make_unique<cispp::InstrumentSingleDelayLinear>(fp_config);
    }

    else if (!force_mueller && InstrumentSingleDelayPixelated::TestType(node)) {
        return std::make_unique<cispp::InstrumentSingleDelayPixelated>(fp_config);
    }

    else {
        return std::make_unique<cispp::Instrument>(fp_config);
    }
}


} // namespace cispp