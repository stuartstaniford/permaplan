// Copyright Staniford Systems.  All Rights Reserved.  February 2022 -
// This class provides methods and data required to model the state of the soil profile
// relevant to plant growth at the location of a given plant.  The data can be extracted
// from resources such as the Harmonized World Soil Database
// (https://www.fao.org/3/aq361e/aq361e.pdf) or national equivalents, and may be
// modified by local on-the-ground observations, interpolation operations between
// neighboring points, etc. 

// For US soil mapping info, see:
// https://websoilsurvey.sc.egov.usda.gov/App/HomePage.htm

// Helpful information on the structure of data in the World Soil Information Service 
// (WoSIS) database:
//https://data.isric.org/geonetwork/srv/eng/catalog.search#/metadata/ca880bd4-cff8-11e9-8046-0cc47adaa92c
// https://essd.copernicus.org/articles/12/299/2020/essd-12-299-2020.pdf

#include "SoilProfile.h"
#include <stdio.h>


// =======================================================================================
// Static variables, etc.

using namespace rapidjson;


// =======================================================================================
/// @brief Constructor

SoilHorizon::SoilHorizon(Value json)
{
}


// =======================================================================================
/// @brief Constructor

#define ERROR_MARGIN 32

int SoilHorizon::writeJson(char* buf, unsigned bufSize)
{
  char* writePoint = buf;
  if(bufSize < ERROR_MARGIN) goto ErrorReturn;
    
  writePoint += sprintf(writePoint, "{\n");
  if(bufSize - (writePoint-buf) < ERROR_MARGIN) goto ErrorReturn;
  
  writePoint += sprintf(writePoint, "depth: %.2f,\n", depth);
  if(bufSize - (writePoint-buf) < ERROR_MARGIN) goto ErrorReturn;

  writePoint += sprintf(writePoint, "coarseFragmentFraction: %.4f,\n", coarseFragmentFraction);
  if(bufSize - (writePoint-buf) < ERROR_MARGIN) goto ErrorReturn;

  writePoint += sprintf(writePoint, "sandFraction: %.4f,\n", sandFraction);
  if(bufSize - (writePoint-buf) < ERROR_MARGIN) goto ErrorReturn;

  writePoint += sprintf(writePoint, "siltFraction: %.4f,\n", siltFraction);
  if(bufSize - (writePoint-buf) < ERROR_MARGIN) goto ErrorReturn;

  writePoint += sprintf(writePoint, "clayFraction: %.4f,\n", clayFraction);
  if(bufSize - (writePoint-buf) < ERROR_MARGIN) goto ErrorReturn;

  writePoint += sprintf(writePoint, "usdaTextureClass: %s,\n", USDATextureName[usdaTextureClass]);
  if(bufSize - (writePoint-buf) < ERROR_MARGIN) goto ErrorReturn;

  writePoint += sprintf(writePoint, "bulkDensity: %.2f,\n", bulkDensity);
  if(bufSize - (writePoint-buf) < ERROR_MARGIN) goto ErrorReturn;

  writePoint += sprintf(writePoint, "organicCarbonFraction: %.4f,\n", organicCarbonFraction);
  if(bufSize - (writePoint-buf) < ERROR_MARGIN) goto ErrorReturn;

  writePoint += sprintf(writePoint, "pH: %.2f\n", pH);  // last one, no comma
  if(bufSize - (writePoint-buf) < ERROR_MARGIN) goto ErrorReturn;

  writePoint += sprintf(writePoint, "}\n\n");
  
  return writePoint - buf;
  
  ErrorReturn:
  // Log the error
  return -1;
}


// =======================================================================================
/// @brief Constructor

SoilProfile::SoilProfile(void)
{
}


// =======================================================================================
/// @brief Destructor

SoilProfile::~SoilProfile(void)
{
}


// =======================================================================================
