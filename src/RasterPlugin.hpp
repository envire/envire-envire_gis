#ifndef _RASTER_PLUGIN_HPP_
#define _RASTER_PLUGIN_HPP_

#include <envire_gis/RasterPluginBand.hpp>

/** Std library **/
#include <iostream>    //
#include <map>    // std:map:map
#include <typeinfo>    // for getting the plugin class name 

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

        /** Band number coincident with GDAL nBand but indexed by plugin type name **/
        BandNumber band_number;

    public:
        /** @brief Default Constructor
         */
        RasterPlugin(RasterPluginBand<T> *parent = NULL): RasterPluginBand<T>(parent)
        {
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
        bool isRaster() { return true; }

        /** @brief getName
         *
         * @return the raster plugin name
         *
         */
        template<typename Type>
        const std::string static getName()
        {
            return typeid(RasterPluginBand< Type >).name();
        }

        template<typename Type>
        void toGis(const Type &data)
        {
            /** Shorter raster plugin band type name **/
            typedef RasterPluginBand<Type> BandType;

            /** Retrieve the raster band **/
            BandType *band = dynamic_cast<BandType*>(this);

            std::cout<<"[TO_GIS] Band name is: "<<band->getName()<<" with number "<<this->band_number[band->getName()]<<"\n";

            if(band)
            {
                /** Dedicated method to convert data to GIS GDAL**/
                band->toRasterBand(data);
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
                        /** If raster band is not NULL **/
                /** Get the raster band name **/
                //std::cout<<"FROM_GIS: dynamic_cast worked, band name: "<<band->getName<Type>()<<"!!\n";

               // std::cout<<"[FROM_GIS] SUCCESS Band name is: "<<band->getName<Type>()<<" with number "<<this->band_number[band->getName<Type>()]<<"\n";

                /** Dedicated method to convert from GIS GDAL to data **/
                //this->template convertToEnvireType<Type>(data);
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
}} // end namespace envire::gis

#endif // _RASTER_PLUGIN_HPP_
