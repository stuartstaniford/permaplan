// Copyright Staniford Systems.  All Rights Reserved.  February 2022 -

#ifndef SOIL_HORIZON_H
#define SOIL_HORIZON_H

#include "GroundLayer.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/error/en.h"


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

  /// @brief The cation exchange capacity of the clay component of the soil (cmol/Kg).
  float cecClay;

  /// @brief The cation exchange capacity of the whole soil (cmol/Kg).
  float cecSoil;

  /// @brief the sum of exchangeable cations (nutrients) Na, Ca, Mg and K as a 
  /// percentage of the overall exchange capacity of the soil (including the 
  /// same cations plus H and Al)..
  float baseSaturation;

  /// @brief Total exchangeable bases in the soil horizon.
  float totalExchangeableBases;

  /// @brief Total lime (calcium carbonate) content in the soil horizon.
  float limeContent;

  /// @brief Total gypsum (calcium sulphate) content in the soil horizon.
  float gypsumContent;

  /// @brief Exchangeable sodium percentage in the soil horizon.
  float exchangeableNaPercentage;

  /// @brief Electrical Conductivity of the soil (dS/m).
  float electricalConductivity;

  /// @brief Bulk Density (extra fields at end of HWSD).
  float bulkDensityNonRef;
  
  // Useful methods
  SoilHorizon(char* nm);
  SoilHorizon(rapidjson::Value& json);
  ~SoilHorizon(void);
  virtual DynamicType getDynamicType(void) {return TypeSoilHorizon;}
  virtual int writeJsonFields(char* buf, unsigned bufSize);

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




