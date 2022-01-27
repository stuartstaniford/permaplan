// Copyright Staniford Systems.  All Rights Reserved.  January 2022 -

#ifndef GDAL_FILE_INTERFACE_H
#define GDAL_FILE_INTERFACE_H

#include "Lockable.h"


// =======================================================================================
// Forward declarations

class GDALDataset;


// =======================================================================================
/// @brief Interface to geospatial data files via the GDAL library.
///
/// Permaplan uses libgdal to access files of geospatial data (eg geotiff files).  GDAL
/// is very complex and powerful, and this class provides a simplified interface which
/// just abstracts the limited amount of functionality we actually require.


class GdalFileInterface: public Lockable
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  GdalFileInterface(char* fileName);
  ~GdalFileInterface(void);
  bool getValueAtLocation(int band, float latitude, float longtitude, float& retVal);
  void printOverviewData();
  
private:
  
  // Instance variables - private
  GDALDataset*  dataset;
  char*         srcFileName;
  double        geoTransform[6];   

  // Member functions - private
  /// @brief Prevent copy-construction.
  GdalFileInterface(const GdalFileInterface&);       
  /// @brief Prevent assignment.
  GdalFileInterface& operator=(const GdalFileInterface&);      
};


// =======================================================================================

#endif




