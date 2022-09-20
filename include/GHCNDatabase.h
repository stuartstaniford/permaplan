// Copyright Staniford Systems.  All Rights Reserved.  September 2022 -

#ifndef GHCN_DATABASE_H
#define GHCN_DATABASE_H


// =======================================================================================
/// @brief Interface to the Global Historical Climatology Network - Daily data.
///
/// This provides global harmonized data for recent daily climate from all over the 
/// world.  The purpose of this class is to provide the data in a usable form to the
/// rest of permaserv.  For more on the dataset:
/// https://www.ncei.noaa.gov/metadata/geoportal/rest/metadata/item/gov.noaa.ncdc:C00861/html.

class GHCNDatabase
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  GHCNDatabase(void);
  ~GHCNDatabase(void);
  
private:
  
  // Instance variables - private
  
  // Member functions - private
  /// @brief Prevent copy-construction.
  GHCNDatabase(const GHCNDatabase&);       
  /// @brief Prevent assignment.
  GHCNDatabase& operator=(const GHCNDatabase&);      
};


// =======================================================================================

#endif




