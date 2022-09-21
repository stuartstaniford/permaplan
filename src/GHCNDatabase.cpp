// Copyright Staniford Systems.  All Rights Reserved.  September 2022 -
// Interface to the Global Historical Climatology Network - Daily data.  This provides 
// global harmonized data for recent daily climate from all over the world.  The purpose 
// of this class is to provide the data in a usable form to the rest of permaserv.  For 
// more on the dataset:
// https://www.ncei.noaa.gov/metadata/geoportal/rest/metadata/item/gov.noaa.ncdc:C00861/html

#include "GHCNDatabase.h"
#include "Logging.h"
#include <string.h>
#include <stdio.h>


// =======================================================================================
/// @brief Constructor

GHCNDatabase::GHCNDatabase(char* path): dbPath(path)
{
  readStations();
}


// =======================================================================================
/// @brief Destructor

GHCNDatabase::~GHCNDatabase(void)
{
}


// =======================================================================================
/// @brief Read all the stations into our data structures.
///
/// For more detail, search for 'IV. FORMAT OF "ghcnd-stations.txt"' in
///  https://www.ncei.noaa.gov/pub/data/ghcn/daily/readme.txt

void GHCNDatabase::readStations(void)
{
  // Figure out the name of the station file
  char stationFileName[128];
  if(snprintf(stationFileName, 128, "%s/ghcnd-stations.txt", dbPath) >= 128)
   {
    LogClimateDbErr("Overflow in stationFileName in GHCNDatabase::readStations.\n");
    return;
   };
 
  // Open the station file
  FILE* stationFile = fopen(stationFileName, "r");
  unless(stationFile)
   {
    LogClimateDbErr("Couldn't open GHCNDatabase station file %s.\n", stationFileName);
    return;
   };
  
  // Loop over the lines in the file
  char buf[96];
  while(fgets(buf, 96, stationFile))
   {
    // Read a line and create a GHCNStation
    GHCNStation* station = new GHCNStation(buf);
#ifdef LOG_CLIMATE_DB_ERR
    int line = 1;
#endif
    unless(station)
     {
      LogClimateDbErr("Couldn't create new station in line %d of station file %s.\n",
                                                            line++, stationFileName);
      return;
     }
    
    // Store the GHCNStation in our data structure

   }
  
  // Close up and go home
  fclose(stationFile);
}


// =======================================================================================
/// @brief Create a single station from a line in the file.
///
/// @param buf A char* pointing to the line buffer

// ------------------------------
// Variable   Columns   Type
// ------------------------------
// ID            1-11   Character
// LATITUDE     13-20   Real
// LONGITUDE    22-30   Real
// ELEVATION    32-37   Real
// STATE        39-40   Character
// NAME         42-71   Character
// GSN FLAG     73-75   Character
// HCN/CRN FLAG 77-79   Character
// WMO ID       81-85   Character
// ------------------------------

GHCNStation::GHCNStation(char* buf)
{
  
}


// =======================================================================================
