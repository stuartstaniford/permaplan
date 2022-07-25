// Copyright Staniford Systems.  All Rights Reserved.  July 2022 -
// This class provides methods and data required to model the state of the soil profile
// relevant to plant growth at the location of a given plant.  The data is extract from
// the Harmonized World Soil Database (https://www.fao.org/3/aq361e/aq361e.pdf).
// Generic data and methods should go in SoilProfile, and this class should only be used
// for things that are HWSD specific.

#include "HWSDProfile.h"
#include "SoilHorizon.h"
#include "MdbFile.h"
#include "Logging.h"
#include "Global.h"



// =======================================================================================
/// @brief helper function to error check as we read the database in the constructor.

void HWSDProfile::columnCheck(int column, char* colName, int expectedType)
{
  MdbColumn *col = (MdbColumn*)g_ptr_array_index(hwsdReader.table->columns, column);
  unless(col->col_type == expectedType)
   {
    LogSoilDbErr("Wrong type %d in column %d.\n", col->col_type, column);
    return;
   }
  LogHSWDExhaustive("%s len: %d; val: %s\n", colName, hwsdReader.boundLens[column],
                      hwsdReader.boundValues[column]);
}


// =======================================================================================
/// @brief Constructor used when reading from the Harmonized World Soil Database.
/// 
/// @param hwsdTableReader An MdbTableReader which should already have been initialized
/// and into which the next row should have been read.  We examine the buffers and
/// extract the information into our structures.  See the technical report at
/// https://www.fao.org/3/aq361e/aq361e.pdf for the semantics of the different fields.

HWSDProfile::HWSDProfile(MdbTableReader& hwsdTableReader): hwsdReader(hwsdTableReader)
{ 
  int i = 0;
  // Global information about the profile

  //[ID]      Long Integer,   
  columnCheck(i, (char*)"ID", MDB_LONGINT);
  dbId = atoi(hwsdReader.boundValues[i++]);
  
  //[MU_GLOBAL]      Long Integer, 
  columnCheck(i, (char*)"MU_GLOBAL", MDB_LONGINT);
  muGlobal = atoi(hwsdReader.boundValues[i++]);
  
  //[MU_SOURCE1]      Text (12), 
  columnCheck(i, (char*)"MU_SOURCE1", MDB_TEXT);
  strncpy(muSource1, hwsdReader.boundValues[i++], 12);
  
  //[MU_SOURCE2]      Long Integer, 
  columnCheck(i, (char*)"MU_SOURCE2", MDB_LONGINT);
  muSource2 = atoi(hwsdReader.boundValues[i++]);

  //[ISSOIL]      Byte, 
  columnCheck(i, (char*)"ISSOIL", MDB_BYTE);
  isSoil = (hwsdReader.boundValues[i++][0] == '1');
  
  //[SHARE]      Single, 
  columnCheck(i, (char*)"SHARE", MDB_FLOAT);
  share = atof(hwsdReader.boundValues[i++]);
  
  //[SEQ]      Byte,
  columnCheck(i, (char*)"SEQ", MDB_BYTE);
  seq = (unsigned char)atoi(hwsdReader.boundValues[i++]);
  
  //[SU_SYM74]      Text (6), 
  columnCheck(i, (char*)"SU_SYM74", MDB_TEXT);
  strncpy(suSym74, hwsdReader.boundValues[i++], 6);

  //[SU_CODE74]      Integer,
  columnCheck(i, (char*)"SU_CODE74", MDB_INT);
  suCode74 = atoi(hwsdReader.boundValues[i++]);

  //[SU_SYM85]      Text (6),
  columnCheck(i, (char*)"SU_SYM85", MDB_TEXT);
  strncpy(suSym85, hwsdReader.boundValues[i++], 6);

  //[SU_CODE85]      Integer,
  columnCheck(i, (char*)"SU_CODE85", MDB_INT);
  suCode85 = atoi(hwsdReader.boundValues[i++]);

  //[SU_SYM90]      Text (6),
  columnCheck(i, (char*)"SU_SYM90", MDB_TEXT);
  strncpy(suSym90, hwsdReader.boundValues[i++], 6);

  //[SU_CODE90]      Integer,
  columnCheck(i, (char*)"SU_CODE90", MDB_INT);
  suCode90 = atoi(hwsdReader.boundValues[i++]);

  //[T_TEXTURE]      Byte,
  columnCheck(i, (char*)"T_TEXTURE", MDB_BYTE);
  tTexture = (unsigned char)atoi(hwsdReader.boundValues[i++]);

  //[DRAINAGE]      Integer,
  columnCheck(i, (char*)"DRAINAGE", MDB_INT);
  drainage = atoi(hwsdReader.boundValues[i++]);

  //[REF_DEPTH]      Integer,
  columnCheck(i, (char*)"REF_DEPTH", MDB_INT);
  refDepth = atoi(hwsdReader.boundValues[i++]);
  
  //[AWC_CLASS]      Byte,
  columnCheck(i, (char*)"AWC_CLASS", MDB_BYTE);
  awcClass = (unsigned char)atoi(hwsdReader.boundValues[i++]);

  //[PHASE1]      Byte,
  columnCheck(i, (char*)"PHASE1", MDB_BYTE);
  phase1 = (unsigned char)atoi(hwsdReader.boundValues[i++]);

  //[PHASE2]      Byte,
  columnCheck(i, (char*)"PHASE2", MDB_BYTE);
  phase2 = (unsigned char)atoi(hwsdReader.boundValues[i++]);

  //[ROOTS]      Byte,
  columnCheck(i, (char*)"ROOTS", MDB_BYTE);
  roots = (unsigned char)atoi(hwsdReader.boundValues[i++]);

  //[IL]      Byte,
  columnCheck(i, (char*)"IL", MDB_BYTE);
  il = (unsigned char)atoi(hwsdReader.boundValues[i++]);

  //[SWR]      Byte,
  columnCheck(i, (char*)"SWR", MDB_BYTE);
  swr = (unsigned char)atoi(hwsdReader.boundValues[i++]);

  //[ADD_PROP]      Byte,
  columnCheck(i, (char*)"ADD_PROP", MDB_BYTE);
  addProp = (unsigned char)atoi(hwsdReader.boundValues[i++]);
  
  
  // Deal with the topsoil qualities
  SoilHorizon* topSoil = new SoilHorizon((char*)"topSoil");
  push_back(topSoil);

  //[T_GRAVEL]      Integer, 
  columnCheck(i, (char*)"T_GRAVEL", MDB_INT);
  topSoil->coarseFragmentFraction = atof(hwsdReader.boundValues[i++])/100.0f;

  //[T_SAND]      Integer, 
  columnCheck(i, (char*)"T_SAND", MDB_INT);
  topSoil->sandFraction = atof(hwsdReader.boundValues[i++])/100.0f;

  //[T_SILT]      Integer, 
  columnCheck(i, (char*)"T_SILT", MDB_INT);
  topSoil->siltFraction = atof(hwsdReader.boundValues[i++])/100.0f;

  //[T_CLAY]      Integer, 
  columnCheck(i, (char*)"T_CLAY", MDB_INT);
  topSoil->clayFraction = atof(hwsdReader.boundValues[i++])/100.0f;

  //[T_USDA_TEX_CLASS]      Byte, 
  columnCheck(i, (char*)"T_USDA_TEX_CLASS", MDB_BYTE);
  topSoil->usdaTextureClass = (USDATextureClass)atoi(hwsdReader.boundValues[i++]);
    
  //[T_REF_BULK_DENSITY]      Double, 
  // See also http://www.pedosphere.com/resources/bulkdensity/index.html. 
  columnCheck(i, (char*)"T_REF_BULK_DENSITY", MDB_DOUBLE);
  topSoil->bulkDensity = atof(hwsdReader.boundValues[i++]);

  //[T_OC]      Double, 
  columnCheck(i, (char*)"T_OC", MDB_DOUBLE);
  topSoil->organicCarbonPercent = atof(hwsdReader.boundValues[i++]);
  
  //[T_PH_H2O]      Double, 
  columnCheck(i, (char*)"T_PH_H2O", MDB_DOUBLE);
  topSoil->pH = atof(hwsdReader.boundValues[i++]);

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

  LogFlush();
}


// =======================================================================================
/// @brief Destructor

HWSDProfile::~HWSDProfile(void)
{
}


// =======================================================================================
