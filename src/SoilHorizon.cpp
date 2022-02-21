// Copyright Staniford Systems.  All Rights Reserved.  February 2022 -
// This class ..... is a template for real classes

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

SoilHorizon::SoilHorizon(Value json)
{
}


// =======================================================================================
/// @brief Destructor

SoilHorizon::~SoilHorizon(void)
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
/// @brief Static function to test that some json would make a valid SoilHorizon object

bool SoilHorizon::isValid(rapidjson::Value json)
{
  unless(json.IsObject())
   {
    return false;
   }

  unless(json.HasMember("depth") && json["depth"].IsFloat())
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

  unless(json.HasMember("bulkDensity") && json["bulkDensity"].IsFloat())
   {
    return false;
   }

  unless(json.HasMember("organicCarbonFraction") 
                                      && json["organicCarbonFraction"].IsFloat())
   {
    return false;
   }

  unless(json.HasMember("pH") && json["pH"].IsFloat())
   {
    return false;
   }
  
  return true;
}


// =======================================================================================
