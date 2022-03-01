// Copyright Staniford Systems.  All Rights Reserved.  February 2022 -

#ifndef SOIL_PROFILE_H
#define SOIL_PROFILE_H

#include "GroundLayer.h"
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

class SoilProfile: std::vector<GroundLayer*>
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  SoilProfile(void);
  ~SoilProfile(void);
  
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



