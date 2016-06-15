rock_library(envire_gis
    HEADERS EnvireNetwork.hpp
            RasterPluginBand.hpp
            RasterPlugin.hpp
    DEPS_CMAKE GDAL
    DEPS_PKGCONFIG envire_core)

