// Copyright Staniford Systems.  All Rights Reserved.  July 2022 -
// This class provides methods and data required to model the state of the soil profile
// relevant to plant growth at the location of a given plant.  The data is extract from
// the Harmonized World Soil Database (https://www.fao.org/3/aq361e/aq361e.pdf).
// Generic data and methods should go in SoilProfile, and this class should only be used
// for things that are HWSD specific.

// NB!!!! This class has two constructors - one from JSON in permaplan, and one for 
// reading from the database in permaserv.

#include "HWSDProfile.h"
#include "SoilHorizon.h"
#include "MdbFile.h"
#include "Logging.h"
#include "Global.h"



// =======================================================================================
/// @brief helper function to error check as we read the database in the constructor.

void HWSDProfile::columnCheck(int column, char* colName, int expectedType)
{
  MdbColumn *col = (MdbColumn*)g_ptr_array_index(hwsdReader->table->columns, column);
  unless(col->col_type == expectedType)
   {
    LogSoilDbErr("Wrong type %d in column %d.\n", col->col_type, column);
    return;
   }
  LogHSWDExhaustive("%s len: %d; val: %s\n", colName, hwsdReader->boundLens[column],
                      hwsdReader->boundValues[column]);
}


// =======================================================================================
/// @brief Constructor used when reading from the Harmonized World Soil Database.
/// 
/// @param hwsdTableReader An MdbTableReader which should already have been initialized
/// and into which the next row should have been read.  We examine the buffers and
/// extract the information into our structures.  See the technical report at
/// https://www.fao.org/3/aq361e/aq361e.pdf for the semantics of the different fields.

HWSDProfile::HWSDProfile(MdbTableReader& hwsdTableReader): hwsdReader(&hwsdTableReader)
{ 
  int i = 0;
  // Global information about the profile

  //[ID]      Long Integer,   
  columnCheck(i, (char*)"ID", MDB_LONGINT);
  dbId = atoi(hwsdReader->boundValues[i++]);
  
  //[MU_GLOBAL]      Long Integer, 
  columnCheck(i, (char*)"MU_GLOBAL", MDB_LONGINT);
  muGlobal = atoi(hwsdReader->boundValues[i++]);
  
  //[MU_SOURCE1]      Text (12), 
  columnCheck(i, (char*)"MU_SOURCE1", MDB_TEXT);
  strncpy(muSource1, hwsdReader->boundValues[i++], 12);
  
  //[MU_SOURCE2]      Long Integer, 
  columnCheck(i, (char*)"MU_SOURCE2", MDB_LONGINT);
  muSource2 = atoi(hwsdReader->boundValues[i++]);

  //[ISSOIL]      Byte, 
  columnCheck(i, (char*)"ISSOIL", MDB_BYTE);
  isSoil = (hwsdReader->boundValues[i++][0] == '1');
  
  //[SHARE]      Single, 
  columnCheck(i, (char*)"SHARE", MDB_FLOAT);
  share = atof(hwsdReader->boundValues[i++]);
  
  //[SEQ]      Byte,
  columnCheck(i, (char*)"SEQ", MDB_BYTE);
  seq = (unsigned char)atoi(hwsdReader->boundValues[i++]);
  
  //[SU_SYM74]      Text (6), 
  columnCheck(i, (char*)"SU_SYM74", MDB_TEXT);
  strncpy(suSym74, hwsdReader->boundValues[i++], 6);

  //[SU_CODE74]      Integer,
  columnCheck(i, (char*)"SU_CODE74", MDB_INT);
  suCode74 = atoi(hwsdReader->boundValues[i++]);

  //[SU_SYM85]      Text (6),
  columnCheck(i, (char*)"SU_SYM85", MDB_TEXT);
  strncpy(suSym85, hwsdReader->boundValues[i++], 6);

  //[SU_CODE85]      Integer,
  columnCheck(i, (char*)"SU_CODE85", MDB_INT);
  suCode85 = atoi(hwsdReader->boundValues[i++]);

  //[SU_SYM90]      Text (6),
  columnCheck(i, (char*)"SU_SYM90", MDB_TEXT);
  strncpy(suSym90, hwsdReader->boundValues[i++], 6);

  //[SU_CODE90]      Integer,
  columnCheck(i, (char*)"SU_CODE90", MDB_INT);
  suCode90 = atoi(hwsdReader->boundValues[i++]);

  //[T_TEXTURE]      Byte,
  columnCheck(i, (char*)"T_TEXTURE", MDB_BYTE);
  tTexture = (unsigned char)atoi(hwsdReader->boundValues[i++]);

  //[DRAINAGE]      Integer,
  columnCheck(i, (char*)"DRAINAGE", MDB_INT);
  drainage = atoi(hwsdReader->boundValues[i++]);

  //[REF_DEPTH]      Integer,
  columnCheck(i, (char*)"REF_DEPTH", MDB_INT);
  refDepth = atoi(hwsdReader->boundValues[i++]);
  
  //[AWC_CLASS]      Byte,
  columnCheck(i, (char*)"AWC_CLASS", MDB_BYTE);
  awcClass = (unsigned char)atoi(hwsdReader->boundValues[i++]);

  //[PHASE1]      Byte,
  columnCheck(i, (char*)"PHASE1", MDB_BYTE);
  phase1 = (unsigned char)atoi(hwsdReader->boundValues[i++]);

  //[PHASE2]      Byte,
  columnCheck(i, (char*)"PHASE2", MDB_BYTE);
  phase2 = (unsigned char)atoi(hwsdReader->boundValues[i++]);

  //[ROOTS]      Byte,
  columnCheck(i, (char*)"ROOTS", MDB_BYTE);
  roots = (unsigned char)atoi(hwsdReader->boundValues[i++]);

  //[IL]      Byte,
  columnCheck(i, (char*)"IL", MDB_BYTE);
  il = (unsigned char)atoi(hwsdReader->boundValues[i++]);

  //[SWR]      Byte,
  columnCheck(i, (char*)"SWR", MDB_BYTE);
  swr = (unsigned char)atoi(hwsdReader->boundValues[i++]);

  //[ADD_PROP]      Byte,
  columnCheck(i, (char*)"ADD_PROP", MDB_BYTE);
  addProp = (unsigned char)atoi(hwsdReader->boundValues[i++]);
  
  
  // Deal with the topsoil qualities
  SoilHorizon* topSoil = new SoilHorizon((char*)"topSoil");
  push_back(topSoil);

  //[T_GRAVEL]      Integer, 
  columnCheck(i, (char*)"T_GRAVEL", MDB_INT);
  topSoil->coarseFragmentFraction = atof(hwsdReader->boundValues[i++])/100.0f;

  //[T_SAND]      Integer, 
  columnCheck(i, (char*)"T_SAND", MDB_INT);
  topSoil->sandFraction = atof(hwsdReader->boundValues[i++])/100.0f;

  //[T_SILT]      Integer, 
  columnCheck(i, (char*)"T_SILT", MDB_INT);
  topSoil->siltFraction = atof(hwsdReader->boundValues[i++])/100.0f;

  //[T_CLAY]      Integer, 
  columnCheck(i, (char*)"T_CLAY", MDB_INT);
  topSoil->clayFraction = atof(hwsdReader->boundValues[i++])/100.0f;

  //[T_USDA_TEX_CLASS]      Byte, 
  columnCheck(i, (char*)"T_USDA_TEX_CLASS", MDB_BYTE);
  topSoil->usdaTextureClass = (USDATextureClass)atoi(hwsdReader->boundValues[i++]);
    
  //[T_REF_BULK_DENSITY]      Double, 
  // See also http://www.pedosphere.com/resources/bulkdensity/index.html. 
  columnCheck(i, (char*)"T_REF_BULK_DENSITY", MDB_DOUBLE);
  topSoil->bulkDensity = atof(hwsdReader->boundValues[i++]);

  //[T_OC]      Double, 
  columnCheck(i, (char*)"T_OC", MDB_DOUBLE);
  topSoil->organicCarbonPercent = atof(hwsdReader->boundValues[i++]);
  
  //[T_PH_H2O]      Double, 
  columnCheck(i, (char*)"T_PH_H2O", MDB_DOUBLE);
  topSoil->pH = atof(hwsdReader->boundValues[i++]);

  //[T_CEC_CLAY]      Double, 
  columnCheck(i, (char*)"T_CEC_CLAY", MDB_DOUBLE);
  topSoil->cecClay = atof(hwsdReader->boundValues[i++]);

  //[T_CEC_SOIL]      Double, 
  columnCheck(i, (char*)"T_CEC_SOIL", MDB_DOUBLE);
  topSoil->cecSoil = atof(hwsdReader->boundValues[i++]);

  //[T_BS]      Double, 
  columnCheck(i, (char*)"T_BS", MDB_DOUBLE);
  topSoil->baseSaturation = atof(hwsdReader->boundValues[i++]);

  //[T_TEB]      Double, 
  columnCheck(i, (char*)"T_TEB", MDB_DOUBLE);
  topSoil->totalExchangeableBases = atof(hwsdReader->boundValues[i++]);

  //[T_CACO3]      Double, 
  columnCheck(i, (char*)"T_CACO3", MDB_DOUBLE);
  topSoil->limeContent = atof(hwsdReader->boundValues[i++]);
    
  //[T_CASO4]      Double, 
  columnCheck(i, (char*)"T_CASO4", MDB_DOUBLE);
  topSoil->gypsumContent = atof(hwsdReader->boundValues[i++]);

  //[T_ESP]      Double, 
  columnCheck(i, (char*)"T_ESP", MDB_DOUBLE);
  topSoil->exchangeableNaPercentage = atof(hwsdReader->boundValues[i++]);

  //[T_ECE]      Double, 
  columnCheck(i, (char*)"T_ECE", MDB_DOUBLE);
  topSoil->electricalConductivity = atof(hwsdReader->boundValues[i++]);

  
  // Deal with the subsoil qualities
  SoilHorizon* subSoil = new SoilHorizon((char*)"subSoil");
  push_back(subSoil);

  //[S_GRAVEL]      Integer, 
  columnCheck(i, (char*)"S_GRAVEL", MDB_INT);
  subSoil->coarseFragmentFraction = atof(hwsdReader->boundValues[i++])/100.0f;

  //[S_SAND]      Integer, 
  columnCheck(i, (char*)"S_SAND", MDB_INT);
  subSoil->sandFraction = atof(hwsdReader->boundValues[i++])/100.0f;

  //[S_SILT]      Integer, 
  columnCheck(i, (char*)"S_SILT", MDB_INT);
  subSoil->siltFraction = atof(hwsdReader->boundValues[i++])/100.0f;

  //[S_CLAY]      Integer, 
  columnCheck(i, (char*)"S_CLAY", MDB_INT);
  subSoil->clayFraction = atof(hwsdReader->boundValues[i++])/100.0f;

  //[S_USDA_TEX_CLASS]      Byte, 
  columnCheck(i, (char*)"S_USDA_TEX_CLASS", MDB_BYTE);
  subSoil->usdaTextureClass = (USDATextureClass)atoi(hwsdReader->boundValues[i++]);
    
  //[S_REF_BULK_DENSITY]      Double, 
  // See also http://www.pedosphere.com/resources/bulkdensity/index.html. 
  columnCheck(i, (char*)"S_REF_BULK_DENSITY", MDB_DOUBLE);
  subSoil->bulkDensity = atof(hwsdReader->boundValues[i++]);
  
  //[S_OC]      Double, 
  columnCheck(i, (char*)"S_OC", MDB_DOUBLE);
  subSoil->organicCarbonPercent = atof(hwsdReader->boundValues[i++]);
  
  //[S_PH_H2O]      Double, 
  columnCheck(i, (char*)"S_PH_H2O", MDB_DOUBLE);
  subSoil->pH = atof(hwsdReader->boundValues[i++]);

  //[S_CEC_CLAY]      Double, 
  columnCheck(i, (char*)"S_CEC_CLAY", MDB_DOUBLE);
  subSoil->cecClay = atof(hwsdReader->boundValues[i++]);

  //[S_CEC_SOIL]      Double, 
  columnCheck(i, (char*)"S_CEC_SOIL", MDB_DOUBLE);
  subSoil->cecSoil = atof(hwsdReader->boundValues[i++]);

  //[S_BS]      Double, 
  columnCheck(i, (char*)"S_BS", MDB_DOUBLE);
  subSoil->baseSaturation = atof(hwsdReader->boundValues[i++]);

  //[S_TEB]      Double, 
  columnCheck(i, (char*)"S_TEB", MDB_DOUBLE);
  subSoil->totalExchangeableBases = atof(hwsdReader->boundValues[i++]);

  //[S_CACO3]      Double, 
  columnCheck(i, (char*)"S_CACO3", MDB_DOUBLE);
  subSoil->limeContent = atof(hwsdReader->boundValues[i++]);
    
  //[S_CASO4]      Double, 
  columnCheck(i, (char*)"S_CASO4", MDB_DOUBLE);
  subSoil->gypsumContent = atof(hwsdReader->boundValues[i++]);

  //[S_ESP]      Double, 
  columnCheck(i, (char*)"S_ESP", MDB_DOUBLE);
  subSoil->exchangeableNaPercentage = atof(hwsdReader->boundValues[i++]);

  //[S_ECE]      S_ECE, 
  columnCheck(i, (char*)"T_ECE", MDB_DOUBLE);
  subSoil->electricalConductivity = atof(hwsdReader->boundValues[i++]);

  // These next two fields are tacked on the end of the database schema but
  // not documented in the techn report.
  
  //[T_BULK_DENSITY]      Double, 
  columnCheck(i, (char*)"T_BULK_DENSITY", MDB_DOUBLE);
  topSoil->bulkDensity = atof(hwsdReader->boundValues[i++]);

  //[S_BULK_DENSITY]      Double, 
  columnCheck(i, (char*)"S_BULK_DENSITY", MDB_DOUBLE);
  subSoil->bulkDensity = atof(hwsdReader->boundValues[i++]);
}


// =======================================================================================
/// @brief Constructor used in permaplan when we've gotten soil information as json from
/// permaserv.
/// 
/// @param soilJson A rapidjson::Document reference with the parsed json received from
/// permaserv.  By the time we get here, we are guaranteed that it's valid JSON, and
/// guaranteed that the dynamicType was correct for us, but otherwise we need to validate.
/// 
/// See the technical report at
/// https://www.fao.org/3/aq361e/aq361e.pdf for the semantics of the different fields.

using namespace rapidjson;

HWSDProfile::HWSDProfile(Document& soilJson)
{
  //[ID]      Long Integer,   
  if(soilJson.HasMember("dbId") && soilJson["dbId"].IsInt())
   {
    dbId = soilJson["dbId"].GetInt();
    LogHSWDExhaustive("Got dbId value of %d in soil json.\n", dbId);
   }
  else
   {
    dbId = 0;
    LogSoilDbErr("Couldn't get dbId from soil json.\n");
   }
    
  //[MU_GLOBAL]      Long Integer, 
  if(soilJson.HasMember("muGlobal") && soilJson["muGlobal"].IsInt())
    {
     muGlobal = soilJson["muGlobal"].GetInt();
     LogHSWDExhaustive("Got muGlobal value of %d in soil json.\n", muGlobal);
    }
   else
    {
     muGlobal = 0u;
     LogSoilDbErr("Couldn't get muGlobal from soil json.\n");
    }

  //[MU_SOURCE1]      Text (12), 
  if(soilJson.HasMember("muSource1") && soilJson["muSource1"].IsString())
   {
    strncpy(muSource1, soilJson["muSource1"].GetString(), 12);
    LogHSWDExhaustive("Got muSource1 value of %s in soil json.\n", muSource1);
   }
  else
   {
    muSource1[0] = '\0';
    LogSoilDbErr("Couldn't get muSource1 from soil json.\n");
   }
   
  //[MU_SOURCE2]      Long Integer, 
  if(soilJson.HasMember("muSource2") && soilJson["muSource2"].IsInt())
   {
    muSource2 = soilJson["muSource2"].GetInt();
    LogHSWDExhaustive("Got muSource2 value of %d in soil json.\n", muSource2);
   }
  else
   {
    muSource2 = 0u;
    LogSoilDbErr("Couldn't get muSource2 from soil json.\n");
   }

  //[ISSOIL]      Byte, 
   if(soilJson.HasMember("isSoil") && soilJson["isSoil"].IsInt())
     {
      int temp = soilJson["isSoil"].GetInt();
      if(temp == 1)
       {
        isSoil = true;
        LogHSWDExhaustive("Got isSoil value of true in soil json.\n");
       }
      else if(temp == 0)
       {
        isSoil = false;
        LogHSWDExhaustive("Got isSoil value of false in soil json.\n");        
       }
      else
       {
        isSoil = false;
        LogSoilDbErr("Got invalid value %d for isSoil from soil json.\n", temp);
       }
     }
    else
     {
      isSoil = false;
      LogSoilDbErr("Couldn't get isSoil from soil json.\n");
     }

  //[SHARE]      Single, 
  if(soilJson.HasMember("share") && soilJson["share"].IsFloat())
   {
    share = soilJson["share"].GetFloat();
    LogHSWDExhaustive("Got share value of %f in soil json.\n", share);
   }
  else
   {
    share = 0.0f;
    LogSoilDbErr("Couldn't get share from soil json.\n");
   }
  
  //[SEQ]      Byte,
   if(soilJson.HasMember("seq") && soilJson["seq"].IsInt())
     {
      int temp = soilJson["seq"].GetInt();
      if(temp >=0 && temp < 256)
       {
        seq = (unsigned char)temp;
        LogHSWDExhaustive("Got seq value of %d in soil json.\n", seq);
       }
      else
       {
        seq = '\0';
        LogSoilDbErr("Got invalid value of %d for seq in soil json.\n", temp);        
       }
     }
    else
     {
      seq = '\0';
      LogSoilDbErr("Couldn't get seq from soil json.\n");
     }
  
  //[SU_SYM74]      Text (6), 
   if(soilJson.HasMember("suSym74") && soilJson["suSym74"].IsString())
    {
     strncpy(suSym74, soilJson["suSym74"].GetString(), 6);
     LogHSWDExhaustive("Got suSym74 value of %s in soil json.\n", suSym74);
    }
   else
    {
     suSym74[0] = '\0';
     LogSoilDbErr("Couldn't get suSym74 from soil json.\n");
    }

  //[SU_CODE74]      Integer,
   if(soilJson.HasMember("suCode74") && soilJson["suCode74"].IsInt())
    {
     suCode74 = soilJson["suCode74"].GetInt();
     LogHSWDExhaustive("Got suCode74 value of %d in soil json.\n", suCode74);
    }
   else
    {
     suCode74 = 0u;
     LogSoilDbErr("Couldn't get suCode74 from soil json.\n");
    }

  //[SU_SYM85]      Text (6),
   if(soilJson.HasMember("suSym85") && soilJson["suSym85"].IsString())
    {
     strncpy(suSym85, soilJson["suSym85"].GetString(), 6);
     LogHSWDExhaustive("Got suSym85 value of %s in soil json.\n", suSym85);
    }
   else
    {
     suSym85[0] = '\0';
     LogSoilDbErr("Couldn't get suSym85 from soil json.\n");
    }

  //[SU_CODE85]      Integer,
   if(soilJson.HasMember("suCode85") && soilJson["suCode85"].IsInt())
    {
     suCode85 = soilJson["suCode85"].GetInt();
     LogHSWDExhaustive("Got suCode85 value of %d in soil json.\n", suCode85);
    }
   else
    {
     suCode85 = 0u;
     LogSoilDbErr("Couldn't get suCode85 from soil json.\n");
    }

  //[SU_SYM90]      Text (6),
   if(soilJson.HasMember("suSym90") && soilJson["suSym90"].IsString())
    {
     strncpy(suSym85, soilJson["suSym90"].GetString(), 6);
     LogHSWDExhaustive("Got suSym90 value of %s in soil json.\n", suSym90);
    }
   else
    {
     suSym90[0] = '\0';
     LogSoilDbErr("Couldn't get suSym90 from soil json.\n");
    }

  //[SU_CODE90]      Integer,
   if(soilJson.HasMember("suCode90") && soilJson["suCode90"].IsInt())
    {
     suCode85 = soilJson["suCode90"].GetInt();
     LogHSWDExhaustive("Got suCode90 value of %d in soil json.\n", suCode90);
    }
   else
    {
     suCode90 = 0u;
     LogSoilDbErr("Couldn't get suCode90 from soil json.\n");
    }

  //[T_TEXTURE]      Byte,
  if(soilJson.HasMember("tTexture") && soilJson["tTexture"].IsInt())
    {
     int temp = soilJson["tTexture"].GetInt();
     if(temp >=0 && temp < 256)
      {
       tTexture = (unsigned char)temp;
       LogHSWDExhaustive("Got tTexture value of %d in soil json.\n", tTexture);
      }
     else
      {
       tTexture = '\0';
       LogSoilDbErr("Got invalid value of %d for tTexture in soil json.\n", temp);        
      }
    }
   else
    {
     tTexture = '\0';
     LogSoilDbErr("Couldn't get tTexture from soil json.\n");
    }

  //[DRAINAGE]      Integer,
   if(soilJson.HasMember("drainage") && soilJson["drainage"].IsInt())
    {
     drainage = soilJson["drainage"].GetInt();
     LogHSWDExhaustive("Got drainage value of %d in soil json.\n", drainage);
    }
   else
    {
     drainage = 0u;
     LogSoilDbErr("Couldn't get drainage from soil json.\n");
    }

  //[REF_DEPTH]      Integer,
 if(soilJson.HasMember("refDepth") && soilJson["refDepth"].IsInt())
  {
   refDepth = soilJson["refDepth"].GetInt();
   LogHSWDExhaustive("Got refDepth value of %d in soil json.\n", refDepth);
  }
 else
  {
   refDepth = 0u;
   LogSoilDbErr("Couldn't get refDepth from soil json.\n");
  }

  //[AWC_CLASS]      Byte,
  if(soilJson.HasMember("awcClass") && soilJson["awcClass"].IsInt())
    {
     int temp = soilJson["awcClass"].GetInt();
     if(temp >=0 && temp < 256)
      {
       awcClass = (unsigned char)temp;
       LogHSWDExhaustive("Got awcClass value of %d in soil json.\n", awcClass);
      }
     else
      {
       awcClass = '\0';
       LogSoilDbErr("Got invalid value of %d for awcClass in soil json.\n", temp);        
      }
    }
   else
    {
     awcClass = '\0';
     LogSoilDbErr("Couldn't get awcClass from soil json.\n");
    }

  //[PHASE1]      Byte,
  if(soilJson.HasMember("phase1") && soilJson["phase1"].IsInt())
    {
     int temp = soilJson["phase1"].GetInt();
     if(temp >=0 && temp < 256)
      {
       phase1 = (unsigned char)temp;
       LogHSWDExhaustive("Got phase1 value of %d in soil json.\n", phase1);
      }
     else
      {
       phase1 = '\0';
       LogSoilDbErr("Got invalid value of %d for phase1 in soil json.\n", temp);        
      }
    }
   else
    {
     phase1 = '\0';
     LogSoilDbErr("Couldn't get phase1 from soil json.\n");
    }

  //[PHASE2]      Byte,
  if(soilJson.HasMember("phase2") && soilJson["phase2"].IsInt())
    {
     int temp = soilJson["phase2"].GetInt();
     if(temp >=0 && temp < 256)
      {
       phase2 = (unsigned char)temp;
       LogHSWDExhaustive("Got phase2 value of %d in soil json.\n", phase2);
      }
     else
      {
       phase2 = '\0';
       LogSoilDbErr("Got invalid value of %d for phase2 in soil json.\n", temp);        
      }
    }
   else
    {
     phase2 = '\0';
     LogSoilDbErr("Couldn't get phase2 from soil json.\n");
    }

  //[ROOTS]      Byte,
  if(soilJson.HasMember("roots") && soilJson["roots"].IsInt())
    {
     int temp = soilJson["roots"].GetInt();
     if(temp >=0 && temp < 256)
      {
       roots = (unsigned char)temp;
       LogHSWDExhaustive("Got roots value of %d in soil json.\n", roots);
      }
     else
      {
       roots = '\0';
       LogSoilDbErr("Got invalid value of %d for roots in soil json.\n", temp);        
      }
    }
   else
    {
     roots = '\0';
     LogSoilDbErr("Couldn't get roots from soil json.\n");
    }

  //[IL]      Byte,
  if(soilJson.HasMember("il") && soilJson["il"].IsInt())
    {
     int temp = soilJson["il"].GetInt();
     if(temp >=0 && temp < 256)
      {
       il = (unsigned char)temp;
       LogHSWDExhaustive("Got il value of %d in soil json.\n", il);
      }
     else
      {
       il = '\0';
       LogSoilDbErr("Got invalid value of %d for il in soil json.\n", temp);        
      }
    }
   else
    {
     il = '\0';
     LogSoilDbErr("Couldn't get il from soil json.\n");
    }

  //[SWR]      Byte,
  if(soilJson.HasMember("swr") && soilJson["swr"].IsInt())
    {
     int temp = soilJson["swr"].GetInt();
     if(temp >=0 && temp < 256)
      {
       swr = (unsigned char)temp;
       LogHSWDExhaustive("Got swr value of %d in soil json.\n", swr);
      }
     else
      {
       swr = '\0';
       LogSoilDbErr("Got invalid value of %d for swr in soil json.\n", temp);        
      }
    }
   else
    {
     swr = '\0';
     LogSoilDbErr("Couldn't get swr from soil json.\n");
    }

  //[ADD_PROP]      Byte,
  if(soilJson.HasMember("addProp") && soilJson["addProp"].IsInt())
    {
     int temp = soilJson["addProp"].GetInt();
     if(temp >=0 && temp < 256)
      {
       addProp = (unsigned char)addProp;
       LogHSWDExhaustive("Got addProp value of %d in soil json.\n", addProp);
      }
     else
      {
       addProp = '\0';
       LogSoilDbErr("Got invalid value of %d for addProp in soil json.\n", temp);        
      }
    }
   else
    {
     addProp = '\0';
     LogSoilDbErr("Couldn't get addProp from soil json.\n");
    }
}


// =======================================================================================  
/// @brief Destructor

HWSDProfile::~HWSDProfile(void)
{
}


// =======================================================================================
/// @brief Output JSON soil profile format to a buffer.
///
/// @returns The number of bytes written to the buffer.  If greater than or equal to
/// the supplied bufSize parameter, it indicates the buffer was not big enough and the
/// output will have been truncated/incomplete.
/// @param buf The char buffer to write the JSON to.
/// @param bufSize The size of the buffer, which must not be overwritten after the end.

int HWSDProfile::writeJsonFields(char* buf, unsigned bufSize)
{
  char* end = buf + bufSize;
  
  buf += SoilProfile::writeJsonFields(buf, bufSize);
  bufprintf(",\n")
  
  // Write our own fields
  bufprintf("\"dbId\": %d,\n", dbId);
  bufprintf("\"muGlobal\": %d,\n", muGlobal);
  bufprintf("\"muSource1\": \"%s\",\n", muSource1);
  bufprintf("\"muSource2\": %d,\n", muSource2);
  bufprintf("\"isSoil\": %d,\n", isSoil);
  bufprintf("\"share\": %.3f,\n", share);
  bufprintf("\"seq\": %d,\n", seq);
  bufprintf("\"suSym74\": \"%s\",\n", suSym74);
  bufprintf("\"suCode74\": %d,\n", suCode74);
  bufprintf("\"suSym85\": \"%s\",\n", suSym85);
  bufprintf("\"suCode85\": %d,\n", suCode85);
  bufprintf("\"suSym90\": \"%s\",\n", suSym90);
  bufprintf("\"suCode90\": %d,\n", suCode90);
  bufprintf("\"suCode90\": %d,\n", suCode90);
  bufprintf("\"tTexture\": %d,\n", tTexture);
  bufprintf("\"drainage\": %d,\n", drainage);
  bufprintf("\"refDepth\": %d,\n", refDepth);
  bufprintf("\"awcClass\": %d,\n", awcClass);
  bufprintf("\"phase1\": %d,\n", phase1);
  bufprintf("\"phase2\": %d,\n", phase2);
  bufprintf("\"roots\": %d,\n", roots);
  bufprintf("\"il\": %d,\n", il);
  bufprintf("\"swr\": %d,\n", swr);
  bufprintf("\"addProp\": %d\n", addProp);  // last one, no comma

  return bufSize - (end-buf);
}


// =======================================================================================
