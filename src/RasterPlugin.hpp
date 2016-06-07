#ifndef _RASTER_PLUGIN_HPP_
#define _RASTER_PLUGIN_HPP_

/** GDAL Core C/Public declaration **/
#include <gdal/gdal.h>

/** GDAL Core C++/Private declarations **/
#include <gdal/gdal_priv.h>

/** Std library **/
#include <iostream>    //
#include <typeinfo>    //
#include <algorithm>    // std::swap
#include <map>    // std:map:map

namespace envire{ namespace gis
{

    /** @brief RasterPluginBand
     *
     * it create a different band per added type
     */
    template <typename T>
    class RasterPluginBand
    {
        template <typename Type> friend class RasterPlugin;
        typedef std::map<const std::string, const int> BandNumber;

    public:
        virtual ~RasterPluginBand() {}

    protected:
        BandNumber band_number;

    protected:

        /* @param data data to convert
         */
        virtual void toRasterBand(const T &data) = 0;

        /* @param data data to convert
         */
        virtual void fromRasterBand(T &data) = 0;
    };

    /** @brief RasterPlugin
     *
     * Raster plugin for template type
     * By default it creates one band.
     */
    template <class T>
    class RasterPlugin: public GDALDataset,
                        public RasterPluginBand< T >
    {
    public:
        /** @brief Default Constructor
         */
        RasterPlugin()
            :GDALDataset()
        {
            /** The RasterPlugin must have at least one band **/
            this->registerBand< T >(dynamic_cast< RasterPluginBand< T > *>(this), 1);
        }

        static RasterPlugin<T> *Import(GDALDataset *dataset)
        {
            RasterPlugin<T> * raster_plugin = static_cast<RasterPlugin<T> *>(dataset);
            if(raster_plugin)
            {
                std::cout<<"Number of Raster Band: "<<raster_plugin->GetRasterCount()<<"\n";
                /** GDAL number of bands starts at 1 **/
                for (size_t i=1; i< raster_plugin->GetRasterCount()+1; ++i)
                {
                    std::cout<<"Checking Raster Band: "<<i<<"\n";
                    GDALRasterBand *band = dynamic_cast<GDALRasterBand*>(raster_plugin->GetRasterBand(i));
                    if (raster_plugin->checkBandType<typename T::TemplateType::CellType>(band))
                    {
                        std::cout<<"Register Band of type  "<<typeid(RasterPluginBand< T >).name() <<" in Band number "<<i<<"\n";
                        raster_plugin->registerBand< T >(dynamic_cast< RasterPluginBand< T > *>(raster_plugin), i);
                    }
                }
            }
            return raster_plugin;
        }

        /** @brief Destructor
         */
        virtual ~RasterPlugin(){}

        bool isRaster() { return true; }

        template<typename Type>
        void toGis(const Type &data)
        {
            /** Shorter raster plugin band type name **/
            typedef RasterPluginBand<Type> BandType;
            const std::string band_name = typeid(dynamic_cast< RasterPluginBand< Type > *>(this)).name();

            /** Retrieve the raster band **/
            BandType *band = dynamic_cast<BandType*>(this->GetRasterBand(this->band_number[band_name]));

            std::cout<<"[TO_GIS] Band name is: "<<typeid(dynamic_cast< RasterPluginBand< T > *>(this)).name()<<" with number "<<this->band_number[band_name]<<"\n";

            if(band)
            {
                band->toRasterBand(data);
            }
            else
            {
                throw std::runtime_error("Wrong Raster Band type given to convert");
            }
        }

        template<typename Type>
        void fromGis(Type &data)
        {
            /** Shorter raster plugin band type name **/
            typedef RasterPluginBand<Type> BandType;
            const std::string band_name = typeid(dynamic_cast< RasterPluginBand< T > *>(this)).name();

            /** Retrieve the raster band **/
            BandType *band = dynamic_cast<BandType*>(this->GetRasterBand(this->band_number[band_name]));

            std::cout<<"[FROM_GIS] Band name is: "<<typeid(dynamic_cast< RasterPluginBand< T > *>(this)).name()<<" with number "<<this->band_number[band_name]<<"\n";

            if(band)
            {
                band->fromRasterBand(data);
            }
            else
            {
                throw std::runtime_error("Wrong Raster Band type given to convert");
            }
        }

    private:

        template <typename Type>
            bool registerBand(RasterPluginBand<Type> *raster_band, const int band_number)
        {
            /** Sanity check**/
            if (band_number < 1)
            {
                throw std::runtime_error("Illegal Band number: GDAL Band number starts at number 1");
            }

            /** Sanity check**/
            if (raster_band == NULL)
            {
                throw std::runtime_error("Unexpected NULL pointer in argument");
            }

            /** Register the band name with its band number **/
            std::pair< typename RasterPluginBand<Type>::BandNumber::iterator, bool> result;
            const std::string band_name = typeid(*raster_band).name();
            std::cout<<"Name: "<<band_name<<" raster_band->band_number: "<< raster_band->band_number.size() <<"\n";
            result = raster_band->band_number.insert(std::pair<const std::string, const int>(band_name, band_number));
            return result.second;
        }

    protected:

        template<typename Type>
        bool checkBandType(GDALRasterBand  *band)
        {
            std::string type_name;
            int size = 0;
            switch(band->GetRasterDataType())
            {
              case GDT_Byte:
                if(typeid(Type)== typeid(unsigned char))
                  return true;
                type_name = typeid(unsigned char).name();
                size = sizeof(unsigned char);
                break;
              case GDT_Int16:
                if(typeid(Type)== typeid(int16_t))
                  return true;
                type_name = typeid(int16_t).name();
                size = sizeof(int16_t);
                break;
              case GDT_UInt16:
               if(typeid(Type)== typeid(uint16_t))
                  return true;
                type_name = typeid(uint16_t).name();
                size = sizeof(uint16_t);
                break;
              case GDT_Int32 :
               if(typeid(Type)== typeid(int32_t))
                  return true;
                type_name = typeid(int32_t).name();
                size = sizeof(int32_t);
                break;
              case GDT_UInt32 :
                if(typeid(Type)== typeid(uint32_t))
                  return true;
                type_name = typeid(uint32_t).name();
                size = sizeof(uint32_t);
                break;
              case GDT_Float32:
               if(typeid(Type)== typeid(float)&&sizeof(Type)==4)
                  return true;
                type_name = typeid(float).name();
                size = 4;
                break;
              case GDT_Float64:
                if(typeid(Type)== typeid(double)&&sizeof(Type)==8)
                  return true;
                type_name = typeid(double).name();
                size = 8;
                break;
              default:
                throw std::runtime_error("GDT type is not supported.");
            }
            std::stringstream strstr;
            strstr << "envire::gis::RasterPluginBand<T>: type missmatch: the array is of type "<< typeid(Type).name()
                   << " and length: " << sizeof(Type) << ". But the Band data are of type "
                   << type_name << " and length: " << size;
            throw std::runtime_error(strstr.str());
        }
    };
}} // end namespace envire::gis

#endif // _RASTER_PLUGIN_HPP_
