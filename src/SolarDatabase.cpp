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
#include "Global.h"
#include <gdal_priv.h>
#include <err.h>

// =======================================================================================
// File level constants

char* solarFilename = (char*) "Materials/Solar/World_DIF_GISdata_LTAy_AvgDailyTotals_GlobalSolarAtlas-v2_GEOTIFF/DIF.tif";


// =======================================================================================
/// @brief Constructor

SolarDatabase::SolarDatabase(void)
{
  GDALAllRegister();

  GDALDataset* difDataset = (GDALDataset*) GDALOpen(solarFilename , GA_ReadOnly);
  unless(difDataset)
    err(-1, "Couldn't open %s.\n", solarFilename);
  
  getDifData(difDataset);
}


// =======================================================================================
/// @brief Destructor

SolarDatabase::~SolarDatabase(void)
{
}


// =======================================================================================
/// @brief Obtain the diffuse horizontal data.

void SolarDatabase::getDifData(GDALDataset* difDataset)
{
  GDALRasterBand  *difBand;
  int             nBlockXSize, nBlockYSize;
  int             bGotMin, bGotMax;
  double          adfMinMax[2];
         
  difBand = difDataset->GetRasterBand(1);
  difBand->GetBlockSize(&nBlockXSize, &nBlockYSize);
  printf( "Block=%dx%d Type=%s, ColorInterp=%s\n",
            nBlockXSize, nBlockYSize,
            GDALGetDataTypeName(difBand->GetRasterDataType()),
            GDALGetColorInterpretationName(difBand->GetColorInterpretation()) );

  adfMinMax[0] = difBand->GetMinimum(&bGotMin);
  adfMinMax[1] = difBand->GetMaximum(&bGotMax);
  unless(bGotMin && bGotMax)
    GDALComputeRasterMinMax((GDALRasterBandH)difBand, TRUE, adfMinMax);
  printf("Min=%.3fd, Max=%.3f\n", adfMinMax[0], adfMinMax[1]);
  if(difBand->GetOverviewCount() > 0 )
    printf("Band has %d overviews.\n", difBand->GetOverviewCount());
  if(difBand->GetUnitType())
    printf("Band has %s units.\n", difBand->GetUnitType());
 }


// =======================================================================================
