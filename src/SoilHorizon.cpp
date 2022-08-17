// Copyright Staniford Systems.  All Rights Reserved.  February 2022 -
// This class ..... is a template for real classes


#include "SoilHorizon.h"
#include <stdio.h>
#include "Global.h"


// =======================================================================================
// Static variables, etc.

using namespace rapidjson;

char* USDATextureName[] = {
                            (char*)"NoTextureClass", 
                            (char*)"HeavyClay",
                            (char*)"SiltyClay",
                            (char*)"Clay",
                            (char*)"SiltyClayLoam",
                            (char*)"ClayLoam",
                            (char*)"Silt",
                            (char*)"SiltyLoam",
                            (char*)"SandyClay",
                            (char*)"Loam",
                            (char*)"SandyClayLoam",
                            (char*)"SandyLoam",
                            (char*)"LoamySand",
                            (char*)"Sand"
};


// =======================================================================================
/// @brief Blank constructor used, eg, for reading from the HWSD.

SoilHorizon::SoilHorizon(char* nm): GroundLayer(nm)
{
}


// =======================================================================================
/// @brief Constructor for reading from the JSON format
///
/// Note - call isValid first to ensure the JSON is valid

SoilHorizon::SoilHorizon(Value json): GroundLayer(NULL)
{
}


// =======================================================================================
/// @brief Destructor

SoilHorizon::~SoilHorizon(void)
{
}


// =======================================================================================
/// @brief Output JSON soil horizon format to a buffer.
/// 
/// @returns The number of bytes written to the buffer.  If greater than or equal to 
/// the supplied bufSize parameter, it indicates the buffer was not big enough and the
/// output will have been truncated/incomplete.
/// @param buf The char buffer to write the JSON to.
/// @param bufSize The size of the buffer, which must not be overwritten after the end.

int SoilHorizon::writeJsonFields(char* buf, unsigned bufSize)
{
  char* end = buf + bufSize;
  
  // Write parent and ancestor fields
  buf += GroundLayer::writeJsonFields(buf, bufSize);
  bufprintf(",\n");
  
  // Write our own fields.  
  bufprintf("\"coarseFragmentFraction\": %.4f,\n", coarseFragmentFraction);
  bufprintf("\"sandFraction\": %.4f,\n", sandFraction);
  bufprintf("\"siltFraction\": %.4f,\n", siltFraction);
  bufprintf("\"clayFraction\": %.4f,\n", clayFraction);
  bufprintf("\"usdaTextureClass\": \"%s\",\n", USDATextureName[usdaTextureClass]);
  bufprintf("\"bulkDensity\": %.2f,\n", bulkDensity);  
  bufprintf("\"organicCarbonPercent\": %.1f,\n", organicCarbonPercent);  
  bufprintf("\"pH\": %.2f,\n", pH);  
  bufprintf("\"cecClay\": %.1f,\n", cecClay);
  bufprintf("\"cecSoil\": %.1f,\n", cecSoil);
  bufprintf("\"baseSaturation\": %.1f,\n", baseSaturation);
  bufprintf("\"totalExchangeableBases\": %.1f,\n", totalExchangeableBases);
  bufprintf("\"limeContent\": %.1f,\n", limeContent);
  bufprintf("\"gypsumContent\": %.1f,\n", gypsumContent);
  bufprintf("\"exchangeableNaPercentage\": %.1f,\n", exchangeableNaPercentage);
  bufprintf("\"electricalConductivity\": %.1f,\n", electricalConductivity);
  
  // Last one, no comma
  bufprintf("\"bulkDensityNonRef\": %.1f", bulkDensityNonRef);
  
  return bufSize - (end-buf);
}


// =======================================================================================
/// @brief Static function to test that some json would make a valid SoilHorizon object

bool SoilHorizon::isValid(Value& json)
{
  unless(json.IsObject())
   {
    return false;
   }

  unless(json.HasMember("GroundLayer") 
              && GroundLayer::isValid(json["GroundLayer"]))
   {
    return false;
   }

  unless(json.HasMember("coarseFragmentFraction") 
                                          && json["coarseFragmentFraction"].IsFloat())
   {
    return false;
   }

  unless(json.HasMember("sandFraction") && json["sandFraction"].IsFloat())
   {
    return false;
   }

  unless(json.HasMember("siltFraction") && json["siltFraction"].IsFloat())
   {
    return false;
   }

  unless(json.HasMember("clayFraction") && json["clayFraction"].IsFloat())
   {
    return false;
   }

/*  if(json.HasMember("usdaTextureClass"))
                                      && json["usdaTextureClass"].IsString())
   {
    return false;
   }*/

  unless(json.HasMember("pH") && json["pH"].IsFloat())
   {
    return false;
   }
  
  return true;
}


// =======================================================================================
