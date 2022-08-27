#include "yaml-cpp/yaml.h"
#include "include/instrument.h"
#include "include/camera.h"

using std::vector;
using std::unique_ptr;
using std::string;


namespace cispp {


/**
* @brief Construct Instrument from .YAML config file
* 
* @param fp_config 
*/
Instrument::Instrument(string fp_config)
: fp_config(fp_config)
{
    YAML::Node nd_config = YAML::LoadFile(fp_config);
    YAML::Node nd_cam = nd_config["camera"];
    YAML::Node nd_int = nd_config["interferometer"];

    lens_1_focal_length = nd_config["lens_1_focal_length"].as<double>();
    lens_2_focal_length = nd_config["lens_2_focal_length"].as<double>();
    lens_3_focal_length = nd_config["lens_3_focal_length"].as<double>();

    camera = cispp::Camera(
        nd_cam["sensor_format"][0].as<int>(),  // sensor_format_x
        nd_cam["sensor_format"][1].as<int>(),  // sensor_format_y
        nd_cam["pixel_size"].as<double>(),
        nd_cam["bit_depth"].as<int>(),
        nd_cam["qe"].as<double>(),
        nd_cam["epercount"].as<double>(),
        nd_cam["cam_noise"].as<double>(),
        nd_cam["type"].as<string>()
    );

    // loop over interferometer components
    for (size_t i = 0; i < nd_int.size(); i++)
    {
        if (nd_int[i]["LinearPolariser"]){
            std::cout << "LinearPolariser" << '\n';
            double orientation = nd_int[i]["LinearPolariser"]["orientation"].as<double>(0) * M_PI / 180;
            auto ptr = std::make_unique<cispp::Polariser>(orientation);
            interferometer.push_back(std::move(ptr));
        }

        else if (nd_int[i]["UniaxialCrystal"]){
            std::cout << "UniaxialCrystal" << '\n';
            double orientation = nd_int[i]["UniaxialCrystal"]["orientation"].as<double>() * M_PI / 180;
            double tilt_x = nd_int[i]["UniaxialCrystal"]["tilt_x"].as<double>(0);
            double tilt_y = nd_int[i]["UniaxialCrystal"]["tilt_y"].as<double>(0);
            double thickness = nd_int[i]["UniaxialCrystal"]["thickness"].as<double>();
            double cut_angle = nd_int[i]["UniaxialCrystal"]["cut_angle"].as<double>() * M_PI / 180;
            std::string material = nd_int[i]["UniaxialCrystal"]["material"].as<std::string>();
            
            auto ptr = std::make_unique<cispp::UniaxialCrystal>(
                orientation, 
                tilt_x,
                tilt_y,
                thickness, 
                cut_angle, 
                material
            );
            interferometer.push_back(std::move(ptr));    

            // if (nd_int[i]["UniaxialCrystal"]["sellmeier_coefs"]){
            //     std::cout << "sellmeier coefficients found" << std::endl;
            // }    
        }

        else if (nd_int[i]["QuarterWaveplate"]){
            std::cout << "QuarterWaveplate" << '\n';
            double orientation = nd_int[i]["QuarterWaveplate"]["orientation"].as<double>(0) * M_PI / 180;
            auto ptr = std::make_unique<cispp::QuarterWaveplate>(orientation);
            interferometer.push_back(std::move(ptr));
        }
        
        else { 
            throw std::logic_error("Interferometer component was not understood.");
        }
    }

    type = get_type();
}


/**
* @brief Incidence angle in radians of ray through interferometer component
* 
* @param x x position on sensor plane in metres
* @param y y position on sensor plane in metres
* @param component unique pointer to interferometer component
* @return double 
*/
double Instrument::get_inc_angle(double x, double y, unique_ptr<cispp::Component>& component)
{
    double x0 = lens_3_focal_length * tan(component->tilt_x);
    double y0 = lens_3_focal_length * tan(component->tilt_y);
    return atan2(sqrt(pow(x-x0,2) + pow(y-y0,2)), lens_3_focal_length);
}


/**
* @brief Azimuthal angle in radians of ray through interfereometer component
* 
* @param x x position on sensor plane in metres
* @param y y position on sensor plane in metres
* @param component unique pointer to interferometer component
* @return double 
*/
double Instrument::get_azim_angle(double x, double y, unique_ptr<cispp::Component>& component)
{
    double x0 = lens_3_focal_length * tan(component->tilt_x);
    double y0 = lens_3_focal_length * tan(component->tilt_y);
    return atan2(y-y0, x-x0) + M_PI - (component->orientation);
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
    for (size_t i = 0; i < interferometer.size(); i++)
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


void Instrument::image_to_file(string fpath, vector<unsigned short int>* image)
{
    std::ofstream file;
    file.open(fpath);
    file << "P3\n" << camera.sensor_format_x << " " << camera.sensor_format_y << "\n255\n";
    for (size_t j = 0; j < camera.sensor_format_y; j++) {
        size_t idx_col = j * camera.sensor_format_x;
        for (size_t i = 0; i < camera.sensor_format_x; i++) {
            unsigned short int counts = (*image)[i + idx_col];
            file << counts << ' ' << counts << ' ' << counts << '\n';
        }
    }
    file.close();
}


 std::string Instrument::get_type()
 {
    if (test_type_single_delay_linear()) {
        return "single_delay_linear";
    }
    else if (test_type_single_delay_pixelated()) {
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
    // size_t ilast = interferometer.size() - 1;
    // if (interferometer.size() > 2 &&
    //     camera.type == "monochrome" &&
    //     interferometer[0]->name == "Polariser" && 
    //     interferometer[ilast]->name == "Polariser" &&
    //     test_align90(interferometer[0], interferometer[ilast]))
    // {
    //     size_t rcount = 0;
    //     for (size_t i=1; i<ilast; i++)
    //     {
    //         if (interferometer[i]->name == "Retarder" && 
    //             test_align45(interferometer[i], interferometer[0])){
    //             rcount++;
    //         }
    //     }
    //     if (rcount == interferometer.size() - 2){
    //         return true;
    //     }
    // }
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
    // size_t ilast = interferometer.size() - 1;
    // std::cout << "interferometer[0]->name = " << interferometer[0]->name << std::endl;
    // std::cout << "interferometer[ilast]->name = " << interferometer[ilast]->name << std::endl;

    // if (interferometer.size() > 2 &&
    //     camera.type == "monochrome_polarised" &&
    //     interferometer[0]->name == "Polariser" && 
    //     interferometer[ilast]->name == "QuarterWaveplate" &&
    //     test_align90(interferometer[0], interferometer[ilast]))
    // {
    //     return true;
    // }
    return false;
}


/**
 * @brief Capture interferogram for a uniform scene of monochromatic, unpolarised light (general)
 * 
 * This is really an approximation: perfectly monochromatic, unpolarised light is impossible!
 * @param wavelength wavelength of light in metres
 * @param flux photon flux
 * @param image pointer to image vector (row-major order)
 */
void Instrument::capture(double wavelength, double flux, vector<unsigned short int>* image)
{
    if (type == "single_delay_linear") {
        capture_single_delay_linear(wavelength, flux, image);
    }
    else if (type == "single_delay_pixelated") {
        capture_single_delay_pixelated(wavelength, flux, image);
    }
    else {
        capture_mueller(wavelength, flux, image);
    }
}


/**
 * @brief Capture interferogram for a uniform scene of monochromatic, unpolarised light (Mueller model)
 * 
 * @param wavelength wavelength of light in metres
 * @param flux photon flux
 * @param image pointer to image vector (row-major order)
 */
void Instrument::capture_mueller(double wavelength, double flux, vector<unsigned short int>* image)
{
    assert((*image).size() == camera.sensor_format_x * camera.sensor_format_y);
    Eigen::Vector4d stokes_in;
    Eigen::Vector4d stokes_out;
    stokes_in[0] = flux;
    stokes_in[1] = 0;
    stokes_in[2] = 0;
    stokes_in[3] = 0;
    for (size_t j = 0; j < camera.sensor_format_y; j++){
        size_t idx_col = j * camera.sensor_format_x;
        for (size_t i = 0; i < camera.sensor_format_x; i++){
            stokes_out = get_mueller_matrix(
                camera.pixel_centres_x[i], 
                camera.pixel_centres_y[j], 
                wavelength
            ) * stokes_in;
            (*image)[i + idx_col] = static_cast<unsigned short int>(stokes_out[0]);
        }
    }
}


/**
 * @brief Capture interferogram for a uniform scene of monochromatic, unpolarised light (single_delay_linear)
 * 
 * @param wavelength wavelength of light in metres
 * @param flux photon flux
 * @param image pointer to image vector (row-major order)
 */
void Instrument::capture_single_delay_linear(double wavelength, double flux, vector<unsigned short int>* image)
{
    for (size_t j = 0; j < camera.sensor_format_y; j++) {
        size_t idx_col = j * camera.sensor_format_x;
        for (size_t i = 0; i < camera.sensor_format_x; i++) {
            double inc_angle = get_inc_angle(
                camera.pixel_centres_x[i], 
                camera.pixel_centres_y[j], 
                interferometer[1]
            );
            double azim_angle = get_azim_angle(
                camera.pixel_centres_x[i], 
                camera.pixel_centres_y[j], 
                interferometer[1]
            );
            double delay = interferometer[1]->get_delay(wavelength, inc_angle, azim_angle);
            (*image)[i + idx_col] = static_cast<unsigned short int>((flux / 4) * (1 + cos(delay)));
        }
    }
}

/**
 * @brief Capture interferogram for a uniform scene of monochromatic, unpolarised light (single_delay_pixelated)
 * 
 * @param wavelength wavelength of light in metres
 * @param flux photon flux
 * @param image pointer to image vector (row-major order)
 */
void Instrument::capture_single_delay_pixelated(double wavelength, double flux, vector<unsigned short int>* image)
{
    for (size_t j = 0; j < camera.sensor_format_y; j++) {
        size_t idx_col = j * camera.sensor_format_x;
        for (size_t i = 0; i < camera.sensor_format_x; i++) {
            double inc_angle = get_inc_angle(
                camera.pixel_centres_x[i], 
                camera.pixel_centres_y[j], 
                interferometer[1]
            );
            double azim_angle = get_azim_angle(
                camera.pixel_centres_x[i], 
                camera.pixel_centres_y[j], 
                interferometer[1]
            );
            double delay = interferometer[1]->get_delay(wavelength, inc_angle, azim_angle);
            double mask = camera.get_pixelated_phase_mask(
                camera.pixel_centres_x[i],
                camera.pixel_centres_y[j]
            );
            (*image)[i + idx_col] = static_cast<unsigned short int>((flux / 4) * (1 + cos(delay + mask)));
        }
    }
}


} // namespace cispp