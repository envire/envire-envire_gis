#ifndef _RASTER_PLUGIN_BAND_HPP_
#define _RASTER_PLUGIN_BAND_HPP_

/** GDAL Core C/Public declaration **/
#include <gdal/gdal.h>

/** GDAL Core C++/Private declarations **/
#include <gdal/gdal_priv.h>

/** Eigen **/
#include <base/Eigen.hpp>

/** Std library **/
#include <iostream>    // std cout
#include <typeinfo>    // for getting the plugin class name 

namespace envire{ namespace gis
{

    /*****************************************
     * @brief RasterPluginBand
     *
     * it creates a different band per added type
     *****************************************/
    template <typename T>
    class RasterPluginBand: public GDALRasterBand
    {
        /** Friend class of the plugin **/
        template <typename Type> friend class RasterPlugin;

    public:
        /** @brief Constructor
         */
        RasterPluginBand(GDALRasterBand *parent = NULL): GDALRasterBand()
        {
            /** In case there is parent make a copy constructor **/
            if (parent)
            {
                /** Get Raster Sizes **/
                this->nRasterXSize = parent->GetXSize();
                this->nRasterYSize = parent->GetYSize();

                /** Get Band **/
                this->nBand = parent->GetBand();

                /** Associated with a dataset */
                this->poDS = parent->GetDataset();

                /** Raster Data type **/
                this->eDataType = parent->GetRasterDataType();

                /** Get Access type **/
                this->eAccess = parent->GetAccess();

                /** Blocking ans raster cache **/
                parent->GetBlockSize(&(this->nBlockXSize), &(this->nBlockYSize));
                this->nBlocksPerRow = DIV_ROUND_UP(this->nRasterXSize, this->nBlockXSize);
                this->nBlocksPerColumn = DIV_ROUND_UP(this->nRasterYSize, this->nBlockYSize);

                /** Raster Mask **/
                this->nMaskFlags = parent->GetMaskFlags();
                this->poMask = parent->GetMaskBand();
                if(this->poMask != NULL)
                {
                    this->bOwnMask = true;
                }
            }
        }

        /** @brief Destructor
         *
         * by default call parent de
         *
         */
        virtual ~RasterPluginBand() {}

        /** @brief Import
         *
         * import static cast helper function
         *
         */
        static RasterPluginBand<T> *Import(GDALRasterBand *raster_band)
        {
            /** Static cast the raster band to the RasterPluginBand **/
            RasterPluginBand<T> * raster_plugin = static_cast<RasterPluginBand<T> *>(raster_band);

            if (RasterPluginBand<T>::checkCellType<typename T::TemplateType::CellType>(raster_band))
            {
                std::cout<<"[IMPORT] GDALRasterBand with "<<raster_plugin->GetBand()<<" number of Raster Bands of type "<<raster_band->GetRasterDataType()<<"\n";
            }

            return raster_plugin;
        }

        friend std::ostream& operator<<(std::ostream& out, const RasterPluginBand<T>& val)
        {
            out <<"RasterPLuginBand <"<< typeid( T ).name()
                <<"> at band number "<< val.nBand<<" with size: "
                <<val.nRasterXSize<<" x "<<val.nRasterYSize<<" "
                <<"block Size "<<val.nBlockXSize<<" x "<<val.nBlockYSize;
            return out;
        }

    protected:

        template<typename Type>
        static bool checkCellType(GDALRasterBand  *band)
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
                std::stringstream strdefault;
                strdefault <<"GDAL type"<< typeid(Type).name() <<" is not supported.";
                throw std::runtime_error(strdefault.str());
            }
            std::stringstream strstr;
            strstr << "envire::gis::RasterPluginBand<T>: type missmatch: the array is of type "<< typeid(Type).name()
                   << " and length: " << sizeof(Type) << ". But the Band data are of type "
                   << type_name << " and length: " << size;
            throw std::runtime_error(strstr.str());
        }

    protected:

        /* @param data data to convert
         */
        virtual void convertToRasterBand(const T &data) = 0;

        /* @param data data to convert
         */
        virtual void convertToEnvireType(T &data, const base::Vector2d &cell_resolution = base::Vector2d::Zero()) = 0;

        /** GDALRasterBand left this method as pure virtual **/
        virtual CPLErr IReadBlock( int, int, void * ) {};

    };
}} // end namespace envire::gis

#endif // _RASTER_PLUGIN_BAND_HPP_
