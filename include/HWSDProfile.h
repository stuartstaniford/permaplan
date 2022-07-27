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
  void columnCheck(int column, char* colName, int expectedType);

private:
  
  // Instance variables - private.
  // Note some reordering has been done to improve packing
  MdbTableReader& hwsdReader;
  int             dbId;
  int             muGlobal;
  char            muSource1[12];
  int             muSource2;
  float           share;
  char            suSym74[6];
  bool            isSoil;
  unsigned char   seq;
  int             suCode74;
  char            suSym85[6];
  unsigned char   tTexture;
  unsigned char   awcClass;
  int             suCode85;
  char            suSym90[6];
  unsigned char   phase1;
  unsigned char   phase2;
  int             suCode90;
  int             drainage;
  int             refDepth;
  unsigned char   roots;
  unsigned char   il;
  unsigned char   swr;
  unsigned char   addProp;
  
  ///@todo SU_ variables not clear if in here or soilProfile
  
  
  // Member functions - private
  /// @brief Prevent copy-construction.
  HWSDProfile(const HWSDProfile&);       
  /// @brief Prevent assignment.
  HWSDProfile& operator=(const HWSDProfile&);      
};


// =======================================================================================

#endif