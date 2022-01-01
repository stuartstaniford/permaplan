// Copyright Staniford Systems.  All Rights Reserved.  Jun 2021 -

#ifndef SOLAR_DATABASE_H
#define SOLAR_DATABASE_H


// =======================================================================================
/// @brief Interface to solar databases.
///  
/// This class provides an interface to databases of information required to do solar
/// irradiance calculations.  Currently, it uses libgeotiff to read information from
/// the raster files downloaded from 
///https://datacatalog.worldbank.org/dataset/world-direct-normal-irradiation-dni-gis-data-global-solar-atlas
/// @todo This is a stub with no implementation yet.

class SolarDatabase
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  SolarDatabase(void);
  ~SolarDatabase(void);
  
private:
  
  // Instance variables - private
  
  // Member functions - private
  void getDifData(void);
  
  SolarDatabase(const SolarDatabase&);                 // Prevent copy-construction
  SolarDatabase& operator=(const SolarDatabase&);      // Prevent assignment

};


// =======================================================================================

#endif




