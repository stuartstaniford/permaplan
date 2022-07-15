// Copyright Staniford Systems.  All Rights Reserved.  February 2022 -

#ifndef SOIL_DATABASE_H
#define SOIL_DATABASE_H

#include "BILFile.h"
#include "MdbFile.h"
#include <unordered_map>


// =======================================================================================
/// @brief Interface to soil databases.
///  
/// This class provides an interface to databases of information required to provide soil
/// profiles for various locations to permaplan instances (as part of permaserv).  
/// Currently, it uses BILFile to provide information from the Harmonized World Soil 
/// Database v 1.2
///https://www.fao.org/soils-portal/soil-survey/soil-maps-and-databases/harmonized-world-soil-database-v12/en/

class SoilDatabase
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  SoilDatabase(void);
  ~SoilDatabase(void);
  void loadHWSDProfiles(void);
  unsigned printJsonSoilProfileAtPoint(char* buf, unsigned bufSize, float lat, float longt);
  void createHWSDSchema(void);

private:
  
  // Instance variables - private
  BILFile worldSoilBilFile;
  MdbDatabase worldSoilMdbDatabase;
  std::unordered_map<unsigned, SoilProfile*> worldSoilProfiles;
  MdbTableSchema hwsdSchema;
  
  // Member functions - private
  /// @brief Prevent copy-construction.
  SoilDatabase(const SoilDatabase&);       
  /// @brief Prevent assignment.
  SoilDatabase& operator=(const SoilDatabase&);      
};


// =======================================================================================

#endif




