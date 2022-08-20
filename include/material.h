


double get_refractive_index_e()
{

}

double get_refractive_index_o()
{

}

/**
 * @brief 
 * 
 * @param wl_um 
 * @param sellA 
 * @param sellB 
 * @param sellC 
 * @param sellD 
 * @return double 
 */
double sellmeier_eqn(double wl_um, double A, double B, double C, double D)
{
    wl_um2 = pow(wl_um, 2);
    return sqrt(A + (B / (wl_um2 + C)) + (D * wl_um2));
}


/**
 * @brief 
 * 
 * @param wl_um 
 * @param sellA 
 * @param sellB 
 * @param sellC 
 * @param sellD 
 * @return double 
 */
double sellmeier_eqn(double wl_um, double A, double B, double C, double D, double E)
{
    wl_um2 = pow(wl_um, 2);
    return sqrt(A + (B / (wl_um2 + C)) + (D / (wl_um2 + E)));
}


double sellmeier_eqn(double wl_um, double A, double B, double C, double D, double E, double F)
{
    wl_um2 = pow(wl_um, 2);
    
}

