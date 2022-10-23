// Copyright Staniford Systems.  All Rights Reserved.  September 2022 -
// Models a climate scenario at some particular location.  This contains a series 
// of daily observations/projections of the min and max temperatures and 
// precipitation data that is intended to be weather data for a particular location.
// This is the unit of data obtained by permaplan from permaserv, and then used in 
// projections within permaplan

#include "ClimateInfo.h"
#include "Global.h"
#include "HttpServThread.h"
#include <assert.h>
#include <gsl/gsl_sf_bessel.h>


// =======================================================================================
/// @brief Constructor
///
/// @param start The integer year of the first year of data
/// @param end The integer year after that last year of data

ClimateInfo::ClimateInfo(int start, int end): startYear(start),
                                              endYear(end)
{
  assert(endYear > startYear);
  climateYears  = new ClimateYear*[endYear - startYear];
  nYears        = 0;
}


// =======================================================================================
/// @brief Destructor

ClimateInfo::~ClimateInfo(void)
{
  delete[] climateYears;
}


// =======================================================================================
/// @brief Climate Year constructor
/// 
/// @param year The year (A.D.) for which this records the climate 

ClimateYear::ClimateYear(int inYear):year(inYear), flags(0u)
{
  
}


// =======================================================================================
/// @brief Assess whether this year has enough valid data to be worth keeping around
/// 
/// Will set the flags field to indicate which types of data are valid for the year as a
/// whole.
/// @returns True if there is enough valid data of at least one kind, false if it's dross.

bool ClimateYear::assessValidity(void)
{
  int days      = DaysInYear(year);
  int hiTCount  = 0;
  int loTCount  = 0;
  int prcpCount = 0;
  
  for(int j=0; j<days; j++)
   {
    unsigned dayFlags = climateDays[j].flags;
    if(dayFlags & LOW_TEMP_VALID)
      loTCount++;
    if(dayFlags & HI_TEMP_VALID)
      hiTCount++;
    if(dayFlags & PRECIP_VALID)
      prcpCount++;
   }
  
  if(days - loTCount <= MISSING_DAYS_ALLOWED)
    flags |= LOW_TEMP_VALID;
  if(days - hiTCount <= MISSING_DAYS_ALLOWED)
    flags |= HI_TEMP_VALID;
  if(days - prcpCount <= MISSING_DAYS_ALLOWED)
    flags |= PRECIP_VALID;
 
  return (flags & ALL_OBS_VALID);
}


// =======================================================================================
/// @brief Compute the average difference in high temp of this year and another year
///
/// @returns true if there is enough data for a comparison, false otherwise.
/// @param difference A reference to a float to store the average difference.
/// @param years A vector of the years found with valid comparison
/// @param diffs A vector of the differences matching those years.
/// @param andFlagMask The quantity to and with the flags to determine if a given year/day
/// is valid.
/// @param obsOffset The offset in bytes of the observable being computed in a ClimateDay
/// structure.

bool ClimateInfo::diffObservable(ClimateInfo* otherInfo, std::vector<int>& years,
                      std::vector<float>& diffs, unsigned andFlagMask, unsigned obsOffset)
{
  
  years.clear();
  diffs.clear();
  
  // loop over the years in this ClimateInfo
  int j = 0;
  for(int i=0; i<nYears; i++)
   {
    // Make sure this year is valid for us
    unless(climateYears[i]->flags & andFlagMask)
      continue;
    int year = climateYears[i]->year;
    
    // Try to find a matching year on the other side
    while(j < otherInfo->nYears && otherInfo->climateYears[j]->year < year)
      j++;
     
    // End the search if we've run out of years on the other side
    if(j == otherInfo->nYears)
      break;
      
    // Skip this year if the other side doesn't have a matching year
    if(otherInfo->climateYears[j]->year > year)
      continue;
      
    // Skip this year if the other side's year is not valid for our purposes
    unless(otherInfo->climateYears[j]->flags & andFlagMask)
      continue;
   
    //compute the difference
    float difference;
    unless(climateYears[i]->diffObservable(otherInfo->climateYears[j], difference,
                                                                  andFlagMask, obsOffset))
      continue;
    
    // if we get here, we have two matching valid years, and a valid comparison, so
    // record the fact
    years.push_back(year);
    diffs.push_back(difference);
   }
   
  if(years.size())
    return true;
  else
    return false;
}


// =======================================================================================
/// @brief Compute the average difference in some observable of this year and another year
///
/// @returns true if there is enough data for a comparison, false otherwise.
/// @param difference A reference to a float to store the average difference.
/// @param andFlagMask The quantity to and with the flags to determine if a given year/day
/// is valid.
/// @param obsOffset The offset in bytes of the observable being computed in a ClimateDay
/// structure.

bool ClimateYear::diffObservable(ClimateYear* otherYear, float& difference,
                                                  unsigned andFlagMask, unsigned obsOffset)
{
  // Go home early unless both years have enough valid data
  unless(flags & andFlagMask)
    return false;
  unless(otherYear->flags & andFlagMask)
    return false;

  // Figure out number of days
  int days1     = DaysInYear(year);
  int days2     = DaysInYear(otherYear->year);
  int days      = days1<days2?days1:days2;

  // Main loop over the days
  difference = 0.0f;
  int count = 0;
  for(int j=0; j<days; j++)
   {
    // Validity tests
    unsigned dayFlags = climateDays[j].flags;
    unless(dayFlags & andFlagMask)
      continue;
    dayFlags = otherYear->climateDays[j].flags;
    unless(dayFlags & andFlagMask)
      continue;

    // update the counts/totals
    difference += *((float*)((char*)(climateDays + j) + obsOffset)) -
                              *((float*)((char*)(otherYear->climateDays + j) + obsOffset));
    count++;
   }
   
  return difference/count;
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
    unsigned flags = climateYears[i]->climateDays[j].flags;
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

    buf += DynamicallyTypable::writeJsonFields(buf, bufSize);
    bufprintf(",\n");

    // Write our own fields.
    bufprintf("\"startYear\": %d,\n", startYear);
    bufprintf("\"endYear\": %d,\n", endYear);
   bufprintf("\"nYears\": %d,\n", nYears);

    // Write the arrays of climate data
    bufprintf("[\n");
    for(int i = 0; i < nYears; i++)
     {
      if(i)
        bufprintf(",\n");
      
      /// @todo Need to turn the year into an object
      
      bufprintf("[\n");
      int days = DaysInYear(i+startYear);
      for(int j = 0; j < days; j++)
       {
        if(j)
          bufprintf(",\n");
        buf += climateYears[i]->climateDays[j].writeJson(buf, bufSize - (end-buf));  
       }
      
      bufprintf("\n]"); // no ,\n as we don't know we are last        
     }
   
    bufprintf("\n]"); // no ,\n as we don't know we are last
    return bufSize - (end-buf);
  }


// =======================================================================================
/// @brief Output JSON format of climate data for a single day to a buffer.
/// 
/// @returns The number of bytes written to the buffer.  If greater than or equal to 
/// the supplied bufSize parameter, it indicates the buffer was not big enough and the
/// output will have been truncated/incomplete.
/// @param buf The char buffer to write the JSON to.
/// @param bufSize The size of the buffer, which must not be overwritten after the end.

  int ClimateDay::writeJson(char* buf, unsigned bufSize)
  {
    char* end = buf + bufSize;

    bufprintf("{\n");

    if(flags & LOW_TEMP_VALID)
      bufprintf("\"lowTemp\": %.2f,\n", lowTemp);
    if(flags & HI_TEMP_VALID)
      bufprintf("\"hiTemp\": %.2f,\n", hiTemp);
   if(flags & PRECIP_VALID)
     bufprintf("\"precip\": %.1f\n", precip); // no comma as last
   
    bufprintf("}"); // no ,\n as we don't know we are last
    return bufSize - (end-buf);
  }


/// =======================================================================================
/// @brief Output HTML table detail data for diagnostic HTTP server purposes.
/// 
/// @returns True if all was well writing to the buffer.  If false, it indicates the 
/// buffer was not big enough and the output will have been truncated/incomplete.
/// @param serv A pointer to the HttpServThread managing the HTTP response.

bool ClimateInfo::diagnosticHTML(HttpServThread* serv)
{
  // Open the table
  unless(serv->startTable((char*)"Stations"))
    return false;

  // Header row
  httPrintf("<tr><th>Day</th>");
  for(int i=0; i < nYears; i++)
   {
    httPrintf("<th>%d</th>", climateYears[i]->year);
   }
  httPrintf("</tr>\n");
  
  // Loop over per-day rows
  for(int j=0; j < 366; j++)
   {
    httPrintf("<tr><td>%d</td>", j);
    for(int i=0; i < nYears; i++)
     {
      httPrintf("<td>");
      ClimateDay* today = climateYears[i]->climateDays + j;
      if(today->flags & HI_TEMP_VALID)
        httPrintf("<span style=\"color:red\">T+:%.1f</span>", today->hiTemp);
      if(today->flags & LOW_TEMP_VALID)
        httPrintf("<span style=\"color:green\">T-:%.1f</span>", today->lowTemp);
      if(today->flags & PRECIP_VALID)
        httPrintf("<span style=\"color:blue\">P:%.1f</span>", today->precip);

      httPrintf("</td>");
     }

    httPrintf("</tr>\n");    
   }
  
  // End table
  httPrintf("</table></center>\n");

  return true;  
}


// =======================================================================================
