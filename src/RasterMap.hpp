#ifndef _RASTER_MAP_HPP_
#define _RASTER_MAP_HPP_

/** Envire Maps Library **/
#include <envire_core/items/Transform.hpp>
#include <envire_maps/GridMap.hpp>

/** GDAL library **/
#include <gdal/gdal_priv.h>

/** Std library **/
#include <sstream>
#include <iostream>
#include <stdexcept>

namespace envire{ namespace gis

{
    class RasterMap
    {
        public:
            GDALDataset  *dataset;
            double adfGeoTransform[6];

        private:

            /** @brief Helper method for deserialization
             *
             * Reads the data contained in the provided band of the GDAL-readable
             * file at \c path into the \c key band of this map
             */
            //void readGridData(const std::string &band,const std::string& path,int base_band = 1, boost::enable_if< boost::is_fundamental<T> >* enabler = 0);

            /** @brief Check teh band type
             */
            //bool checkBandType(const GDALRasterBand  *band);

        public:

            /** @brief default constructor */
            RasterMap();

            /** @brief default destructor */
            ~RasterMap();

            /**@brief open a raster
             * */
            void open(std::string const& path);

            /** @brief close a raster
             * */
            void close();

            void toGridMap(const int band_number);

            /**
             * Print a welcome to stdout
             * \return nothing
             */
            void welcome();

        private:

    };

}} // end namespace envire::gis

#endif // _RASTER_MAP_HPP_
