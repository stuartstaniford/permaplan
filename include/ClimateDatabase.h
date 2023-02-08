// Copyright Staniford Systems.  All Rights Reserved.  September 2022 -

#ifndef CLIMATE_DATABASE_H
#define CLIMATE_DATABASE_H

#include <vector>


// =======================================================================================
// Forward declarations

class GHCNDatabase;
class GHCNStation;
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
  ClimateDatabase(float fileSpacing = -1.0f);
  ~ClimateDatabase(void);
  bool indexPageTable(HttpServThread* serv);
  unsigned printClimateJson(char* buf, unsigned bufSize, 
                                            float lat, float longt, unsigned yearCount);
  bool printStationDiagnosticTable(HttpServThread* serv, 
                                            float lat, float longt, unsigned yearCount);
  bool processStationDiagnosticRequest(HttpServThread* serv, char* stationId);
  bool processStationComparisonRequest(HttpServThread* serv, char* url, char* urlStub, 
                                       unsigned andMask, unsigned offset);
  bool processObservationCurvesRequest(HttpServThread* serv, char* url, char* urlStub,
                            unsigned andFlagMask, unsigned obsOffset, char* titleObsName);
  
private:
  
  // Instance variables - private
  GHCNDatabase* ghcnDatabase;
  
  // Member functions - private
  bool stationTableHeader(HttpServThread* serv, float* latLong,
                    std::vector<GHCNStation*>& relevantStations, std::vector<bool>& skipStations);

  /// @brief Prevent copy-construction.
  ClimateDatabase(const ClimateDatabase&);       
  /// @brief Prevent assignment.
  ClimateDatabase& operator=(const ClimateDatabase&);      
};


// =======================================================================================

#endif




