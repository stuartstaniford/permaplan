// Copyright Staniford Systems.  All Rights Reserved.  February 2022 -

#ifndef SOIL_HORIZON_H
#define SOIL_HORIZON_H

#include "GroundLayer.h"


// =======================================================================================
/// @brief An enum for USDA texture classes.  See
/// 
/// For details of the definition, see p13 of https://www.fao.org/3/aq361e/aq361e.pdf

enum USDATextureClass
{
  NoTextureClass,
  HeavyClay,
  SiltyClay,
  Clay,
  SiltyClayLoam,
  ClayLoam,
  Silt,
  SiltyLoam,
  SandyClay,
  Loam,
  SandyClayLoam,
  SandyLoam,
  LoamySand,
  Sand
};


// =======================================================================================
/// @brief Helper class for a single soil horizon in a soil profile
///
/// This class represents one unit of soil in a soil profile (eg taken from some database
/// or soil survey map).  For example the topsoil (or "A horizon") might be an example.

class SoilHorizon: public GroundLayer
{
  public:
  
  // The data fields.

  /// @brief Descriptive name of this horizon.
  char* name; 
  
  /// @brief The volumetric fraction of gravel/channers/etc (0.0 - 1.0).  Coarse 
  /// Fragments are considered to be those over 2mm in size.
  float coarseFragmentFraction; 
  
  /// @brief The weight fraction of sand in the soil horizon (0.0 - 1.0).  Sand 
  /// comprises particles, or granules, ranging in diameter from 0.0625 mm (or 1⁄16 mm) 
  /// to 2 millimeters.  (Assumed to be the fraction of the fine earth but this is TBD.)
  float sandFraction;

  /// @brief The weight fraction of silt in the soil horizon (0.0 - 1.0).  Silt 
  /// comprises particles, or granules, ranging in diameter from 0.002mm to 0.0625 mm 
  /// (or 1⁄16 mm).  (Assumed to be the fraction of the fine earth but this is TBD.)
  float siltFraction;

  /// @brief The weight fraction of clay in the soil horizon (0.0 - 1.0).  Clay comprises 
  /// particles, or granules, with diameter below 0.002mm to 0.0625 mm.  (Assumed to 
  /// be the fraction of the fine earth but this is TBD.)
  float clayFraction;
  
  /// @brief The USDA texture class.  Enum defined above, for full detail,  see p13 of 
  /// https://www.fao.org/3/aq361e/aq361e.pdf
  USDATextureClass usdaTextureClass;

  /// @brief The bulk density of the soil horizon
  float bulkDensity;

  /// @brief The percentage of organic carbon in the soil
  float organicCarbonPercent;
  
  /// @brief The soil pH (measured in a soil-water solution).
  float pH;
  
  // Useful methods
  SoilHorizon(char* nm);
  SoilHorizon(rapidjson::Value json);
  ~SoilHorizon(void);
  int writeJson(char* buf, unsigned bufSize);
  
  // Static methods
  static bool isValid(rapidjson::Value& json);

  // Member functions - private
  /// @brief Prevent copy-construction.
  SoilHorizon(const SoilHorizon&);       
  /// @brief Prevent assignment.
  SoilHorizon& operator=(const SoilHorizon&);      
};


// =======================================================================================

#endif




