// Copyright Staniford Systems.  All Rights Reserved.  July 2022 -
// This class provides methods and data required to model the state of the soil profile
// relevant to plant growth at the location of a given plant.  The data is extract from
// the Harmonized World Soil Database (https://www.fao.org/3/aq361e/aq361e.pdf).
// Generic data and methods should go in SoilProfile, and this class should only be used
// for things that are HWSD specific.

#include "HWSDProfile.h"
#include "SoilHorizon.h"


// =======================================================================================
/// @brief Constructor used when reading from the Harmonized World Soil Database.
/// 
/// @param hwsdTableReader An MdbTableReader which should already have been initialized
/// and into which the next row should have been read.  We examine the buffers and
/// extract the information into our structures.

HWSDProfile::HWSDProfile(MdbTableReader& hwsdTableReader)
{ 
  // Global information about the profile

  //[ID]      Long Integer,   
  //[MU_GLOBAL]      Long Integer, 
  //[MU_SOURCE1]      Text (12), 
  //[MU_SOURCE2]      Long Integer, 
  //[ISSOIL]      Byte, 
  //[SHARE]      Single, 
  //[SEQ]      Byte, 
  //[SU_SYM74]      Text (6), 
  //[SU_CODE74]      Integer, 
  //[SU_SYM85]      Text (6), 
  //[SU_CODE85]      Integer, 
  //[SU_SYM90]      Text (6), 
  //[SU_CODE90]      Integer, 
  //[T_TEXTURE]      Byte, 
  //[DRAINAGE]      Integer, 
  //[REF_DEPTH]      Integer, 
  //[AWC_CLASS]      Byte, 
  //[PHASE1]      Byte, 
  //[PHASE2]      Byte, 
  //[ROOTS]      Byte, 
  //[IL]      Byte, 
  //[SWR]      Byte, 
  //[ADD_PROP]      Byte, 
  
  // Deal with the topsoil qualities
  SoilHorizon* topSoil = new SoilHorizon((char*)"topSoil");
  push_back(topSoil);

  //[T_GRAVEL]      Integer, 
  //[T_SAND]      Integer, 
  //[T_SILT]      Integer, 
  //[T_CLAY]      Integer, 
  //[T_USDA_TEX_CLASS]      Byte, 
  //[T_REF_BULK_DENSITY]      Double, 
  //[T_OC]      Double, 
  //[T_PH_H2O]      Double, 
  //[T_CEC_CLAY]      Double, 
  //[T_CEC_SOIL]      Double, 
  //[T_BS]      Double, 
  //[T_TEB]      Double, 
  //[T_CACO3]      Double, 
  //[T_CASO4]      Double, 
  //[T_ESP]      Double, 
  //[T_ECE]      Double, 

  // Deal with the subsoil qualities
  SoilHorizon* subSoil = new SoilHorizon((char*)"subSoil");
  push_back(subSoil);

  //[S_GRAVEL]      Integer, 
  //[S_SAND]      Integer, 
  //[S_SILT]      Integer, 
  //[S_CLAY]      Integer, 
  //[S_USDA_TEX_CLASS]      Byte, 
  //[S_REF_BULK_DENSITY]      Double, 
  //[S_OC]      Double, 
  //[S_PH_H2O]      Double, 
  //[S_CEC_CLAY]      Double, 
  //[S_CEC_SOIL]      Double, 
  //[S_BS]      Double, 
  //[S_TEB]      Double, 
  //[S_CACO3]      Double, 
  //[S_CASO4]      Double, 
  //[S_ESP]      Double, 
  //[S_ECE]      Double, 
  //[T_BULK_DENSITY]      Double, 
  //[S_BULK_DENSITY]      Double

}


// =======================================================================================
/// @brief Destructor

HWSDProfile::~HWSDProfile(void)
{
}


// =======================================================================================
