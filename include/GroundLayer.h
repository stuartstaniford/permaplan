// Copyright Staniford Systems.  All Rights Reserved.  February 2022 -

#ifndef GROUND_LAYER_H
#define GROUND_LAYER_H

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/error/en.h"


// =======================================================================================
/// @brief A superclass for anything which gets incorporated as a layer into a 
/// SoilProfile.  
/// 
/// The most common subclass is SoilHorizon, but we will eventually need subclasses for
/// organic litter layers, mulches, perhaps snow, etc.  In general, things that lie on
/// the ground in sheets and affect the behavior of the ground, but don't have 
/// meaningful values for soil-related data items like siltFraction or pH.

class GroundLayer
{
public:
  
  // Instance variables - public
  
  /// @brief The depth of this particular soil layer (cm).
  float depth;                  
  
  /// @brief The reference bulk density of the layer (kg/l) 
  float bulkDensity;  
  
  /// @brief The weight fraction of carbon in organic molecules in the fine earth
  /// component of the soil.  For useful background see:
  /// https://www.soilquality.org.au/factsheets/organic-carbon
  float organicCarbonFraction;
  
  
  // Member functions - public
  GroundLayer(void);
  ~GroundLayer(void);
  static bool isValid(rapidjson::Value& json);
  
private:
  
  // Instance variables - private
  
  // Member functions - private
  /// @brief Prevent copy-construction.
  GroundLayer(const GroundLayer&);       
  /// @brief Prevent assignment.
  GroundLayer& operator=(const GroundLayer&);      
};


// =======================================================================================

#endif




