include_directories(BEFORE ${PROJECT_SOURCE_DIR}/viz)

rock_testsuite(test_raster_to_elevation_viz test_RasterToElevationViz.cpp
    DEPS envire_gis
    DEPS_PKGCONFIG vizkit3d  vizkit3d-viz envire_maps envire_maps-viz)
