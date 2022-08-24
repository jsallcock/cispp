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
    YAML::Node nd_config = YAML::LoadFile(fp_config);
    YAML::Node nd_cam = nd_config["camera"];
    YAML::Node nd_int = nd_config["interferometer"];

    lens_1_focal_length = nd_config["lens_1_focal_length"].as<double>();
    lens_2_focal_length = nd_config["lens_2_focal_length"].as<double>();
    lens_3_focal_length = nd_config["lens_3_focal_length"].as<double>();

    camera = Camera(
        nd_cam["sensor_format"][0].as<int>(),  // sensor_format_x
        nd_cam["sensor_format"][1].as<int>(),  // sensor_format_y
        nd_cam["pixel_size"].as<double>(),
        nd_cam["bit_depth"].as<int>(),
        nd_cam["qe"].as<double>(),
        nd_cam["epercount"].as<double>(),
        nd_cam["cam_noise"].as<double>(),
        nd_cam["type"].as<std::string>()
    );

    // loop over interferometer components
    for (std::size_t i = 0; i < nd_int.size(); i++)
    {
        if (nd_int[i]["LinearPolariser"]){
            double orientation = nd_int[i]["LinearPolariser"]["orientation"].as<double>();
            // std::unique_ptr<Component> ptr = std::make_unique<Polariser>(orientation);
            auto ptr = std::make_unique<Polariser>(orientation);
            interferometer.push_back(std::move(ptr));
        }

        else if (nd_int[i]["UniaxialCrystal"]){
            double orientation = nd_int[i]["UniaxialCrystal"]["orientation"].as<double>();
            double thickness = nd_int[i]["UniaxialCrystal"]["thickness"].as<double>();
            double cut_angle = nd_int[i]["UniaxialCrystal"]["cut_angle"].as<double>();
            std::string material = nd_int[i]["UniaxialCrystal"]["material"].as<std::string>();
            
            // std::unique_ptr<Component> ptr = std::make_unique<UniaxialCrystal>(orientation, thickness, cut_angle, material);
            auto ptr = std::make_unique<UniaxialCrystal>(orientation, thickness, cut_angle, material);
            interferometer.push_back(std::move(ptr));    

            if (nd_int[i]["UniaxialCrystal"]["sellmeier_coefs"]){
                std::cout << "sellmeier coefficients found" << std::endl;
            }    
        }

        else if (nd_int[i]["QuarterWaveplate"]){
            double orientation = nd_int[i]["QuarterWaveplate"]["orientation"].as<double>();
            // std::unique_ptr<Component> ptr = std::make_unique<UniaxialCrystal>(orientation, thickness, cut_angle, material);
            auto ptr = std::make_unique<QuarterWaveplate>(orientation);
            interferometer.push_back(std::move(ptr));
        }
        
        else { 
            throw std::logic_error("Interferometer component was not understood.");
        }
    }

    type = get_type();
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
    return atan2(y, x) + M_PI - (component->orientation * M_PI / 180);
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
        Eigen::Matrix4d m = interferometer[i]->get_mueller_matrix(wavelength, inc_angle, azim_angle);
        if (i==0){
            mtot = m;
        }
        else {
            mtot *= m; 
        }
    }
    if (camera.type == "monochrome_polarised"){
        mtot *= camera.get_mueller_matrix(x, y);
    }
    return mtot;
}


/**
* @brief Capture interferogram for a uniform scene of monochromatic, unpolarised light
* 
* (But remember monochromatic, unpolarised light is impossible!)
* Saves image to PPM format for testing.
* @param wavelength 
*/
void Instrument::capture(double wavelength, double flux)
{
    std::vector<double> x = camera.get_pixel_centres_x();
    std::vector<double> y = camera.get_pixel_centres_y();
    std::ofstream file;
    file.open ("out.ppm");
    // int max_counts = 255;
    file << "P3\n" << camera.sensor_format_x << " " << camera.sensor_format_y << "\n255\n";


    if (type == "single_delay_linear")
    {
        std::cout << "capture: single_delay_linear" << std::endl;
        for (std::size_t j = 0; j < y.size(); j++){
            for (std::size_t i = 0; i < x.size(); i++){
                double inc_angle = get_inc_angle(x[i], y[j], interferometer[1]);
                double azim_angle = get_azim_angle(x[i], y[j], interferometer[1]);
                double delay = interferometer[1]->get_delay(wavelength, inc_angle, azim_angle);
                int counts = static_cast<int>((flux / 4) * (1 + cos(delay)));

                file << counts << ' ' << counts << ' ' << counts << '\n';
            }
        }
        file.close();
    }

    else if (type == "single_delay_pixelated")
    {
        std::cout << "capture: single_delay_pixelated" << std::endl;
        for (std::size_t j = 0; j < y.size(); j++){
            for (std::size_t i = 0; i < x.size(); i++){
                double inc_angle = get_inc_angle(x[i], y[j], interferometer[1]);
                double azim_angle = get_azim_angle(x[i], y[j], interferometer[1]);
                double delay = interferometer[1]->get_delay(wavelength, inc_angle, azim_angle);
                double mask = camera.get_pixelated_phase_mask(x[i], y[j]);
                int counts = static_cast<int>((flux / 4) * (1 + cos(delay + mask)));

                file << counts << ' ' << counts << ' ' << counts << '\n';
            }
        }
        file.close();
    }


    else if (type == "mueller")
    {
        std::cout << "capture: mueller" << std::endl;
        Eigen::Vector4d stokes_in;
        Eigen::Vector4d stokes_out;
        stokes_in[0] = flux;
        stokes_in[1] = 0;
        stokes_in[2] = 0;
        stokes_in[3] = 0;

        for (std::size_t j = 0; j < y.size(); j++){
            for (std::size_t i = 0; i < x.size(); i++){
                stokes_out = get_mueller_matrix(x[i], y[j], wavelength) * stokes_in;
                int counts = static_cast<int>(stokes_out[0]);

                file << counts << ' ' << counts << ' ' << counts << '\n';
            }
        }
        file.close();
    }

    else {
        throw std::logic_error("invalid instrument type");
    }
}


/**
 * @brief determine instrument type based on configuraiton
 * 
 * @return std::string 
 */
 std::string Instrument::get_type()
 {
    if (test_type_single_delay_linear()){
        return "single_delay_linear";
    }

    else if (test_type_single_delay_pixelated()){
        return "single_delay_pixelated";
    }

    else return "mueller";
 }


/**
 * @brief test if instrument configuration matches "single_delay_linear" type
 * 
 * @return true 
 * @return false 
 */
bool Instrument::test_type_single_delay_linear()
{
    size_t ilast = interferometer.size() - 1;
    if (interferometer.size() > 2 &&
        camera.type == "monochrome" &&
        interferometer[0]->name == "Polariser" && 
        interferometer[ilast]->name == "Polariser" &&
        test_align90(interferometer[0], interferometer[ilast]))
    {
        size_t rcount = 0;
        for (int i=1; i<ilast; i++)
        {
            if (interferometer[i]->name == "Retarder" && 
                test_align45(interferometer[i], interferometer[0])){
                rcount++;
            }
        }
        if (rcount == interferometer.size() - 2){
            return true;
        }
    }
    return false;
}


/**
 * @brief test if instrument configuration matches "single_delay_pixelated" type
 * 
 * @return true 
 * @return false 
 */
bool Instrument::test_type_single_delay_pixelated()
{
    size_t ilast = interferometer.size() - 1;
    std::cout << "interferometer[0]->name = " << interferometer[0]->name << std::endl;
    std::cout << "interferometer[ilast]->name = " << interferometer[ilast]->name << std::endl;

    if (interferometer.size() > 2 &&
        camera.type == "monochrome_polarised" &&
        interferometer[0]->name == "Polariser" && 
        interferometer[ilast]->name == "QuarterWaveplate" &&
        test_align90(interferometer[0], interferometer[ilast]))
    {
        return true;
    }
    return false;
}