# CMakeLists.txt has to be located in the project folder and cmake has to be
# executed from 'project/build' with 'cmake ../'.
cmake_minimum_required(VERSION 2.6)
find_package(Rock)
set(ROCK_USE_CXX11 TRUE)
set(ROCK_TEST_ENABLED ON)

rock_init(envire_gis 0.1)
rock_standard_layout()

# install EnvireGIS.cmake extension
configure_file(cmake/enviregis-config.cmake.in enviregis-config.cmake @ONLY)
install(FILES cmake/EnvireGIS.cmake
              ${CMAKE_CURRENT_BINARY_DIR}/enviregis-config.cmake
        DESTINATION share/rock/cmake)
