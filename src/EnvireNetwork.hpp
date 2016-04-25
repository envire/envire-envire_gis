#pragma once

/** Envire Core Library **/
#include <envire_core/all>

/** GDAL library **/
#include <gdal/gnm.h>
#include <gdal/gnm_priv.h>

/** Std library **/
#include <sstream>
#include <iostream>
#include <stdexcept>

namespace envire{ namespace gis
{
    class EnvireNetwork: public GNMGenericNetwork
    {
    public:
        EnvireNetwork();

        ~EnvireNetwork();
    };

}} // end namespace envire::gis
