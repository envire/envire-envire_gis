# class_loader (and rock.core package_set patch) setting boost targets as libs
# so we need search for them here
find_package(Boost COMPONENTS system thread)

rock_library(envire_gis
    SOURCES EnvireNetwork.cpp
    HEADERS EnvireNetwork.hpp
            RasterPluginBand.hpp
            RasterPlugin.hpp
    DEPS_CMAKE GDAL
    DEPS_PKGCONFIG envire_core)

