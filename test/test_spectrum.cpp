#include "include/spectrum.h"


int main()
{
    cispp::Spectrum spec = cispp::gaussian(465e-9, 1e-9, 1000, 1000, 5);
    return 0;
}