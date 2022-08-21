#include <iostream>
#include "../include/material.h"


int main()
{
    MaterialProperties material_properties_empty;
    std::cout << material_properties_empty.name << std::endl;
    std::cout << material_properties_empty.sellAe << std::endl;
    std::cout << material_properties_empty.sellBe << std::endl;
    std::cout << material_properties_empty.sellCe << std::endl;

    MaterialProperties material_properties = get_material_properties("a-BBO");
    std::cout << material_properties.name << std::endl;
    std::cout << material_properties.sellAe << std::endl;
    std::cout << material_properties.sellBe << std::endl;
    std::cout << material_properties.sellCe << std::endl;
}