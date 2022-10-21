// Copyright Staniford Systems.  All Rights Reserved.  February 2022 -
// This class provides an interface to databases of information required to provide soil
// profiles for various locations to permaplan instances (as part of permaserv).  
// Currently, it uses BILFile to provide information from the Harmonized World Soil 
// Database v 1.2
//https://www.fao.org/soils-portal/soil-survey/soil-maps-and-databases/harmonized-world-soil-database-v12/en/
// See also the tech report at https://www.fao.org/3/aq361e/aq361e.pdf

#include "SoilDatabase.h"
#include "SoilProfile.h"
#include "Logging.h"
#include "HWSDProfile.h"
#include "Global.h"

#include <stdio.h>

char* worldSoilBilFileName = (char*)"Materials/Soil/hwsd";

char* worldSoilMdbDBName = (char*)"Materials/Soil/HWSD.mdb";

// =======================================================================================
/// @brief Constructor

SoilDatabase::SoilDatabase(void):
                              worldSoilBilFile(worldSoilBilFileName),
                              worldSoilMdbDatabase(worldSoilMdbDBName)
{
  loadHWSDProfiles();
}


// =======================================================================================
/// @brief Destructor

SoilDatabase::~SoilDatabase(void)
{
  for(auto profile: worldSoilProfiles)
    delete profile.second;
}


// =======================================================================================
/// @brief Create the schema for the HWSD table 

void SoilDatabase::createHWSDSchema(void)
{
}


// =======================================================================================
/// @brief Read the table of all the soil profiles at startup.
/// 
/// Reads them from the MS Access database worldSoilMdbDBName and stores them all in 
/// the worldSoilProfiles unordered_map in memory for later efficient retrieval (a single
/// hash look-up).

void SoilDatabase::loadHWSDProfiles(void)
{
  MdbTableReader hwsdTable(worldSoilMdbDatabase.mdb, (char*)"HWSD_DATA", 2048);
  HWSDProfile* soil;
  
  while(hwsdTable.getNextRow())
   {
    soil = new HWSDProfile(hwsdTable);
    worldSoilProfiles[soil->muGlobal] = soil;
    LogSoilDbOps("Got HWSD profile for soil index %u.\n",  soil->muGlobal);
   }
}


/// =======================================================================================
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
/// @param loLat The low end of the latitude range requested.
/// @param hiLat The high end of the latitude range requested.
/// @param hiLong The low end of the longtitude range requested.
/// @param longt The high end of the longtitude range requested.
/// @todo Currently we always pull from the Harmonized World Soil Database, but ultimately
/// we should select the best database for a given location.
/// @todo Currently we only accommodate finding and returning a single profile, even if
/// there were actually more to find (we are given lat/long intervals and would ideally
/// return all profile data from that rectangle).

unsigned SoilDatabase::printJsonSoilProfiles(char* buf, unsigned bufSize, 
                                      float loLat, float hiLat, float loLong, float hiLong)
{
  float lat = (loLat + hiLat)/2.0f;
  float longt = (loLong + hiLong)/2.0f;
  
  unsigned short soilIndex = worldSoilBilFile.valueAtPoint(lat, longt);
  unless(soilIndex)
   {
    LogSoilDbErr("Could not get soil index for lat,long: %.3f, %.3f.\n", lat, longt);
    return 0u;    
   }
  LogSoilDbOps("Obtained soil index %u for lat,long: %.3f, %.3f.\n", 
                                                                  soilIndex, lat, longt);
  unless(worldSoilProfiles.count(soilIndex))
   {
    LogSoilDbErr("Could not get profile for soilIndex %u at lat,long: %.3f, %.3f.\n", 
                                                                    soilIndex, lat, longt);
    return 0u;    
   }
  
  SoilProfile* soilProfile = worldSoilProfiles[soilIndex];
  unsigned retVal = soilProfile->writeJson(buf, bufSize);
  return retVal;
}


// =======================================================================================
