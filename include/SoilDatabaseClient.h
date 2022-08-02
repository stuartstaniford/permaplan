// Copyright Staniford Systems.  All Rights Reserved.  August 2022 -

#ifndef SOIL_DATABASE_CLIENT_H
#define SOIL_DATABASE_CLIENT_H

#include <vector>


// =======================================================================================
// Forward declarations

class HttpPermaservClient;
class SoilProfile;


// =======================================================================================
/// @brief Client side access to SoilDatabase.
///
/// A class to manage the permaplan client side process of getting soil profiles
/// from the SoilDatabase in permaserv, and making them available for plant growth
/// algorithms as needed.

class SoilDatabaseClient
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  SoilDatabaseClient(HttpPermaservClient& httpPermCli);
  ~SoilDatabaseClient(void);
  SoilProfile* getSoil(float latitude, float longtitude);
  
private:
  
  // Instance variables - private
  HttpPermaservClient& httpPermClient;
  std::vector<SoilProfile*> soilSamples;
  
  // Member functions - private
  /// @brief Prevent copy-construction.
  SoilDatabaseClient(const SoilDatabaseClient&);       
  /// @brief Prevent assignment.
  SoilDatabaseClient& operator=(const SoilDatabaseClient&);      
};


// =======================================================================================

#endif




