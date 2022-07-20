// Copyright Staniford Systems.  All Rights Reserved.  July 2022 -

#ifndef HWSD_PROFILE_H
#define HWSD_PROFILE_H

#include "SoilProfile.h"


// =======================================================================================
// Forward declarations

class MdbTableReader;


// =======================================================================================
/// @brief Subclass of SoilProfile for information and code specific to the Harmonized
/// World Soil Database (HWSD).
///
/// This class provides methods and data required to model the state of the soil profile
/// relevant to plant growth at the location of a given plant.  The data is extract from
/// the Harmonized World Soil Database (https://www.fao.org/3/aq361e/aq361e.pdf).
/// Generic data and methods should go in SoilProfile, and this class should only be used
/// for things that are HWSD specific.

class HWSDProfile: public SoilProfile
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  HWSDProfile(MdbTableReader& hwsdTableReader);
  ~HWSDProfile(void);
  
private:
  
  // Instance variables - private
  
  // Member functions - private
  /// @brief Prevent copy-construction.
  HWSDProfile(const HWSDProfile&);       
  /// @brief Prevent assignment.
  HWSDProfile& operator=(const HWSDProfile&);      
};


// =======================================================================================

#endif




