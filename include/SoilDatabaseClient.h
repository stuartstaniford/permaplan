// Copyright Staniford Systems.  All Rights Reserved.  August 2022 -

#ifndef SOIL_DATABASE_CLIENT_H
#define SOIL_DATABASE_CLIENT_H


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
  SoilDatabaseClient(void);
  ~SoilDatabaseClient(void);
  
private:
  
  // Instance variables - private
  
  // Member functions - private
  /// @brief Prevent copy-construction.
  SoilDatabaseClient(const SoilDatabaseClient&);       
  /// @brief Prevent assignment.
  SoilDatabaseClient& operator=(const SoilDatabaseClient&);      
};


// =======================================================================================

#endif




