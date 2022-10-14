// Copyright Staniford Systems.  All Rights Reserved.  September 2022 -

#ifndef CLIMATE_INFO_H
#define CLIMATE_INFO_H

#include "DynamicallyTypable.h"


// =======================================================================================
// Forward declarations

class GHCNDatabase;
class HttpServThread;


// =======================================================================================
// Useful macros

#define forAllDays(i, j)  int loopDays; for(int i=0; i < endYear-startYear; \
                                  i++, loopDays = DaysInYear(startYear+i)) \
                                    for(int j=0; j < loopDays; j++)


// =======================================================================================
// Flags for storage in a ClimateDay structure.

#define LOW_TEMP_VALID  0x00000001
#define HI_TEMP_VALID   0x00000002
#define PRECIP_VALID    0x00000004
#define ALL_OBS_VALID   0x00000007


// =======================================================================================
/// @brief One day's worth of climate data.

class ClimateDay
{
  public:

  // Member functions - public
  int writeJson(char* buf, unsigned bufSize);
  
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

class ClimateInfo: public DynamicallyTypable
{
  friend GHCNDatabase;
  
public:
  
  // Instance variables - public
  int startYear;        // the first year for which we have data
  int endYear;         // the year *after* the last year of data
  
  // Member functions - public
  ClimateInfo(int start, int end);
  ~ClimateInfo(void);
  void countValidDays(unsigned& totalDays, unsigned& validDays);
  bool diagnosticHTML(HttpServThread* serv);
  virtual DynamicType getDynamicType(void) {return TypeClimateInfo;}
  virtual int writeJsonFields(char* buf, unsigned bufSize);

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




