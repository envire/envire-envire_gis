#define BOOST_TEST_MODULE EnvireGisVizTest
#include <boost/test/unit_test.hpp>

#include <envire_gis/Raster.hpp>

#include <vizkit3d/Vizkit3DWidget.hpp>
#include <vizkit3d/QtThreadedWidget.hpp>
#include <vizkit3d/ElevationMapVisualization.hpp>
#include <vizkit3d/GridVisualization.hpp>


BOOST_AUTO_TEST_CASE(raster_to_elevation_viz)
{
    envire::gis::Raster raster;

    if (boost::unit_test::framework::master_test_suite().argc > 1.0)
    {
        /** Read the Raster into a GridMap **/
        raster.open(static_cast<std::string>(boost::unit_test::framework::master_test_suite().argv[1]));
        envire::maps::GridMap<float> grid_map;
        raster.toGridMap(1, grid_map);

        /** Get the minimum value **/
        float minimum = grid_map.getMin(false);
        std::cout<<"Default value grid_map: "<<grid_map.getDefaultValue()<<"\n";
        std::cout<<"Maximum grid_map: "<<grid_map.getMax()<<"\n";
        std::cout<<"Minimum grid_map: "<<minimum<<"\n";

        /** Convert into an ElevationMap **/
        envire::maps::ElevationMap elev_map(grid_map.getNumCells(), grid_map.getResolution());
        elev_map.getLocalFrame().translation() << -0.5 * elev_map.getSize(), 0;

        for (unsigned int x = 0; x < elev_map.getNumCells().x(); ++x)
        {
            for (unsigned int y = 0; y < elev_map.getNumCells().y(); ++y)
            {
                if (grid_map.at(x,y) != grid_map.getDefaultValue())
                {
                    elev_map.at(x,y).elevation = static_cast<double>(grid_map.at(x,y)-minimum);
                }
            }
        }

        /** set up test environment **/
        QtThreadedWidget<vizkit3d::Vizkit3DWidget> app;
        app.start();

        /** create vizkit3d plugin for showing envire **/
        vizkit3d::ElevationMapVisualization *elev_viz = new vizkit3d::ElevationMapVisualization();
        elev_viz->updateData(elev_map);

        /** create vizkit3d widget **/
        vizkit3d::Vizkit3DWidget *widget = app.getWidget();

        /** grid plugin **/
        vizkit3d::GridVisualization *grid_viz = new vizkit3d::GridVisualization();
        widget->addPlugin(grid_viz);

        /** add envire plugin **/
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

