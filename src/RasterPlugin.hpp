#ifndef _RASTER_PLUGIN_HPP_
#define _RASTER_PLUGIN_HPP_

/** GDAL Core C/Public declaration **/
#include <gdal/gdal.h>

/** GDAL Core C++/Private declarations **/
#include <gdal/gdal_priv.h>

/** Std library **/
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
        typedef std::map< RasterPluginBand<T> *, int> BandNumber;

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
        virtual void fromRasterBand(const T &data) = 0;
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
    protected:
        /* top left x */
        /* w-e pixel resolution */
        /* 0 */
        /* top left y */
        /* 0 */
        /* n-s pixel resolution (negative value) */
        std::array<double, 6> geo_transform;

    public:
        RasterPlugin()
        :GDALDataset()
        {
            /** The RasterPlugin has at least one band **/
            dynamic_cast< RasterPluginBand< T >* >(this)->band_number[dynamic_cast< RasterPluginBand< T >* >(this)] = 0;
        }

        RasterPlugin(const std::array<double, 6> &_geo_transform)
        :GDALDataset()
        {
            this->geo_transform = _geo_transform;
        }

        virtual ~RasterPlugin(){}

        bool isRaster() { return true; }

        template<typename Type>
        void toGis(const Type &data)
        {
            /** Shorter raster plugin band type name **/
            typedef RasterPluginBand<Type> RasterType;

            /** Get the raster band **/
            RasterType *band = dynamic_cast<RasterType*>(this->GetRasterBand(this->band_number[dynamic_cast<RasterType*>(this)]));

            if(band)
            {
                band->toRasterBand(data);
            }
            else
            {
                throw std::runtime_error("Wrong type given to convert");
            }
        }

        template<typename Type>
        void fromGis(Type &data)
        {
            /** Shorter raster plugin band type name **/
            typedef RasterPluginBand<Type> RasterType;

            RasterType *band = dynamic_cast<RasterType*>(this->GetRasterBand(this->band_number[dynamic_cast<RasterType*>(this)]));

            if(band)
            {
                band->fromRasterBand(data);
            }
            else
            {
                throw std::runtime_error("Wrong type given to convert");
            }
        }

    private:

    };
}} // end namespace envire::gis

#endif // _RASTER_PLUGIN_HPP_
