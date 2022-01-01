// Copyright Staniford Systems.  All Rights Reserved.  Jun 2021 -
// This class provides an interface to databases of information required to do solar
// irradiance calculations.  Currently, it uses the gdal library to read information 
// from the raster files downloaded from 
//https://datacatalog.worldbank.org/dataset/world-direct-normal-irradiation-dni-gis-data-global-solar-atlas

// For more on the geotiff format see
// http://docs.opengeospatial.org/is/19-008r4/19-008r4.html

// For more on libgeotiff see
// https://github.com/OSGeo/libgeotiff
// https://svn.osgeo.org/metacrs/geotiff/trunk/libgeotiff/docs/manual.txt

// Geotiff is a special case of TIFF, and this is the spec for that:
// https://www.adobe.io/content/dam/udp/en/open/standards/tiff/TIFF6.pdf

// For more on the gdal library see
// http://pkg.cs.ovgu.de/LNF/i386/5.10/LNFgdal-docs/reloc/gdal/html/gdal_tutorial.html
// http://pkg.cs.ovgu.de/LNF/i386/5.10/LNFgdal-docs/reloc/gdal/html/gdal_datamodel.html
// https://developers.planet.com/planetschool/getting-started-with-gdal/

#include "SolarDatabase.h"
#include "gdal_priv.h"
#include <err.h>

char* solarFilename = (char*) "Materials/Solar/World_DIF_GISdata_LTAy_AvgDailyTotals_GlobalSolarAtlas-v2_GEOTIFF/DIF.tif";

// =======================================================================================
/// @brief Constructor

SolarDatabase::SolarDatabase(void)
{
  GDALAllRegister();

  GDALDataset* poDataset = (GDALDataset*) GDALOpen(solarFilename , GA_ReadOnly);
  if(poDataset == NULL)
   {
    err(-1, "Couldn't open %s.\n", solarFilename);
   }
}


// =======================================================================================
/// @brief Destructor

SolarDatabase::~SolarDatabase(void)
{
}


// =======================================================================================
