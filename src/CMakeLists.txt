rock_library(envire_gis
    SOURCES EnvireNetwork.cpp
    HEADERS EnvireNetwork.hpp
            RasterPluginBand.hpp
            RasterPlugin.hpp
    DEPS_CMAKE GDAL
    DEPS_PKGCONFIG envire_core)

