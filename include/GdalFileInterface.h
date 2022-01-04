// Copyright Staniford Systems.  All Rights Reserved.  January 2022 -

#ifndef GDAL_FILE_INTERFACE_H
#define GDAL_FILE_INTERFACE_H


// =======================================================================================
/// @brief Interface to geospatial data files via the GDAL library.
///
/// Permaplan uses libgdal to access files of geospatial data (eg geotiff files).  GDAL
/// is very complex and powerful, and this class provides a simplified interface which
/// just abstracts the limited amount of functionality we actually require.


class GdalFileInterface
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  GdalFileInterface(void);
  ~GdalFileInterface(void);
  
private:
  
  // Instance variables - private
  
  // Member functions - private
  /// @brief Prevent copy-construction.
  GdalFileInterface(const GdalFileInterface&);       
  /// @brief Prevent assignment.
  GdalFileInterface& operator=(const GdalFileInterface&);      
};


// =======================================================================================

#endif




