#include <iostream>
#include <envire_gis/RasterMap.hpp>

int main(int argc, char** argv)
{
    envire::gis::RasterMap raster;
    raster.welcome();
    raster.open(static_cast<std::string>(argv[1]));

    envire::maps::GridMap<double>::Ptr grid_map;

    grid_map = raster.toGridMap<double>(0);

    //raster.close();

    return 0;
}
