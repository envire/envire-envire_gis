#include "RasterMap.hpp"

using namespace envire::gis;

RasterMap::RasterMap()
{

    GDALAllRegister();
}

RasterMap::~RasterMap() {this->close();}

void RasterMap::welcome()
{
    std::cout << "You successfully compiled and executed RasterMap. Welcome!" << std::endl;
}


void RasterMap::open(std::string const& path)
{
    this->dataset = (GDALDataset *) GDALOpen(path.c_str(), GA_ReadOnly );

    if( this->dataset == NULL )
        throw std::runtime_error("can not open file " + path);

    std::cout<< "Driver: "<< this->dataset->GetDriver()->GetDescription() <<" "<<
            this->dataset->GetDriver()->GetMetadataItem( GDAL_DMD_LONGNAME )<<"\n";

    std::cout<<"Size : "<< this->dataset->GetRasterXSize()<<" "<<this->dataset->GetRasterYSize() << " "<<
            this->dataset->GetRasterCount()<<"\n";

    if(this->dataset->GetProjectionRef()  != NULL)
    {
        std::cout<<"Projection is "<<this->dataset->GetProjectionRef()<<"\n";
    }

    if(this->dataset->GetGeoTransform( this->adfGeoTransform ) == CE_None)
    {
        std::cout<<"Origin = "<<this->adfGeoTransform[0]<<" "<<this->adfGeoTransform[3]<<"\n";
        std::cout<<"Pixel Size = "<<this->adfGeoTransform[1]<<" "<<this->adfGeoTransform[5]<<"\n";
    }
    return;
}

void RasterMap::close()
{

    GDALClose(this->dataset);
    return;
}


template <typename T>
typename envire::maps::GridMap<T>::Ptr RasterMap::toGridMap(const int band_number)
{

    /** Check the band number **/
    if(this->dataset->GetRasterCount() < band_number)
    {
        std::stringstream strstr;
        strstr << "Loaded "<<this->dataset->GetDriver()->GetDescription()<< " has " << this->dataset->GetRasterCount()
            << " raster bands but the band " << band_number << " was requested\n";
        throw std::runtime_error(strstr.str());
    }

    /** Get the band **/
    GDALRasterBand  *band;
    band = this->dataset->GetRasterBand(band_number);

    GDALDataType band_type = band->GetRasterDataType();

    /** Check that the band type and the grid type are compatible **/

    /** Create and configure the map **/
    envire::maps::Vector2ui cell_size(this->dataset->GetRasterXSize(), this->dataset->GetRasterYSize());
    Eigen::Vector2d cell_resolution (this->adfGeoTransform[1], this->adfGeoTransform[5]);
    typename envire::maps::GridMap<T>::Ptr map = envire::maps::GridMap<T>(cell_size, cell_resolution, 0.00);

    /** Fetching the Grid Map **/
    typename envire::maps::GridMap<T>::GridCell &data(map->getCells);
    T* data_ptr = &data[0][0];
    data.resize(boost::extents[cell_size[0]][cell_size[1]]);

    std::cout<<"Size of data: "<<sizeof(data)<<"\n";

    /** Read the Raster Band **/
    band->RasterIO(GF_Read, 0, 0,cell_size[0],cell_size[1],
            data_ptr, cell_size[0], cell_size[1],
            band_type, 0, sizeof(T) * cell_size[0] * cell_size[1]);

    return map;
}

//bool RasterMap::checkBandType(const envire::maps::GridMap<T> const GDALRasterBand  *band)
//{
//    std::string type_name;
//    int size = 0;
//    switch(band->GetRasterDataType())
//    {
//      case  GDT_Byte:
//        if(typeid(T)== typeid(unsigned char))
//          return true;
//        type_name = typeid(unsigned char).name();
//        size = sizeof(unsigned char);
//        break;
//      case GDT_Int16:
//        if(typeid(T)== typeid(int16_t))
//          return true;
//        type_name = typeid(int16_t).name();
//        size = sizeof(int16_t);
//        break;
//      case GDT_UInt16:
//       if(typeid(T)== typeid(uint16_t))
//          return true;
//        type_name = typeid(uint16_t).name();
//        size = sizeof(uint16_t);
//        break;
//      case GDT_Int32 :
//       if(typeid(T)== typeid(int32_t))
//          return true;
//        type_name = typeid(int32_t).name();
//        size = sizeof(int32_t);
//        break;
//      case GDT_UInt32 :
//        if(typeid(T)== typeid(uint32_t))
//          return true;
//        type_name = typeid(uint32_t).name();
//        size = sizeof(uint32_t);
//        break;
//      case GDT_Float32:
//       if(typeid(T)== typeid(float)&&sizeof(T)==4)
//          return true;
//        type_name = typeid(float).name();
//        size = 4;
//        break;
//      case GDT_Float64:
//        if(typeid(T)== typeid(double)&&sizeof(T)==8)
//          return true;
//        type_name = typeid(double).name();
//        size = 8;
//        break;
//      default:
//        throw std::runtime_error("GDT type is not supported.");
//    }
//    std::stringstream strstr;
//    strstr << "enview::Grid<T>: type missmatch: the array is of type "<< typeid(T).name()
//           << " and length: " << sizeof(T) << ". But the Band data are of type "
//           << type_name << " and length: " << size;
//    throw std::runtime_error(strstr.str());
//}
