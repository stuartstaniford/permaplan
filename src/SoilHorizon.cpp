// Copyright Staniford Systems.  All Rights Reserved.  February 2022 -
// This class ..... is a template for real classes


#include "SoilHorizon.h"
#include "Logging.h"
#include "Global.h"
#include <stdio.h>
#include <unordered_map>
#include <string>


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

bool usdaReverseIndexPresent = false;

std::unordered_map<std::string, USDATextureClass> reverseUSDAIndex;

// =======================================================================================
/// @brief Blank constructor used, eg, for reading from the HWSD.

SoilHorizon::SoilHorizon(char* nm): GroundLayer(nm)
{
}


// =======================================================================================
/// @brief Utility function for reading floats from the soil json by 
/// SoilHorizon(Value& json).

inline float checkSetFloat(Value& soilJson, char* name)
{
  float retVal;
  if(soilJson.HasMember(name) && soilJson[name].IsFloat())
   {
    retVal = soilJson[name].GetFloat();
    LogHSWDExhaustive("Got %s value of %f in soil json.\n", name, retVal);
   }
  else
   {
    retVal = 0.0f;
    LogSoilDbErr("Couldn't get %s from soil json.\n", name);
   }
  return retVal;
}


// =======================================================================================
/// @brief Function to set up the reverse index for USDATextureNames.
/// 
/// Should only be called once, the first time we create a new SoilHorizon.

void setUpUsdaReverseIndex(void)
{
  usdaReverseIndexPresent = true;
  
  for(unsigned i = NoTextureClass; i <= Sand; i++)
    reverseUSDAIndex[USDATextureName[i]] = (USDATextureClass)i;
}


// =======================================================================================
/// @brief Constructor for reading from the JSON format
///
/// Note - call isValid first to ensure the JSON is valid

SoilHorizon::SoilHorizon(Value& json): GroundLayer(NULL)
{
  // ensure we have the reverse index the first time we are called
  unless(usdaReverseIndexPresent)
    setUpUsdaReverseIndex();
  
  // Initialize our parent class GroundLayer
  if(json.HasMember("name") && json["name"].IsString())
    initVars(json["name"].GetString(), 0.0f, 0.0f);
  else
    LogSoilDbErr("Couldn't get groundlayer name from soil json.\n");
    
  // Need to do USDA texture variables.
  if(json.HasMember("usdaTextureClass") && json["usdaTextureClass"].IsString()
                        && reverseUSDAIndex.count(json["usdaTextureClass"].GetString()) > 0)
   {
    usdaTextureClass = reverseUSDAIndex[json["usdaTextureClass"].GetString()];
    LogHSWDExhaustive("Got usdaTextureClass value of %s in soil json.\n", 
                                                  json["usdaTextureClass"].GetString());
   }
  else
    LogSoilDbErr("Couldn't get valid usdaTextureClass from soil json.\n");

  // Float variables directly in this class
  coarseFragmentFraction    = checkSetFloat(json, (char*)"coarseFragmentFraction");
  sandFraction              = checkSetFloat(json, (char*)"sandFraction");
  siltFraction              = checkSetFloat(json, (char*)"siltFraction");
  clayFraction              = checkSetFloat(json, (char*)"clayFraction");
  bulkDensity               = checkSetFloat(json, (char*)"bulkDensity");
  organicCarbonPercent      = checkSetFloat(json, (char*)"organicCarbonPercent");
  pH                        = checkSetFloat(json, (char*)"pH");
  cecClay                   = checkSetFloat(json, (char*)"cecClay");
  cecSoil                   = checkSetFloat(json, (char*)"cecSoil");
  baseSaturation            = checkSetFloat(json, (char*)"baseSaturation");
  totalExchangeableBases    = checkSetFloat(json, (char*)"totalExchangeableBases");
  limeContent               = checkSetFloat(json, (char*)"limeContent");
  gypsumContent             = checkSetFloat(json, (char*)"gypsumContent");
  exchangeableNaPercentage  = checkSetFloat(json, (char*)"exchangeableNaPercentage");
  electricalConductivity    = checkSetFloat(json, (char*)"electricalConductivity");
  bulkDensityNonRef         = checkSetFloat(json, (char*)"bulkDensityNonRef");
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
