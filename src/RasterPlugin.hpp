//
// Copyright (c) 2015, Deutsches Forschungszentrum für Künstliche Intelligenz GmbH.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.
//
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//

#ifndef _RASTER_PLUGIN_HPP_
#define _RASTER_PLUGIN_HPP_

#include <envire_gis/RasterPluginBand.hpp>

/** Std library **/
#include <map>    // std:map:map

namespace envire{ namespace gis
{
    /*****************************************
     * @brief RasterPlugin
     *
     * Raster plugin for template type
     *
     *****************************************/
    template <class T>
    class RasterPlugin: public RasterPluginBand< T >
    {
        /** Band Number type **/
        typedef std::map< const std::string, int> BandNumber;

        /** GDAL band numbers start counting at 1, so it is an invalid band number and the default one in most of the GDAL code **/
        static const int DEFAULT_BAND_NUMBER = 0;

    protected:

        /** resolution of the raster **/
        base::Vector2d resolution;

        /** Band number coincident with GDAL nBand but indexed by plugin type name **/
        BandNumber band_number;

    public:
        /** @brief Default Constructor
         */
        RasterPlugin(GDALRasterBand *parent = NULL, const base::Vector2d &resolution = base::Vector2d::Zero()): RasterPluginBand<T>(parent)
        {
            /** resolution of the raster **/
            this->resolution = resolution;

            /** Register the plugin type name and the band number **/
            this->band_number.insert(std::pair<std::string, int>(this->getName<T>(), this->nBand));

            std::cout<<"[CONSTRUCTOR] Name ["<<this->band_number.size()<< "]: "<<this->getName<T>()<<" is "<< this->band_number.find(this->getName<T>())->second<<"\n";
        }

        /** @brief Destructor
         *
         * by default call parent de
         *
         */
        ~RasterPlugin(){}

        static RasterPlugin *Import(GDALRasterBand *raster_band)
        {
            return dynamic_cast<RasterPlugin *>(RasterPluginBand<T>::Import(raster_band));
        }

        /**@brief is
         * return whether it is a Raster pl
         */
        bool isRaster() const { return true; }

        /** @brief getName
         *
         * @return the raster plugin name
         *
         */
        template<typename Type = T>
        const std::string static getName()
        {
            return typeid(RasterPluginBand< Type >).name();
        }

        template<typename Type = T>
        const int bandNumber() const
        {
            return this->band_number.find(this->getName<Type>())->second;
        }

        template<typename Type>
        void toGis(const Type &data)
        {
            /** Shorter raster plugin band type name **/
            typedef RasterPluginBand<Type> BandType;

            /** Retrieve the raster band **/
            BandType *band = dynamic_cast<BandType*>(this);

            std::cout<<"[TO_GIS] Band name is: "<<band->getName<Type>()<<" with number "<<this->band_number[band->getName<Type>()]<<"\n";

            if(band)
            {
                /** Dedicated method to convert data to GIS GDAL**/
                band->convertToRasterBand(data);
            }
            else
            {
                throw std::runtime_error("[TO_GIS] Wrong Raster Band type given to convert");
            }
        }

        /*@brief Convert from GDAL to Envire Type
         */
        template<typename Type>
        void fromGis(Type &data)
        {
            /** Shorter raster plugin band type name **/
            typedef RasterPluginBand<Type> BandType;

            std::cout<<"[FROM_GIS]: Requested band_type: "<<this->getName<Type>()<<" \n";

            /** Retrieve the raster band **/
            BandType *band = dynamic_cast<BandType*>(this);

            if (band)
            {
                /** Dedicated method to convert from GIS GDAL to data **/
                band->convertToEnvireType(data, this->resolution);
            }
            else
            {
                throw std::runtime_error("[FROM_GIS] Wrong Raster Band type given to convert");
            }
        }

    private:

        template<typename Type>
        void registerBand(RasterPluginBand<Type> *raster_band, const int bnumber)
        {
            /** Sanity check**/
            if (bnumber < 1)
            {
                throw std::runtime_error("Illegal Band number: GDAL Band number starts at number 1");
            }

            /** Sanity check**/
            if (raster_band == NULL)
            {
                throw std::runtime_error("Unexpected NULL pointer in argument");
            }

            /** Register the band name with its band number **/
            try
            {
                std::cout<<"Name: "<<raster_band->getName()<<" raster_band->band_number: "<< raster_band->band_number.size() <<"\n";
                raster_band->band_number.at(raster_band->getName()) = bnumber;
            }
            catch (const std::out_of_range& oor)
            {
                std::cerr << "[REGISTER_BAND] Out of Range error: " << oor.what() << '\n';
            }
            return;
        }
    };

    //Note how this is declared outside of the class body, so it is a free function instead of a memberfunction
    template<class Type> inline std::ostream& operator<<(std::ostream& out, const RasterPlugin<Type>& val)
    {
        out << "RasterPlugin <"<<typeid(Type).name()<<"> maps to Band: "<<val.bandNumber() <<"\n";
        out << static_cast< const RasterPluginBand< Type > &>(val);
        return out;
    }

}} // end namespace envire::gis

#endif // _RASTER_PLUGIN_HPP_
