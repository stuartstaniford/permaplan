// Copyright Staniford Systems.  All Rights Reserved.  Jun 2021 -

#ifndef SOLAR_DATABASE_H
#define SOLAR_DATABASE_H

#include "GdalFileInterface.h"


// =======================================================================================
// Forward declarations

class HttpServThread;


// =======================================================================================
/// @brief Interface to solar databases.
///  
/// This class provides an interface to databases of information required to do solar
/// irradiance calculations.  Currently, it uses GdalFileInterface to read information 
/// from the raster files downloaded from 
///https://datacatalog.worldbank.org/dataset/world-direct-normal-irradiation-dni-gis-data-global-solar-atlas

class SolarDatabase
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  SolarDatabase(void);
  ~SolarDatabase(void);
  bool indexPageTable(HttpServThread* serv);
  float getDIFValue(float lat, float longt);
  float getDNIValue(float lat, float longt);
  
private:
  
  // Instance variables - private
  GdalFileInterface difFile;
  GdalFileInterface dniFile;
  
  // Member functions - private
  void getDifData(GdalFileInterface* difDataset);
  
  SolarDatabase(const SolarDatabase&);                 // Prevent copy-construction
  SolarDatabase& operator=(const SolarDatabase&);      // Prevent assignment
};


// =======================================================================================

#endif




