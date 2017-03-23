//
// Copyright (c) 2015, Deutsches Forschungszentrum für Künstliche Intelligenz GmbH.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.
//
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//

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

