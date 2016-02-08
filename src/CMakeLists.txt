rock_library(envire_gis
    SOURCES Dummy.cpp
    HEADERS Dummy.hpp)

rock_executable(envire_gis_bin Main.cpp
    DEPS envire_gis)

