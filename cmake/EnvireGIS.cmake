# Allow to find and build the envire gis plugins
macro(envire_gis_plugin_layout)
    if (EXISTS ${PROJECT_SOURCE_DIR}/Doxyfile.in)
        rock_doxygen()
    endif()

    if(IS_DIRECTORY ${PROJECT_SOURCE_DIR}/src)
        rock_add_source_dir(src ${PROJECT_NAME})
    endif()

    # Test for known types of Rock subprojects
    if(IS_DIRECTORY ${PROJECT_SOURCE_DIR}/viz)
        option(ROCK_VIZ_ENABLED "set to OFF to disable the visualization plugin. Visualization plugins are automatically disabled if Rock's vizkit3d is not available" ON)
        if (ROCK_VIZ_ENABLED)
            if ("${PROJECT_NAME}" STREQUAL vizkit3d)
                add_subdirectory(viz)
            else()
                rock_find_pkgconfig(vizkit3d vizkit3d)
                if (vizkit3d_FOUND)
                    message(STATUS "vizkit3d found ... building the vizkit3d plugin")
                    rock_add_source_dir(viz vizkit3d)
                else()
                    message(STATUS "vizkit3d not found ... NOT building the vizkit3d plugin")
                endif()
            endif()
        else()
            message(STATUS "visualization plugins disabled as ROCK_VIZ_ENABLED is set to OFF")
        endif()
    endif()

    if (IS_DIRECTORY ${PROJECT_SOURCE_DIR}/ruby)
        if (EXISTS ${PROJECT_SOURCE_DIR}/ruby/CMakeLists.txt)
            include(RockRuby)
            if (RUBY_FOUND)
                add_subdirectory(ruby)
            endif()
        endif()
    endif()

    if (IS_DIRECTORY ${PROJECT_SOURCE_DIR}/bindings/ruby)
        if (EXISTS ${PROJECT_SOURCE_DIR}/bindings/ruby/CMakeLists.txt)
            include(RockRuby)
            if (RUBY_FOUND)
                add_subdirectory(bindings/ruby)
            endif()
        endif()
    endif()

    if (IS_DIRECTORY ${PROJECT_SOURCE_DIR}/configuration)
	install(DIRECTORY ${PROJECT_SOURCE_DIR}/configuration/ DESTINATION configuration/${PROJECT_NAME}
	        FILES_MATCHING PATTERN "*" 
	                       PATTERN "*.pc" EXCLUDE)
    endif()

    if (IS_DIRECTORY ${PROJECT_SOURCE_DIR}/gis)
        rock_find_pkgconfig(envire_gis envire_gis)
        if(envire_gis_FOUND)
            message(STATUS "GIS found ... building ${PROJECT_NAME} gis plugin")
            rock_add_source_dir(gis envire_gis)
        else()
            message(STATUS "GIS not found ... NOT building ${PROJECT_NAME} gis plugin")
        endif()
    endif()

    if (IS_DIRECTORY ${PROJECT_SOURCE_DIR}/test)
        option(ROCK_TEST_ENABLED "set to OFF to disable the unit tests. Tests are automatically disabled if the boost unit test framework is not available" ON)
        if (ROCK_TEST_ENABLED)
            find_package(Boost COMPONENTS unit_test_framework system)
            if (Boost_UNIT_TEST_FRAMEWORK_FOUND)
                message(STATUS "boost/test found ... building the test suite")
                add_subdirectory(test)
            else()
                message(STATUS "boost/test not found ... NOT building the test suite")
            endif()
        else()
            message(STATUS "unit tests disabled as ROCK_TEST_ENABLED is set to OFF")
        endif()
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
        if (envire_gis_NOTFOUND)
            list(APPEND additional_deps DEPS_PKGCONFIG envire_gis)
        endif()
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


