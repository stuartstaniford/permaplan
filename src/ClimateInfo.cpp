// Copyright Staniford Systems.  All Rights Reserved.  September 2022 -
// Models a climate scenario at some particular location.  This contains a series 
// of daily observations/projections of the min and max temperatures and 
// precipitation data that is intended to be weather data for a particular location.
// This is the unit of data obtained by permaplan from permaserv, and then used in 
// projections within permaplan

#include "ClimateInfo.h"
#include "Global.h"
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
/// @brief Count how many days have fully valid info
/// 
/// This counts the number of days that have high and low temps and precipitation data.
/// @param totalDays A reference to a counter for the total days in the year range
/// @param validDays A reference to a counter for the days with valid information

void ClimateInfo::countValidDays(unsigned& totalDays, unsigned& validDays)
{
  totalDays = validDays = 0u;
  
  forAllDays(i,j)
   {
    unsigned flags = climateYears[i][j].flags;
    totalDays++;
    if((flags & ALL_OBS_VALID) == ALL_OBS_VALID)
        validDays++;
   }
}


// =======================================================================================
/// @brief Output JSON format of climate data to a buffer.
/// 
/// @returns The number of bytes written to the buffer.  If greater than or equal to 
/// the supplied bufSize parameter, it indicates the buffer was not big enough and the
/// output will have been truncated/incomplete.
/// @param buf The char buffer to write the JSON to.
/// @param bufSize The size of the buffer, which must not be overwritten after the end.

  int ClimateInfo::writeJsonFields(char* buf, unsigned bufSize)
  {
    char* end = buf + bufSize;

   /*
    buf += DynamicallyTypable::writeJsonFields(buf, bufSize);
    bufprintf(",\n");

    // Write our own fields.
    bufprintf("\"latitude\": %.6f,\n", latitude);
    bufprintf("\"longtitude\": %.6f,\n", longtitude);
   */ 
    bufprintf("]"); // no ,\n as we don't know we are last
    return bufSize - (end-buf);
  }


// =======================================================================================
