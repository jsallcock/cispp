#include "yaml-cpp/yaml.h"
#include "../include/instrument.h"
#include "../include/camera.h"


/**
* @brief Construct Instrument from .YAML config file
* 
* @param fp_config 
*/
Instrument::Instrument(std::string fp_config)
: fp_config(fp_config)
{
    YAML::Node config = YAML::LoadFile(fp_config);

    lens_1_focal_length = config["lens_1_focal_length"].as<double>();
    lens_2_focal_length = config["lens_2_focal_length"].as<double>();
    lens_3_focal_length = config["lens_3_focal_length"].as<double>();

    camera = Camera(
        config["camera"]["sensor_format"][0].as<int>(),  // sensor_format_x
        config["camera"]["sensor_format"][1].as<int>(),  // sensor_format_y
        config["camera"]["pixel_size"].as<double>(),
        config["camera"]["bit_depth"].as<int>(),
        config["camera"]["qe"].as<double>(),
        config["camera"]["epercount"].as<double>(),
        config["camera"]["cam_noise"].as<double>()
    );

    YAML::Node node = config["interferometer"];

    // loop over interferometer components
    for (std::size_t i = 0; i < node.size(); i++)
    {
        if (node[i]["LinearPolariser"]){
            double orientation = node[i]["LinearPolariser"]["orientation"].as<double>();
            std::unique_ptr<Component> ptr = std::make_unique<Polariser>(orientation);
            interferometer.push_back(std::move(ptr));
        }

        else if (node[i]["UniaxialCrystal"]){
            double orientation = node[i]["UniaxialCrystal"]["orientation"].as<double>();
            double thickness = node[i]["UniaxialCrystal"]["thickness"].as<double>();
            double cut_angle = node[i]["UniaxialCrystal"]["cut_angle"].as<double>();
            std::string material = node[i]["UniaxialCrystal"]["material"].as<std::string>();
            
            std::unique_ptr<Component> ptr = std::make_unique<UniaxialCrystal>(orientation, thickness, cut_angle, material);
            interferometer.push_back(std::move(ptr));    

            if (node[i]["UniaxialCrystal"]["sellmeier_coefs"]){
                std::cout << "sellmeier coefficients found" << std::endl;
            }    
        }
        
        else { 
            throw std::logic_error("Interferometer component was not understood.");
        }
    }
}



/**
* @brief Incidence angle of ray through component in radians
* 
* @param x x position on sensor plane in metres
* @param y y position on sensor plane in metres
* @param component unique pointer to interferometer component
* @return double 
*/
double Instrument::get_inc_angle(double x, double y, std::unique_ptr<Component>& component)
{
    return atan2(sqrt(pow(x,2) + pow(y,2)), lens_3_focal_length);
}
    

/**
* @brief Azimuthal angle of ray through component in radians
* 
* @param x x position on sensor plane in metres
* @param y y position on sensor plane in metres
* @param component unique pointer to interferometer component
* @return double 
*/
double Instrument::get_azim_angle(double x, double y, std::unique_ptr<Component>& component)
{
    return atan2(y, x) + M_PI - component->orientation;
}


/**
* @brief Total Mueller matrix for interferometer
* 
* @param x x position on sensor plane in metres
* @param y y position on sensor plane in metres
* @param wavelength wavelength of ray
* @return Eigen::Matrix4d 
*/
Eigen::Matrix4d Instrument::get_mueller_matrix(double x, double y, double wavelength)
{
    Eigen::Matrix4d mtot;

    for (std::size_t i = 0; i < interferometer.size(); i++)
    {  
        double inc_angle = get_inc_angle(x, y, interferometer[i]);
        double azim_angle = get_azim_angle(x, y, interferometer[i]);
        Eigen::Matrix4d m_component = interferometer[i]->get_mueller_matrix(wavelength, inc_angle, azim_angle);
        if (i==0){
            mtot = m_component;
        }
        else {
            mtot *= m_component; 
        }
    }
    return mtot;
}


/**
* @brief capture interferogram for a uniform scene of monochromatic, unpolarised light
* 
* (But remember monochromatic, unpolarised light is impossible!)
* Saves image to PPM format for testing.
* @param wavelength 
*/
void Instrument::capture(double wavelength, double flux)
{
    std::vector<double> x = camera.get_pixel_positions_x();
    std::vector<double> y = camera.get_pixel_positions_y();
    Eigen::Vector4d stokes_in;
    Eigen::Vector4d stokes_out;
    stokes_in[0] = 500;
    stokes_in[1] = 0;
    stokes_in[2] = 0;
    stokes_in[3] = 0;

    std::ofstream file;
    file.open ("out.ppm");
    // int max_counts = 255;
    file << "P3\n" << camera.sensor_format_x << " " << camera.sensor_format_y << "\n255\n";

    for (std::size_t j = 0; j < y.size(); j++){
        for (std::size_t i = 0; i < x.size(); i++){
            stokes_out = get_mueller_matrix(x[i], y[j], wavelength) * stokes_in;
            int counts = static_cast<int>(stokes_out[0]);
            file << counts << ' ' << counts << ' ' << counts << '\n';
        }
    }
    file.close();
}