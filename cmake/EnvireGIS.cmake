# Allow to find and build the envire gis plugins
macro(envire_gis_plugin_layout)
    find_package(Rock)
    rock_find_pkgconfig(envire_gis envire_gis)

    if(envire_gis_FOUND)
        add_subdirectory(gis)
        message(STATUS "gis found ... building envire_maps gis plugin")
    endif()
endmacro()

## Defines a new envire_gis plugin
#
# envire_gis_plugin(name
#     SOURCES source.cpp source1.cpp ...
#     [DEPS target1 target2 target3]
#     [DEPS_PKGCONFIG pkg1 pkg2 pkg3]
#     [DEPS_CMAKE pkg1 pkg2 pkg3]
#     [HEADERS header1.hpp header2.hpp header3.hpp ...]
#     [MOC qtsource1.hpp qtsource2.hpp]
#     [NOINSTALL])
#
# Creates and (optionally) installs a shared library that defines an envire_gis
# plugin. In Rock, envire_gis is the base for environment representation and 
# GIS conversion.
#
# The library gets linked against the envire_gis libraries automatically (no
# need to list them in DEPS_PKGCONFIG). Moreoer, unlike with a normal shared
# library, the headers get installed in include/envire_gis
#
# The following arguments are mandatory:
#
# SOURCES: list of the C++ sources that should be built into that library
#
# The following optional arguments are available:
#
# DEPS: lists the other targets from this CMake project against which the
# library should be linked
# DEPS_PKGCONFIG: list of pkg-config packages that the library depends upon. The
# necessary link and compilation flags are added
# DEPS_CMAKE: list of packages which can be found with CMake's find_package,
# that the library depends upon. It is assumed that the Find*.cmake scripts
# follow the cmake accepted standard for variable naming
# HEADERS: a list of headers that should be installed with the library. They get
# installed in include/project_name
# MOC: if the library is Qt-based, a list of either source or header files.
# If headers are listed, these headers should be processed by moc, with the
# resulting implementation files are built into the library. If they are source
# files, they get added to the library and the corresponding header file is
# passed to moc.
# NOINSTALL: by default, the library gets installed on 'make install'. If this
# argument is given, this is turned off
function(envire_gis_plugin TARGET_NAME)
    if (${PROJECT_NAME} STREQUAL "envire_gis")
    else()
        list(APPEND additional_deps DEPS_PKGCONFIG envire_gis)
    endif()
    rock_library_common(${TARGET_NAME} ${ARGN} ${additional_deps})
    if (${TARGET_NAME}_INSTALL)
        if (${TARGET_NAME}_LIBRARY_HAS_TARGET)
            install(TARGETS ${TARGET_NAME}
                LIBRARY DESTINATION lib)
        endif()
        install(FILES ${${TARGET_NAME}_HEADERS}
            DESTINATION include/envire_gis)
    endif()
endfunction()


