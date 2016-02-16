#include <boost/test/unit_test.hpp>
#include <envire_gis/RasterMap.hpp>

using namespace envire::gis;

BOOST_AUTO_TEST_CASE(test_read_raster)
{
    envire::gis::RasterMap raster;
    raster.welcome();
    if (boost::unit_test::framework::master_test_suite().argc > 1.0)
    {
        raster.open(static_cast<std::string>(boost::unit_test::framework::master_test_suite().argv[1]));

        envire::maps::GridMap<float> grid_map;

        raster.toGridMap(1, grid_map);
    }
    else
    {
        std::cout<<"Missing Raster file as argument\n";
    }
}
