// Copyright Staniford Systems.  All Rights Reserved.  September 2022 -

#ifndef CLIMATE_DATABASE_CLIENT_H
#define CLIMATE_DATABASE_CLIENT_H


// =======================================================================================
// Forward declarations

class HttpPermaservClient;


// =======================================================================================
/// @brief Fetches climate information from permaserv.
///
/// Gets, for our location, information about the current/recent climate, as well as
/// projections of future changed climate.

class ClimateDatabaseClient
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  ClimateDatabaseClient(HttpPermaservClient& httpPermCli);
  ~ClimateDatabaseClient(void);
  bool getClimateDataFromDatabase(void);

private:
  
  // Instance variables - private
  HttpPermaservClient& httpPermClient;

  // Member functions - private
  /// @brief Prevent copy-construction.
  ClimateDatabaseClient(const ClimateDatabaseClient&);       
  /// @brief Prevent assignment.
  ClimateDatabaseClient& operator=(const ClimateDatabaseClient&);      
};


// =======================================================================================

#endif




