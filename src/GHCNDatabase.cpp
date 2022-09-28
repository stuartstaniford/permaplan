// Copyright Staniford Systems.  All Rights Reserved.  September 2022 -
// Interface to the Global Historical Climatology Network - Daily data.  This provides 
// global harmonized data for recent daily climate from all over the world.  The purpose 
// of this class is to provide the data in a usable form to the rest of permaserv.  For 
// more on the dataset:
// https://www.ncei.noaa.gov/metadata/geoportal/rest/metadata/item/gov.noaa.ncdc:C00861/html

#include "GHCNDatabase.h"
#include "ClimateInfo.h"
#include "Logging.h"
#include <string.h>
#include <stdio.h>
#include <ctype.h>


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
    
    // Store the GHCNStation in our R-tree data structure
    stationTree.Insert(station->latLong, station->latLong, station);
   }
  
  // Close up and go home
  fclose(stationFile);
}


// =======================================================================================
/// @brief Callback for use in R-tree search in GHCNDatabase::getStations.

bool searchCallback(GHCNStation* station, void* context)
{
  return true; // keep going  
}


// =======================================================================================
/// @brief Retrieve the closest stations from our data structures.
///
/// @param lat A float containing the latitude to search for
/// @param longT A float containing the longtitude to search for
/// @todo We are searching on raw lat/long squares here, which will tend to get skeevy
/// towards the poles - might want to add a cos(lat) factor.

void GHCNDatabase::getStations(float lat, float longT)
{
  float searchBound = 0.25f;  // half the side of the search rectangle in degrees.
  float searchMin[2];
  float searchMax[2];
  
  while(1) // keep adjusting the search rectangle until we get a good result
   {
    searchMin[0] = lat    - searchBound;
    searchMin[1] = longT  - searchBound;
    searchMax[0] = lat    + searchBound;
    searchMax[1] = longT  + searchBound;
    
    int hits = stationTree.Search(searchMin, searchMax, searchCallback, NULL);
    LogGHCNExhaustive("Got %d results in search with %.4f degrees of [%.4f, %.4f].\n",
                      hits, searchBound, lat, longT);
    break; // temp
   }
}


// =======================================================================================
/// @brief Read a single .dly file.
///
/// @param fileName A char* pointing to the file name to be opened.

// https://www.ncei.noaa.gov/pub/data/ghcn/daily/readme.txt

bool GHCNDatabase::readOneCSVFile(char* fileName, GHCNStation* station, ClimateInfo* climInfo)
{
  // Open the file
  FILE* file = fopen(fileName, "r");
  unless(file)
   {
    LogClimateDbErr("Couldn't open station csv file %s.\n", fileName);
    return false;
   }
  
  // Loop over the lines in the file
  char buf[128];
  int line = 1;
  while(fgets(buf, 128, file))
   {
    // USC00304174,18930101,TMAX,67,,,6,
    
    // Station id
    if(buf[11] != ',')
     {
      LogClimateDbErr("Bad station id comma line %d of csv file %s.\n", 
                                                                          line, fileName);
      goto ERROR_RETURN;
     }
    buf[11] = '\0';
    unless(strncmp(buf, station->id, 11) == 0)
    {
     LogClimateDbErr("Station id %s does not match %s in csv file %s, line %d.\n", 
                                                      buf, station->id, fileName, line);
     goto ERROR_RETURN;
    }
    
    // Date
    if(buf[20] != ',')
     {
      LogClimateDbErr("Bad date comma line %d of csv file %s.\n", line, fileName);
      goto ERROR_RETURN;
     }
    char dateBuf[5];
    int year, month, day;
    for(int i=0;i<4;i++)
      dateBuf[i] = buf[21+i];
    dateBuf[4] = '\0';
    year  = atoi(dateBuf);
    month = 10*buf[25] + buf[26];
    day   = 10*buf[27] + buf[28];
    
    // Observation type
    
    // Observation
    
    // Flags
    
    
    line++;
   }
  
  // Close up and go home
  fclose(file);
  return true;
  
ERROR_RETURN:
  fclose(file);
  return false;
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
  strncpy(id, buf, 11);
  id[11] = '\0';
  for(int i = 10; isspace(id[i]); i--)
    id[i] = '\0';
  
  strncpy(name, buf+41, 30);
  name[30] = '\0';
  for(int i = 29; isspace(name[i]); i--)
    name[i] = '\0';
  
  latLong[0]  = atof(buf+12);
  latLong[1]  = atof(buf+21);
  elevation   = atof(buf+31);  // in meters, as we are in permaserv
  
  LogGHCNExhaustive("Read station %s (%s) at [%.4f, %.4f], el: %.1fm.\n",
                                              id, name, latLong[0], latLong[1], elevation);
}


// =======================================================================================
