// Copyright Staniford Systems.  All Rights Reserved.  September 2022 -
// Models a climate scenario at some particular location.  This contains a series 
// of daily observations/projections of the min and max temperatures and 
// precipitation data that is intended to be weather data for a particular location.
// This is the unit of data obtained by permaplan from permaserv, and then used in 
// projections within permaplan

#include "ClimateInfo.h"
#include <assert.h>


// =======================================================================================
/// @brief Constructor
///
/// @param start The integer year of the first year of data
/// @param end The integer year after that last year of data

ClimateInfo::ClimateInfo(int start, int end): startYear(start),
                                              endYear(end)
{
  assert(endYear > startYear);
  climateYears = new ClimateYear[endYear - startYear];
}


// =======================================================================================
/// @brief Destructor

ClimateInfo::~ClimateInfo(void)
{
  delete[] climateYears;
}


// =======================================================================================
