// Copyright Staniford Systems.  All Rights Reserved.  February 2022 -
// A superclass for anythink which gets incorporated as a layer into a 
// SoilProfile.  The most common subclass is SoilHorizon, but we will eventually 
// need subclasses for organic litter layers, mulches, perhaps snow, etc.  In 
// general, things that lie on the ground in sheets and affect the behavior of the 
// ground, but don't have meaningful values for soil-related data items like 
// siltFraction or pH.

#include "GroundLayer.h"
#include "Global.h"


// =======================================================================================
// Static variables, etc

using namespace rapidjson;


// =======================================================================================
/// @brief Constructor

GroundLayer::GroundLayer(void)
{
}


// =======================================================================================
/// @brief Destructor

GroundLayer::~GroundLayer(void)
{
}


// =======================================================================================
/// @brief Static function to test that some json would make a valid SoilHorizon object

bool GroundLayer::isValid(Value& json)
{
  unless(json.IsObject())
   {
    return false;
   }

  unless(json.HasMember("depth") && json["depth"].IsFloat())
   {
    return false;
   }

  unless(json.HasMember("bulkDensity") && json["bulkDensity"].IsFloat())
   {
    return false;
   }

  unless(json.HasMember("organicCarbonFraction") 
                                      && json["organicCarbonFraction"].IsFloat())
   {
    return false;
   }
  
  return true;
}


// =======================================================================================
