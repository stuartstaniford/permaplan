// Copyright Staniford Systems.  All Rights Reserved.  Jun 2021 -
// This class provides an interface to databases of information required to do solar
// irradiance calculations.  Currently, it uses libgeotiff to read information from
// the raster files downloaded from 
//https://datacatalog.worldbank.org/dataset/world-direct-normal-irradiation-dni-gis-data-global-solar-atlas

// For more on the geotiff format see
// http://docs.opengeospatial.org/is/19-008r4/19-008r4.html

// For more on libgeotiff see
// https://github.com/OSGeo/libgeotiff
// https://svn.osgeo.org/metacrs/geotiff/trunk/libgeotiff/docs/manual.txt

#include "SolarDatabase.h"
#include "geotiffio.h"
#include "xtiffio.h"


// =======================================================================================
/// @brief Constructor

SolarDatabase::SolarDatabase(void)
{
}


// =======================================================================================
/// @brief Destructor

SolarDatabase::~SolarDatabase(void)
{
}


// =======================================================================================
