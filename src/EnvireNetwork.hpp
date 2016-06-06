#pragma once

/** Envire Core Library **/
#include <envire_core/all>

/** GDAL library **/
#include <gdal/gnm.h>
#include <gdal/gnm_priv.h>

namespace envire{ namespace gis
{
    /** @brief Envire GNM Generic network of the envire graph
     * GNMGenericNetwork inherit from GDALDataset **/

    class EnvireNetwork: public GNMGenericNetwork
    {
    public:
        EnvireNetwork();

        ~EnvireNetwork();
    };

}} // end namespace envire::gis
