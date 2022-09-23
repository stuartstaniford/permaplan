// Copyright Staniford Systems.  All Rights Reserved.  September 2022 -

#ifndef CLIMATE_INFO_H
#define CLIMATE_INFO_H


// =======================================================================================
/// @brief One day's worth of climate data.

class ClimateDay
{
  public:

  // Member functions - public

  // Instance variables - public
  float lowTemp;    // the low temperature (generally around sunrise).
  float hiTemp;     // high temperature (generally mid afternoon).
  float precip;     // mm of water falling during the 24 hour period
};


// =======================================================================================
/// @brief Models a climate scenario at some particular location.
///
/// This contains a series of daily observations/projections of the min and max 
/// temperatures and precipitation data that is intended to be weather data for a 
/// particular location.  This is the unit of data obtained by permaplan from
/// permaserv, and then used in projections within permaplan.

class ClimateInfo
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  ClimateInfo(void);
  ~ClimateInfo(void);
  
private:
  
  // Instance variables - private
  
  // Member functions - private
  /// @brief Prevent copy-construction.
  ClimateInfo(const ClimateInfo&);       
  /// @brief Prevent assignment.
  ClimateInfo& operator=(const ClimateInfo&);      
};


// =======================================================================================

#endif




