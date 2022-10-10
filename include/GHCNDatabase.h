// Copyright Staniford Systems.  All Rights Reserved.  September 2022 -

#ifndef GHCN_DATABASE_H
#define GHCN_DATABASE_H

#include "RTree.h"
#include "HttpClient.h"
#include <vector>
#include <unordered_map>


// =======================================================================================
// Forward declarations

class ClimateInfo;
class ClimateDatabase;


// =======================================================================================
/// @brief Metadata for a single GHCN station.

/// For more detail, search for 'IV. FORMAT OF "ghcnd-stations.txt"' in
///  https://www.ncei.noaa.gov/pub/data/ghcn/daily/readme.txt

class GHCNStation
{
public:
  // Member functions - public
  GHCNStation(char* buf);
  
  // Instance variables - public
  char id[12];
  float latLong[2];    // degrees
  float elevation;    // in meters
  char name[32];
};


// =======================================================================================
/// @brief Interface to the Global Historical Climatology Network - Daily data.
///
/// This provides global harmonized data for recent daily climate from all over the 
/// world.  The purpose of this class is to provide the data in a usable form to the
/// rest of permaserv.  For more on the dataset:
/// https://www.ncei.noaa.gov/metadata/geoportal/rest/metadata/item/gov.noaa.ncdc:C00861/html.

class GHCNDatabase: public HttpClient
{
  friend ClimateDatabase;
  friend bool searchCallback(GHCNStation*, void*);
  
public:
  
  // Instance variables - public
  
  // Member functions - public
  GHCNDatabase(char* path);
  ~GHCNDatabase(void);
  void readStations(void);
  void checkFileIndex(void);
  void getStations(float lat, float longT);
  int readOneCSVFile(char* fileName, GHCNStation* station, ClimateInfo* climInfo);
  bool fetchCSVFile(GHCNStation* station);

private:
  
  // Instance variables - private
  char* dbPath;
  RTree<GHCNStation*, float, 2> stationTree;
  std::vector<GHCNStation*> stationResults;
  std::unordered_map<char*, GHCNStation*> stationsByName;
  
  // Member functions - private
  /// @brief Prevent copy-construction.
  GHCNDatabase(const GHCNDatabase&);       
  /// @brief Prevent assignment.
  GHCNDatabase& operator=(const GHCNDatabase&);      
};


// =======================================================================================

#endif




