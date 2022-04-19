// Copyright Staniford Systems.  All Rights Reserved.  February 2022 -
// This class provides an interface to databases of information required to provide soil
// profiles for various locations to permaplan instances (as part of permaserv).  
// Currently, it uses BILFile to provide information from the Harmonized World Soil 
// Database v 1.2
//https://www.fao.org/soils-portal/soil-survey/soil-maps-and-databases/harmonized-world-soil-database-v12/en/

#include "SoilDatabase.h"
#include "SoilProfile.h"
#include "Logging.h"
#include <stdio.h>

char* worldSoilBilFileName = (char*)"Materials/Solar/HWSD_RASTER/hwsd";

char* worldSoilMdbDBName = (char*)"Materials/Solar/HWSD.mdb";

// =======================================================================================
/// @brief Constructor

SoilDatabase::SoilDatabase(void):
                              worldSoilBilFile(worldSoilBilFileName),
                              worldSoilMdbDatabase(worldSoilMdbDBName)
{
}


// =======================================================================================
/// @brief Destructor

SoilDatabase::~SoilDatabase(void)
{
}


// =======================================================================================
/// @brief Output JSON soil profile format for a particular location to a buffer.
///
/// Note this function does not itself write out JSON, but rather is a gateway which
/// selects the right database (based on the location), retrieves a SoilProfile object
/// from that database, and then delegates actually generating the correct JSON output 
/// to the SoilProfile class.
/// 
/// @returns The number of bytes written to the buffer.  If greater than or equal to 
/// the supplied bufSize parameter, it indicates the buffer was not big enough and the
/// output will have been truncated/incomplete.
/// @param buf The char buffer to write the JSON to.
/// @param bufSize The size of the buffer, which must not be overwritten after the end.
/// @param lat The latitude selected.
/// @param longt The longtitude selected.
/// @todo Currently we always pull from the Harmonized World Soil Database, but ultimately
/// we should select the best database for a given location.

unsigned SoilDatabase::printJsonSoilProfileAtPoint(char* buf, unsigned bufSize, 
                                                            float lat, float longt)
{
  unsigned short soilIndex = worldSoilBilFile.valueAtPoint(lat, longt);
  if(!soilIndex)
   {
    LogSoilDbErr("Could not get soil index for lat,long: %.3f, %.3f.\n", lat, longt);
    return 0u;    
   }
  LogSoilDbOps("Obtained soil index %u for lat,long: %.3f, %.3f.\n", 
                                                                  soilIndex, lat, longt);

  //unsigned retVal = soilProfile->writeJson(buf, bufSize);
  //delete soilProfile;
  //return retVal;
  return 0u;
}


// =======================================================================================
