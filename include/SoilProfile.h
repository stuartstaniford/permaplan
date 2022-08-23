// Copyright Staniford Systems.  All Rights Reserved.  February 2022 -

#ifndef SOIL_PROFILE_H
#define SOIL_PROFILE_H

#include "GroundLayer.h"
#include "DynamicallyTypable.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/error/en.h"
#include <vector>


// =======================================================================================
/// @brief Model the nature of the soil at a given location.
///
/// This class provides methods and data required to model the state of the soil profile
/// relevant to plant growth at the location of a given plant.  The data can be extracted
/// from resources such as the Harmonized World Soil Database
/// (https://www.fao.org/3/aq361e/aq361e.pdf) or national equivalents, and may be
/// modified by local on-the-ground observations, interpolation operations between
/// neighboring points, etc. 
/// 
/// Note that SoilProfile is implemented as a vector of SoilHorizon instances.  If the
/// vector is empty (zero SoilHorizons) that indicates a no soil condition (eg bare rock,
/// deep water, etc).

class SoilProfile: public DynamicallyTypable, public std::vector<GroundLayer*>
{
public:
  
  // Instance variables - public
  ///@brief Latitude of location where this profile is believed valid
  float latitude;
  ///@brief Longtitude of location where this profile is believed valid
  float longtitude;
  
  // Member functions - public
  SoilProfile(void);
  virtual ~SoilProfile(void);
  virtual DynamicType getDynamicType(void) {return TypeSoilProfile;}
  virtual int writeJsonFields(char* buf, unsigned bufSize);
  void getGroundLayersFromJson(rapidjson::Document& soilJson);

private:
  
  // Instance variables - private
  
  // Member functions - private
  /// @brief Prevent copy-construction.
  SoilProfile(const SoilProfile&);       
  /// @brief Prevent assignment.
  SoilProfile& operator=(const SoilProfile&);      
};


// =======================================================================================

#endif
