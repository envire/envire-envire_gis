rock_library(envire_gis
    SOURCES RasterMap.cpp
    HEADERS RasterMap.hpp
    DEPS_CMAKE GDAL
    DEPS_PKGCONFIG envire_core envire_maps)

