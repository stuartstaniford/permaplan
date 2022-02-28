// Copyright Staniford Systems.  All Rights Reserved.  February 2022 -

#ifndef SOIL_DATABASE_H
#define SOIL_DATABASE_H


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
  
private:
  
  // Instance variables - private
  
  // Member functions - private
  /// @brief Prevent copy-construction.
  SoilDatabase(const SoilDatabase&);       
  /// @brief Prevent assignment.
  SoilDatabase& operator=(const SoilDatabase&);      
};


// =======================================================================================

#endif




