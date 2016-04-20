rock_library(envire_gis
    SOURCES Raster.cpp
    HEADERS Raster.hpp
    DEPS_CMAKE GDAL
    DEPS_PKGCONFIG envire_core envire_maps)

