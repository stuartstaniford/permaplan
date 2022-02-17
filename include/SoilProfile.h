// Copyright Staniford Systems.  All Rights Reserved.  February 2022 -

#ifndef SOIL_PROFILE_H
#define SOIL_PROFILE_H

#include <vector>


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


char* USDATextureName[] = {
                            (char*)"NoTextureClass", 
                            (char*)"HeavyClay",
                            (char*)"SiltyClay",
                            (char*)"Clay",
                            (char*)"SiltyClayLoam",
                            (char*)"ClayLoam",
                            (char*)"Silt",
                            (char*)"SiltyLoam",
                            (char*)"SandyClay",
                            (char*)"Loam",
                            (char*)"SandyClayLoam",
                            (char*)"SandyLoam",
                            (char*)"LoamySand",
                            (char*)"Sand"
};

// =======================================================================================
/// @brief POD Helper class for a single soil horizon in a soil profile
///
/// This class represents one unit of soil in a soil profile (eg taken from some database
/// or soil survey map).  For example the topsoil (or "A horizon") might be an example.

class SoilHorizon
{
  public:
  
  /// @brief The depth of this particular soil layer (cm).
  float depth;                  
  
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
  
  /// @brief The reference bulk density of the soil (kg/l) 
  float bulkDensity;  
  
  /// @brief The weight fraction of carbon in organic molecules in the fine earth
  /// component of the soil.  For useful background see:
  /// https://www.soilquality.org.au/factsheets/organic-carbon
  float organicCarbonFraction;
  
  /// @brief The soil pH (measured in a soil-water solution).
  float pH;
};


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

class SoilProfile: std::vector<SoilHorizon>
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




