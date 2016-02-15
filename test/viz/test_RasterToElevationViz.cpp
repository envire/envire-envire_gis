#define BOOST_TEST_MODULE EnvireGisVizTest
#include <boost/test/unit_test.hpp>

#include <envire_gis/RasterMap.hpp>

#include <vizkit3d/Vizkit3DWidget.hpp>
#include <vizkit3d/QtThreadedWidget.hpp>
#include <vizkit3d/ElevationMapVisualization.hpp>
#include <vizkit3d/GridVisualization.hpp>


BOOST_AUTO_TEST_CASE(raster_to_elevation_viz)
{

    envire::gis::RasterMap raster;
    if (boost::unit_test::framework::master_test_suite().argc > 1.0)
    {
        /** Read the Raster into a GridMap **/
        raster.open(static_cast<std::string>(boost::unit_test::framework::master_test_suite().argv[1]));

        envire::maps::GridMap<double> grid_map;

        raster.toGridMap(1);

        /** Convert into an ElevationMap **/
        envire::maps::ElevationMap elev_map(envire::maps::Vector2ui(150, 250), envire::maps::Vector2d(0.1, 0.1));
        elev_map.localFrame().translation() << -0.5 * elev_map.getSize(), 0;

        for (unsigned int x = 10; x < elev_map.getNumCells().x() - 10; ++x)
        {
            float cs = std::cos(x * M_PI/50);
            for (unsigned int y = 10; y < elev_map.getNumCells().y() - 10; ++y)
            {
                float sn = std::sin(y * M_PI/50);

                elev_map.at(x,y).elevation = cs * sn;
                elev_map.at(x,y).elevation_min = -1 * cs * sn;
            }
        }

        // set up test environment
        QtThreadedWidget<vizkit3d::Vizkit3DWidget> app;
        app.start();

        //create vizkit3d plugin for showing envire
        vizkit3d::ElevationMapVisualization *elev_viz = new vizkit3d::ElevationMapVisualization();
        elev_viz->updateData(elev_map);

        //create vizkit3d widget
        vizkit3d::Vizkit3DWidget *widget = app.getWidget();
        // grid plugin
        vizkit3d::GridVisualization *grid_viz = new vizkit3d::GridVisualization();
        widget->addPlugin(grid_viz);
        // add envire plugin
        widget->addPlugin(elev_viz);

        while (app.isRunning())
        {
            usleep(1000);
        }
    }
    else
    {
        std::cout<<"Missing Raster file as argument\n";
    }

}

