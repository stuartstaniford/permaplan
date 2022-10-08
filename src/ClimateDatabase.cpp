// Copyright Staniford Systems.  All Rights Reserved.  September 2022 -
// This class provides climate information within permaserv.  It serves, for any given 
// location, information about the current/recent climate, as well as projections of 
// future changed climate

#include "ClimateDatabase.h"
#include "GHCNDatabase.h"
#include "loadFileToBuf.h"
#include "Logging.h"


// =======================================================================================
// Static variables, etc

char* ghcnPath = (char*)"Materials/Climate/GHCN";


// =======================================================================================
/// @brief Constructor

ClimateDatabase::ClimateDatabase(void)
{
  ghcnDatabase = new GHCNDatabase(ghcnPath);
}


// =======================================================================================
/// @brief Destructor

ClimateDatabase::~ClimateDatabase(void)
{
}


/// =======================================================================================
/// @brief Output JSON climate data for a particular location to a buffer.
///
/// Note the current model is we output everything in a big lump on a single request.
/// There might be an argument for sending it in smaller lumps so the client can get
/// started without waiting for everything, but that would require a more stateful server.
/// 
/// @returns The number of bytes written to the buffer.  If greater than or equal to 
/// the supplied bufSize parameter, it indicates the buffer was not big enough and the
/// output will have been truncated/incomplete.
/// @param buf The char buffer to write the JSON to.
/// @param bufSize The size of the buffer, which must not be overwritten after the end.
/// @param lat The latitude selected.
/// @param longt The longtitude selected.
/// @param yearCount The number of years of climate data to provide (assumed to start in
/// the present year).
/// @todo We currently just return the first remotely viable station result, instead 
/// of properly analyzing them and picking the best or a suitably adjusted admixture
/// @todo Most of this function should probably be in GHCN instead of here.

unsigned ClimateDatabase::printClimateJson(char* buf, unsigned bufSize, 
                                        float lat, float longt, unsigned yearCount)
{
  ghcnDatabase->getStations(lat, longt);
  
  int N = ghcnDatabase->stationResults.size();
  char fileName[128];
  
  snprintf(fileName, 128, "%s/", ghcnPath);
  char* lastEl = fileName + strlen(ghcnPath) + 1;
  int remaining = 128 - (lastEl-fileName);
  
  for(int i=0; i<N; i++)
   {
    GHCNStation* station = ghcnDatabase->stationResults[i]; 
    snprintf(lastEl, remaining, "%s.csv", station->id);
    if(regularFileExists(fileName))
     {
      LogGHCNExhaustive("Found file for station %s: %s.\n", station->id, fileName);
     }
    else
      ghcnDatabase->fetchCSVFile(station);
    
    // lame temp hack
   }
  
  return 0u;
}


// =======================================================================================
