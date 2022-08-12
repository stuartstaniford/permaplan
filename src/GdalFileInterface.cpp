// Copyright Staniford Systems.  All Rights Reserved.  January 2022 -
// Interface to geospatial data files via the GDAL library.  Permaplan uses libgdal 
// to access files of geospatial data (eg geotiff files).  GDAL is very complex and 
// powerful, and this class provides a simplified interface which just abstracts 
// the limited amount of functionality we actually require

// For more on the gdal library see
// http://pkg.cs.ovgu.de/LNF/i386/5.10/LNFgdal-docs/reloc/gdal/html/gdal_tutorial.html
// http://pkg.cs.ovgu.de/LNF/i386/5.10/LNFgdal-docs/reloc/gdal/html/gdal_datamodel.html
// https://developers.planet.com/planetschool/getting-started-with-gdal/
// https://www.gis.usu.edu/~chrisg/python/2009/lectures/ospy_slides4.pdf
// https://gdal.org/api/gdalrasterband_cpp.html
// https://stackoverflow.com/questions/47789256/how-to-get-the-value-of-lat-long-from-a-hdf-file
// https://gdal.org/doxygen/classGDALDataset.html
// https://gdal.org/user/raster_data_model.html


// =======================================================================================
// We are no longer planning to use libgeotiff, but if that changes, here are useful 
// links: 
// For more on the geotiff format see
// http://docs.opengeospatial.org/is/19-008r4/19-008r4.html
// For more on libgeotiff see
// https://github.com/OSGeo/libgeotiff
// https://svn.osgeo.org/metacrs/geotiff/trunk/libgeotiff/docs/manual.txt
// Geotiff is a special case of TIFF, and this is the spec for that:
// https://www.adobe.io/content/dam/udp/en/open/standards/tiff/TIFF6.pdf


// =======================================================================================

#include "GdalFileInterface.h"
#include "Global.h"
#include "Logging.h"
#include <gdal_priv.h>
#include <err.h>


// =======================================================================================
// File scope (static) variables

bool gdalIsRegistered = false;


// =======================================================================================
/// @brief Constructor

GdalFileInterface::GdalFileInterface(char* fileName):
                                          srcFileName(fileName)
{
  lock();
  unless(gdalIsRegistered)
   {
    GDALAllRegister();
    gdalIsRegistered = true;
   }
  
  dataset = (GDALDataset*) GDALOpen(fileName, GA_ReadOnly);
  unless(dataset)
    err(-1, "Couldn't open %s.\n", fileName);
  CPLErr transErr = dataset->GetGeoTransform(geoTransform);
  unless(transErr == CE_None)
    err(-1, "GetGeoTransform failed in GdalFileInterface::printOverviewData.\n");

  unless(fabs(geoTransform[2]) < EPSILON &&  fabs(geoTransform[4]) < EPSILON)
    err(-1, "Reading %s failed, not north-up: (%.2f,%.2f) != (0.0, 0.0).\n", 
                                        fileName, geoTransform[2], geoTransform[4]);  

  unlock();
  LogPermaservOps("Opened Gdal file %s successfully.\n", fileName);

  //printOverviewData();
}


// =======================================================================================
/// @brief Destructor

GdalFileInterface::~GdalFileInterface(void)
{
  lock();
  if(dataset)
    GDALClose(dataset);
  unlock();
}


// =======================================================================================
/// @brief Get a value from a given raster band in the file.
/// 
/// @returns True if the file has a value for the given location in the given band, 
/// false in the event of an error (eg no such band, location not covered).
/// @param retVal A reference to a float to return the value found.
/// @param band An integer specifying the band to look in 
/// @param latitude The latitude of the location we are looking up.
/// @param longtitude The longtitude of the location we are looking up.

bool GdalFileInterface::getValueAtLocation(int band, float latitude, float longtitude, 
                                                                            float& retVal)
{  
  lock();
  // Get and validate the image size in pixels
  int rasterXSize = dataset->GetRasterXSize();
  int rasterYSize = dataset->GetRasterYSize();
  unless(rasterXSize > 0 && rasterYSize > 0) 
   {
    LogGdalError("Raster band %d in file %s has bad sizes %d,%d.\n", 
                                        band, srcFileName, rasterXSize, rasterYSize);
    unlock();
    return false;  
   }

  // Compute the pixel position and validate it.
  int xPixel = (latitude - geoTransform[0])/geoTransform[1];
  unless(xPixel >= 0 && xPixel < rasterXSize)
   {
    LogGdalError("xPixel %d out of range [0,%d] for band %d in file %s.\n", 
                                            xPixel, rasterXSize, band, srcFileName);
    unlock();
    return false;  
   }
  int yLine = (latitude - geoTransform[3])/geoTransform[5];
  unless(yLine >= 0 && yLine < rasterYSize)
   {
    LogGdalError("yLine %d out of range [0,%d] for band %d in file %s.\n", 
                                            yLine, rasterYSize, band, srcFileName);
    unlock();
    return false;  
   }
  
  // Get the correct raster band
  GDALRasterBand* rasterBand = dataset->GetRasterBand(band);
  unless(rasterBand)
   {
    LogGdalError("Cannot get raster band index %d from file %s.\n", band, srcFileName);
    unlock();
    return false;
   }

  // This function is expecting float data
  unless(rasterBand->GetRasterDataType() == GDT_Float32) 
   {
    LogGdalError("Raster band %d in file %s is not float32.\n", band, srcFileName);
    unlock();
    return false;  
   }
  
  // Get and validate the block size
  int nBlockXSize, nBlockYSize;
  rasterBand->GetBlockSize(&nBlockXSize, &nBlockYSize);
  unless(nBlockXSize > 0 && nBlockYSize > 0) 
   {
    LogGdalError("Raster band %d in file %s has bad block sizes %d,%d.\n", 
                                        band, srcFileName, nBlockXSize, nBlockYSize);
    unlock();
    return false;  
   }

  // Get a buffer to read one block into
  float* buf = new float[nBlockXSize * nBlockYSize];
  unless(buf) 
   {
    LogGdalError("Couldn't allocate buffer of %d floats in band %d in file %s.\n", 
                                              nBlockXSize*nBlockYSize, band, srcFileName);
    unlock();
    return false;  
   }
  
  // Read the block
  int xBlock = xPixel/nBlockXSize;
  int yBlock = yLine/nBlockYSize;
  unless(rasterBand->ReadBlock(xBlock, yBlock, buf) == CE_None)
   {
    LogGdalError("Failed to read block location [%d, %d] in band %d in file %s.\n", 
                                                      xBlock, yBlock, band, srcFileName);
    unlock();
    return false;  
   }
           
  // Extract the actual value we are looking for
  retVal = buf[(yLine - yBlock*nBlockYSize)*nBlockXSize + xPixel-xBlock*nBlockXSize];
  
  // Clean up before going home.
  delete[] buf;
           
  unlock();
  return true;
}


// =======================================================================================
/// @brief Print some summary data about the file.

void GdalFileInterface::printOverviewData()
{
  GDALRasterBand* difBand;
  int             nBlockXSize, nBlockYSize;
  int             bGotMin, bGotMax;
  double          adfMinMax[2];
  
  lock();
  for(int i=0; i<6; i++)
    printf("geoTransform[%d] is %.3f\n", i, geoTransform[i]);
  printf("%d pixels in each of %d lines with %d bands.\n", dataset->GetRasterXSize(),
         dataset->GetRasterYSize(), dataset->GetRasterCount());
  difBand = dataset->GetRasterBand(1);
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
  unlock();
 }


// =======================================================================================
