// Copyright Staniford Systems.  All Rights Reserved.  Jun 2021 -
// This class provides an interface to databases of information required to do solar
// irradiance calculations.  Currently, it uses the GdalFileInterface and the gdal 
// library to read information from the raster files downloaded from 
//https://datacatalog.worldbank.org/dataset/world-direct-normal-irradiation-dni-gis-data-global-solar-atlas

#include "SolarDatabase.h"
#include "Global.h"
#include <err.h>


// =======================================================================================
// File level constants

char* difFileName = (char*) "Materials/Solar/World_DIF_GISdata_LTAy_AvgDailyTotals_GlobalSolarAtlas-v2_GEOTIFF/DIF.tif";

char* dniFileName = (char*)
    "Materials/Solar/World_DNI_GISdata_LTAy_AvgDailyTotals_GlobalSolarAtlas-v2_AAIGRID/DNI.asc";


// =======================================================================================
/// @brief Constructor

SolarDatabase::SolarDatabase(void):
                            difFile(difFileName),
                            dniFile(dniFileName)
{
}


// =======================================================================================
/// @brief Destructor

SolarDatabase::~SolarDatabase(void)
{
}


// =======================================================================================
/// @brief Get the diffuse horizontal irradiance value at a given location
/// 
/// @returns A float with the value for diffuse horizontal irradiance
/// @param lat Latitude of the location we are querying about.
/// @param longt Longtitude of the location we are querying about.
 
float SolarDatabase::getDIFValue(float lat, float longt)
{
  float retVal;
  difFile.getValueAtLocation(1, lat, longt, retVal);
  return retVal;
}


// =======================================================================================
/// @brief Get the direct normal irradiance value at a given location
/// 
/// @returns A float with the value for direct normal irradiance
/// @param lat Latitude of the location we are querying about.
/// @param longt Longtitude of the location we are querying about.
 
float SolarDatabase::getDNIValue(float lat, float longt)
{
  float retVal;
  dniFile.getValueAtLocation(1, lat, longt, retVal);
  return retVal;
}


// =======================================================================================
