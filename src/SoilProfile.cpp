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
#include "Logging.h"
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
/// @brief Estimate the total fertility of the soil profile

float SoilProfile::getFertility()
{
  float fertilityTotal = 0.0f;
  
  // Loop over the groundlayers
  float discount = 1.0f;
  for(GroundLayer* gLayer: *this)
   {
    float layerFert = 1.0f;
    if(gLayer->getDynamicType() != TypeSoilHorizon)
      continue; // assume no fertility from anything that isn't soil
    SoilHorizon* horizon = (SoilHorizon*)gLayer;
    layerFert *= horizon->clayFraction;
    layerFert *= discount;
    fertilityTotal += layerFert;
    discount *= 0.5f; // we discount lower soil layers
   }
  return fertilityTotal;
}


// =======================================================================================
/// @brief Output JSON soil profile format to a buffer.
/// 
/// @returns The number of bytes written to the buffer.  If greater than or equal to 
/// the supplied bufSize parameter, it indicates the buffer was not big enough and the
/// output will have been truncated/incomplete.
/// @param buf The char buffer to write the JSON to.
/// @param bufSize The size of the buffer, which must not be overwritten after the end.

int SoilProfile::writeJsonFields(char* buf, unsigned bufSize)
{
  char* end = buf + bufSize;
  
  buf += DynamicallyTypable::writeJsonFields(buf, bufSize);
  bufprintf(",\n");

  // Write our own fields.
  bufprintf("\"latitude\": %.6f,\n", latitude);
  bufprintf("\"longtitude\": %.6f,\n", longtitude);
  
  // Write out the array of layers
  bufprintf("\"groundLayers\": [\n");

  int layersWritten = 0;
  int subLen;
  for(GroundLayer* horizon: *this)
   {
    if( (subLen = horizon->writeJson(buf, end-buf)) < 0)
      return -1;
    else
      buf += subLen;
    bufprintf(",\n");
    layersWritten++;
   }
  if(layersWritten)
   {
    // Remove the last comma in the array
    buf -= 2;
    bufprintf("\n");
   }
   
  bufprintf("]"); // no ,\n as we don't know we are last
  return bufSize - (end-buf);
}


// =======================================================================================
/// @brief Function used in permaplan when we've gotten soil information as json from
/// permaserv to load our ground layers.
/// 
/// Note the convention here is that groundLayers are indexed from the top down.
/// 
/// @param soilJson A rapidjson::Document reference with the parsed json received from
/// permaserv.  By the time we get here, we are guaranteed that it's valid JSON, but 
/// otherwise we need to validate.
/// 
/// See the technical report at
/// https://www.fao.org/3/aq361e/aq361e.pdf for the semantics of the different fields.

using namespace rapidjson;

void SoilProfile::getGroundLayersFromJson(Document& soilJson)
{
  // Check we have a valid array at all
  unless(soilJson.HasMember("groundLayers") && soilJson["groundLayers"].IsArray())
   {
    LogSoilDbErr("Couldn't get valid groundLayers from soil json.\n");
    return;
   }
  
  // Loop over the layers
  int N = soilJson["groundLayers"].Size();
  for(int i=0; i<N; i++)
   {
    // Check it looks valid
    Value& layerJson = soilJson["groundLayers"][i];
    unless(layerJson.HasMember("dynamicType") && layerJson["dynamicType"].IsString())
     {
      LogSoilDbErr("Couldn't get valid layer json %d from soil json.\n", i);
      continue;
     }

    // Handle the different possible types of groundLayer we might instantiate
    if(strcmp(layerJson["dynamicType"].GetString(), "TypeSoilHorizon") == 0)
     {
      GroundLayer* gLayer = new SoilHorizon(layerJson);
      push_back(gLayer);
      LogHSWDExhaustive("Stored layer %d of type %s from soil json.\n",
                                              i, layerJson["dynamicType"].GetString());
     }
    else
     {
      LogSoilDbErr("Unsupported ground layer type %s in soil json position %d.\n",
                                                  layerJson["dynamicType"].GetString(), i);
     }
   }
}


// =======================================================================================
/// @brief Add carbon to the soil surface (eg leaf fall).
/// 
/// @param carbon the amount of carbon to add (in XXX units).

void SoilProfile::addCarbonFromAbove(float carbon)
{
  // If there isn't a litter layer at the front of the layer stack, add one
  
  // Add carbon to the litter layer
}


// =======================================================================================
/// @brief Evolve the carbon in the soil profile based on the passing of time.  
/// 
/// This is the main function that models bioturbation, respiratory decay, etc
/// @todo need to figure out how climate information is passed into this.

void SoilProfile::timeStepCarbonEvolution(void)
{
  
}


// =======================================================================================
