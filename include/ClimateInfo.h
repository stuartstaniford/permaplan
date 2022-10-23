// Copyright Staniford Systems.  All Rights Reserved.  September 2022 -

#ifndef CLIMATE_INFO_H
#define CLIMATE_INFO_H

#include "DynamicallyTypable.h"


// =======================================================================================
// Forward declarations

class GHCNDatabase;
class HttpServThread;
class ClimateDatabase;


// =======================================================================================
// Important constants

// Maximum number of days in a year missing before the year is considered invalid
#define MISSING_DAYS_ALLOWED 28

// Min and max temperature range considered valid.  Values outside this range
// are marked invalid.  These limits are in degrees Celsius.
// https://en.wikipedia.org/wiki/Highest_temperature_recorded_on_Earth
// https://en.wikipedia.org/wiki/Lowest_temperature_recorded_on_Earth
#define MAX_TEMP_VALID 70.0f
#define MIN_TEMP_VALID -110.0f

// Max precipitation considered valid (in mm)
// https://wmo.asu.edu/content/world-greatest-twenty-four-hour-1-day-rainfall
#define MIN_PRECIP  0.0f
#define MAX_PRECIP  3000.0f

// =======================================================================================
// Flags for storage in a ClimateDay structure.

#define LOW_TEMP_VALID  0x00000001
#define HI_TEMP_VALID   0x00000002
#define PRECIP_VALID    0x00000004
#define ALL_OBS_VALID   0x00000007


// =======================================================================================
// Useful macros

#define forAllDays(i, j)  int loopDays; for(int i=0; i < nYears; i++, \
                              loopDays = DaysInYear(climateYears[(i<nYears?i:nYears-1)]->year)) \
                                    for(int j=0; j < loopDays; j++)

#define tempInRange(T) (((T) >= MIN_TEMP_VALID) && ((T) <= MAX_TEMP_VALID))
#define precipInRange(P) (((P) >= MIN_PRECIP) && ((P) <= MAX_PRECIP))


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


// =======================================================================================
/// @brief ClimateYear is an array of ClimateDays - always has space for a leap day.

class ClimateYear
{

  public:
  
  // Instance variables - public
  int         year;
  unsigned    flags;
  ClimateDay  climateDays[366];

  // Member functions - public
  ClimateYear(int inYear);
  bool assessValidity(void);
  bool diffHighTemp(ClimateYear* otherYear, float& difference);
};


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
  friend ClimateDatabase;
  
public:
  
  // Instance variables - public
  int startYear;        // the first year for which we have data
  int endYear;          // the year *after* the last year of data
  int nYears;           // the number of years of actually valid data in climateYears 
  
  // Member functions - public
  ClimateInfo(int start, int end);
  ~ClimateInfo(void);
  void countValidDays(unsigned& totalDays, unsigned& validDays);
  bool diagnosticHTML(HttpServThread* serv);
  virtual DynamicType getDynamicType(void) {return TypeClimateInfo;}
  virtual int writeJsonFields(char* buf, unsigned bufSize);

private:
  
  // Instance variables - private
  ClimateYear** climateYears;
  
  // Member functions - private
  /// @brief Prevent copy-construction.
  ClimateInfo(const ClimateInfo&);       
  /// @brief Prevent assignment.
  ClimateInfo& operator=(const ClimateInfo&);      
};


// =======================================================================================

#endif




