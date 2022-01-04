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

char* solarFilename = (char*) "Materials/Solar/World_DIF_GISdata_LTAy_AvgDailyTotals_GlobalSolarAtlas-v2_GEOTIFF/DIF.tif";


// =======================================================================================
/// @brief Constructor

SolarDatabase::SolarDatabase(void):
                            difFile(solarFilename)
{
}


// =======================================================================================
/// @brief Destructor

SolarDatabase::~SolarDatabase(void)
{
}


// =======================================================================================
