// Copyright Staniford Systems.  All Rights Reserved.  September 2022 -

#ifndef CLIMATE_DATABASE_H
#define CLIMATE_DATABASE_H


// =======================================================================================
// Forward declarations

class GHCNDatabase;
class HttpServThread;


// =======================================================================================
/// @brief Provides climate information within permaserv.
///
/// This class serves, for any given location, information about the current/recent
/// climate, as well as projections of future changed climate.

class ClimateDatabase
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  ClimateDatabase(void);
  ~ClimateDatabase(void);
  unsigned printClimateJson(char* buf, unsigned bufSize, 
                                            float lat, float longt, unsigned yearCount);
  bool printStationDiagnosticTable(HttpServThread* serv, 
                                            float lat, float longt, unsigned yearCount);
  bool processStationDiagnosticRequest(HttpServThread* serv, char* stationId);
  
private:
  
  // Instance variables - private
  GHCNDatabase* ghcnDatabase;
  
  // Member functions - private
  /// @brief Prevent copy-construction.
  ClimateDatabase(const ClimateDatabase&);       
  /// @brief Prevent assignment.
  ClimateDatabase& operator=(const ClimateDatabase&);      
};


// =======================================================================================

#endif




