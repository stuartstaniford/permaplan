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
#include "SoilHorizon.h"
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
/// @brief Constructor used when reading from the Harmonized World Soil Database.
/// 
/// @param hwsdTableReader An MdbTableReader which should already have been initialized
/// and into which the next row should have been read.  We examine the buffers and
/// extract the information into our structures.

SoilProfile::SoilProfile(MdbTableReader& hwsdTableReader)
{ 
  // Global information about the profile

  //[ID]      Long Integer,   
  //[MU_GLOBAL]      Long Integer, 
  //[MU_SOURCE1]      Text (12), 
  //[MU_SOURCE2]      Long Integer, 
  //[ISSOIL]      Byte, 
  //[SHARE]      Single, 
  //[SEQ]      Byte, 
  //[SU_SYM74]      Text (6), 
  //[SU_CODE74]      Integer, 
  //[SU_SYM85]      Text (6), 
  //[SU_CODE85]      Integer, 
  //[SU_SYM90]      Text (6), 
  //[SU_CODE90]      Integer, 
  //[T_TEXTURE]      Byte, 
  //[DRAINAGE]      Integer, 
  //[REF_DEPTH]      Integer, 
  //[AWC_CLASS]      Byte, 
  //[PHASE1]      Byte, 
  //[PHASE2]      Byte, 
  //[ROOTS]      Byte, 
  //[IL]      Byte, 
  //[SWR]      Byte, 
  //[ADD_PROP]      Byte, 
  
  // Deal with the topsoil qualities
  SoilHorizon* topSoil = new SoilHorizon((char*)"topSoil");
  push_back(topSoil);

  //[T_GRAVEL]      Integer, 
  //[T_SAND]      Integer, 
  //[T_SILT]      Integer, 
  //[T_CLAY]      Integer, 
  //[T_USDA_TEX_CLASS]      Byte, 
  //[T_REF_BULK_DENSITY]      Double, 
  //[T_OC]      Double, 
  //[T_PH_H2O]      Double, 
  //[T_CEC_CLAY]      Double, 
  //[T_CEC_SOIL]      Double, 
  //[T_BS]      Double, 
  //[T_TEB]      Double, 
  //[T_CACO3]      Double, 
  //[T_CASO4]      Double, 
  //[T_ESP]      Double, 
  //[T_ECE]      Double, 

  // Deal with the subsoil qualities
  SoilHorizon* subSoil = new SoilHorizon((char*)"subSoil");
  push_back(subSoil);

  //[S_GRAVEL]      Integer, 
  //[S_SAND]      Integer, 
  //[S_SILT]      Integer, 
  //[S_CLAY]      Integer, 
  //[S_USDA_TEX_CLASS]      Byte, 
  //[S_REF_BULK_DENSITY]      Double, 
  //[S_OC]      Double, 
  //[S_PH_H2O]      Double, 
  //[S_CEC_CLAY]      Double, 
  //[S_CEC_SOIL]      Double, 
  //[S_BS]      Double, 
  //[S_TEB]      Double, 
  //[S_CACO3]      Double, 
  //[S_CASO4]      Double, 
  //[S_ESP]      Double, 
  //[S_ECE]      Double, 
  //[T_BULK_DENSITY]      Double, 
  //[S_BULK_DENSITY]      Double

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
