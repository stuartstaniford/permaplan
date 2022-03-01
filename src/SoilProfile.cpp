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
#include "Global.h"

// =======================================================================================
// Static variables, etc.

using namespace rapidjson;


// =======================================================================================
/// @brief Constructor
/// 
/// Note - call isValid first to ensure the JSON is valid

SoilProfile::SoilProfile(void)
{
}


// =======================================================================================
/// @brief Destructor

SoilProfile::~SoilProfile(void)
{
}


// =======================================================================================
/// @brief Output JSON soil profile format to a buffer.
/// 
/// @returns The number of bytes written to the buffer.  If greater than or equal to 
/// the supplied bufSize parameter, it indicates the buffer was not big enough and the
/// output will have been truncated/incomplete.
/// @param buf The char buffer to write the JSON to.
/// @param bufSize The size of the buffer, which must not be overwritten after the end.

#define bufPrintf(...) if((writePt += snprintf(writePt, \
              writeEnd-writePt,  __VA_ARGS__)) >= writeEnd) \
                 return writePt-buf

unsigned SoilProfile::writeJson(char* buf, unsigned bufSize)
{
  char* writePt = buf;
  char* writeEnd = buf + bufSize;
  
  bufPrintf("[\n");

  int layersWritten = 0;
  for(GroundLayer* horizon: *this)
   {
    if( (writePt += horizon->writeJson(writePt, writeEnd-writePt)) >= writeEnd)
      return writePt-buf;
    bufPrintf(",\n");
    layersWritten++;
   }
  if(layersWritten)
   {
    // Remove the last comma in the array
    writePt -= 2;
    bufPrintf("\n");
   }

  bufPrintf("]\n");
  
  return 0u;
}


// =======================================================================================
