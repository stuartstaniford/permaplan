// Copyright Staniford Systems.  All Rights Reserved.  September 2022 -

#ifndef CLIMATE_INFO_H
#define CLIMATE_INFO_H


// =======================================================================================
// Forward declarations

class GHCNDatabase;

// =======================================================================================
// Flags for storage in a ClimateDay structure.

#define LOW_TEMP_VALID  0x00000001
#define HI_TEMP_VALID   0x00000002
#define PRECIP_VALID    0x00000004

// =======================================================================================
/// @brief One day's worth of climate data.

class ClimateDay
{
  public:

  // Member functions - public

  // Instance variables - public
  unsigned flags;
  float lowTemp;    // the low temperature (generally around sunrise).
  float hiTemp;     // high temperature (generally mid afternoon).
  float precip;     // mm of water falling during the 24 hour period
};

/// @brief ClimateYear is an array of ClimateDays - always has space for a leap day.
typedef ClimateDay ClimateYear[366];


// =======================================================================================
/// @brief Models a climate scenario at some particular location.
///
/// This contains a series of daily observations/projections of the min and max 
/// temperatures and precipitation data that is intended to be weather data for a 
/// particular location.  This is the unit of data obtained by permaplan from
/// permaserv, and then used in projections within permaplan.

class ClimateInfo
{
  friend GHCNDatabase;
  
public:
  
  // Instance variables - public
  int startYear;        // the first year for which we have data
  int endYear;         // the year *after* the last year of data
  
  // Member functions - public
  ClimateInfo(int start, int end);
  ~ClimateInfo(void);
  
private:
  
  // Instance variables - private
  ClimateYear* climateYears;
  
  // Member functions - private
  /// @brief Prevent copy-construction.
  ClimateInfo(const ClimateInfo&);       
  /// @brief Prevent assignment.
  ClimateInfo& operator=(const ClimateInfo&);      
};


// =======================================================================================

#endif




