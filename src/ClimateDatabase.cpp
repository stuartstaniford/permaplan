// Copyright Staniford Systems.  All Rights Reserved.  September 2022 -
// This class provides climate information within permaserv.  It serves, for any given 
// location, information about the current/recent climate, as well as projections of 
// future changed climate

#include "ClimateDatabase.h"
#include "GHCNDatabase.h"
#include "HttpServThread.h"
#include "loadFileToBuf.h"
#include "Logging.h"
#include "ClimateInfo.h"


// =======================================================================================
// Static variables, etc

char* ghcnPath = (char*)"Materials/Climate/GHCN";


// =======================================================================================
/// @brief Constructor

ClimateDatabase::ClimateDatabase(void)
{
  ghcnDatabase = new GHCNDatabase(ghcnPath);
}


// =======================================================================================
/// @brief Destructor

ClimateDatabase::~ClimateDatabase(void)
{
}


/// =======================================================================================
/// @brief Output JSON climate data for a particular location to a buffer.
///
/// Note the current model is we output everything in a big lump on a single request.
/// There might be an argument for sending it in smaller lumps so the client can get
/// started without waiting for everything, but that would require a more stateful server.
/// 
/// @returns The number of bytes written to the buffer.  If greater than or equal to 
/// the supplied bufSize parameter, it indicates the buffer was not big enough and the
/// output will have been truncated/incomplete.
/// @param buf The char buffer to write the JSON to.
/// @param bufSize The size of the buffer, which must not be overwritten after the end.
/// @param lat The latitude selected.
/// @param longt The longtitude selected.
/// @param yearCount The number of years of climate data to provide (assumed to start in
/// the present year).
/// @todo We currently just return the first remotely viable station result, instead 
/// of properly analyzing them and picking the best or a suitably adjusted admixture
/// @todo Most of this function should probably be in GHCN instead of here.
/// @todo We do not currently check the age of climateInfo data in memory.

unsigned ClimateDatabase::printClimateJson(char* buf, unsigned bufSize, 
                                        float lat, float longt, unsigned yearCount)
{
  ghcnDatabase->getStations(lat, longt);
  
  int N = ghcnDatabase->stationResults.size();
  
  for(int i=0; i<N; i++)
   {
    GHCNStation* station = ghcnDatabase->stationResults[i];
    unless(station->climate)
     {
      ghcnDatabase->checkCSVFile(station);
      ghcnDatabase->readOneCSVFile(station);
     }
    // lame temp hack
   }
  
  return 0u;
}


/// =======================================================================================
/// @brief Output HTML table of stations close to a particular location.
/// 
/// @returns True if all was well writing to the buffer.  If false, it indicates the 
/// buffer was not big enough and the output will have been truncated/incomplete.
/// @param serv A pointer to the HttpServThread managing the HTTP response.
/// @param lat The latitude selected.
/// @param longt The longtitude selected.
/// @param yearCount The number of years of climate data to provide (assumed to start in
/// the present year).

bool ClimateDatabase::printStationDiagnosticTable(HttpServThread* serv, 
                                                float lat, float longt, unsigned yearCount)
{
  // Find the relevant stations
  ghcnDatabase->getStations(lat, longt);
  int N = ghcnDatabase->stationResults.size();
  
  // Start the HTML page and the table header
  unless(serv->startResponsePage("Climate Station Diagnostics"))
    return false;
  unless(serv->startTable((char*)"Stations"))
    return false;
  httPrintf("<tr><th>Station id</th><th>Name</th><th>Location</th><th>Elevation</th>"
            "<th>Size</th><th>Days</th><th>Valid Days</th></tr>\n");

  // Loop over the rows
  for(int i=0; i<N; i++)
   {
    GHCNStation* station = ghcnDatabase->stationResults[i];
    unless(station->climate)
     {
      ghcnDatabase->checkCSVFile(station);
      ghcnDatabase->readOneCSVFile(station);
     }
    unsigned total, valid;
    station->climate->countValidDays(total, valid);
    httPrintf("<tr><td><a href=\"/climateStation/%s\">%s</a></td><td>%s</td>", 
                                        station->id,  station->id, station->name);
    httPrintf("<td>%.3f, %.3f</td><td>%.0f</td>", 
                            station->latLong[0], station->latLong[1], station->elevation);
    httPrintf("<td>%d</td><td>%d</td><td>%d</td></tr>\n", 
                                                    station->fileBufSize, total, valid); 
   }
  
  // Finish up the table and the page
  httPrintf("</table></center>\n");
  unless(serv->endResponsePage())
    return false;

  return true;
}


/// =======================================================================================
/// @brief Output HTML table detail data for a particular climate station.
/// 
/// @returns True if all was well writing to the buffer.  If false, it indicates the 
/// buffer was not big enough and the output will have been truncated/incomplete.
/// @param serv A pointer to the HttpServThread managing the HTTP response.
/// @param stationId A string hopefully indicating a station ID.  At this point in 
/// processing, it's known to be 11 characters in length, but otherwise could be hostile.

bool ClimateDatabase::processStationDiagnosticRequest(HttpServThread* serv, char* stationId)
{
  // Find the relevant stations
  unless(ghcnDatabase->stationsByName.count(stationId))
   {
    LogClimateDbErr("Couldn't find station for %s in processStationDiagnosticRequest.\n",
                      stationId);
    return false;
   }
  
  GHCNStation* station = ghcnDatabase->stationsByName[std::string(stationId)];
  unless(station->climate)
    return serv->errorPage("No climate information for station.");

  // Start the HTML page and the table header
  char title[128];
  snprintf(title, 128, "Climate Station Details for %s", stationId);
  unless(serv->startResponsePage(title))
    return false;
  
  // Secondary header
  httPrintf("<center><h2>%s (Location: %.3f, %.3f; El: %.0fm)</h2></center>", 
            station->name, station->latLong[0], station->latLong[1], station->elevation);
  
  // Main table of the climate info
  unless(station->climate->diagnosticHTML(serv))
    return false;
  
  // Finish up the page
  unless(serv->endResponsePage())
    return false;

  return true;
}


/// =======================================================================================
/// @brief Beginning of a table of some kind of climate data with station columns.
///
/// @returns True if all was well writing to the buffer.  If false, it indicates the
/// buffer was not big enough and the output will have been truncated/incomplete.
/// @param serv A pointer to the HttpServThread managing the HTTP response.
/// @param relevantStations A refence to the vector of GHCNStation* to make columns of.
/// @param latLong A float* to at least two float values for lat and long.

bool ClimateDatabase::stationTableHeader(HttpServThread* serv, float* latLong,
                            std::vector<GHCNStation*>& relevantStations, bool* skipStations)
{
  int N = relevantStations.size();
  
  // Start the table
  unless(serv->startTable((char*)"Stations"))
    return false;
  
  // Header row with station ids
  httPrintf("<tr><th>Id.</th>");
  for(int i=0; i<N; i++)
   {
    if(skipStations && skipStations[i])
      continue;
    httPrintf("<th>%s</th>", relevantStations[i]->id);
   }
  httPrintf("</tr>");

  // Row of location offset data
  httPrintf("<tr><td>Offset (deg)</td>")
  for(int i=0; i<N; i++)
   {
    if(skipStations && skipStations[i])
      continue;
    httPrintf("<td>%.3f, %.3f</td>", relevantStations[i]->latLong[0] - latLong[0],
                                          relevantStations[i]->latLong[1] - latLong[1]);
   }
  httPrintf("</tr>");

  // Row of Elevation Data
  httPrintf("<tr><td>El. (m)</td>")
  for(int i=0; i<N; i++)
   {
    if(skipStations && skipStations[i])
      continue;
    httPrintf("<td>%.0f</td>", relevantStations[i]->elevation);
   }
  httPrintf("</tr>");

  // Row of station names
  httPrintf("<tr><td>Name</td>")
  for(int i=0; i<N; i++)
   {
    if(skipStations && skipStations[i])
      continue;
    httPrintf("<td>%s</td>", relevantStations[i]->name);
   }
  httPrintf("</tr>");
  
  return true;
}


/// =======================================================================================
/// @brief Output HTML table of min and max temperatures over the year for different
/// stations near some location.
///
/// @returns True if all was well writing to the buffer.  If false, it indicates the
/// buffer was not big enough and the output will have been truncated/incomplete.
/// @param serv A pointer to the HttpServThread managing the HTTP response.
/// @param url A string hopefully indicating the latitude, longtitude, and year.  At
/// this point in processing, it could be hostile.

bool ClimateDatabase::processStationComparisonRequest(HttpServThread* serv, char* url)
{
  // Process the URL and extract and check parameters
  float latLong[2]; // (lat, long)
  unless(extractColonVecN(url, 2, latLong))
   {
    LogRequestErrors("Bad station comparison request: stationComp?%s\n", url);
    return false;
   }
  unless(checkLatLong(latLong))
   {
    LogRequestErrors("Bad lat long to station comparison request: stationComp?%s\n", url);
    return false;
   }
  
  // Start the HTML page
  char title[128];
  snprintf(title, 128, "Comparing min/max temps for stations near %.3f, %.3f",
                                                                  latLong[0], latLong[1]);
  unless(serv->startResponsePage(title))
    return false;

  // Find the relevant stations
  std::vector<GHCNStation*> relevantStations;
  std::vector<unsigned> indices;
  ghcnDatabase->searchStations(latLong[0], latLong[1], relevantStations, indices,
                                                          HI_TEMP_VALID|LOW_TEMP_VALID, 0u);
 
  // Gather the data for the table.
  int N = relevantStations.size();
  std::vector<int>*   years[N];
  std::vector<float>* diffs[N];
  int yearIndices[N];
  bool skipStations[N];
  for(int i=0; i<N; i++)
   {
    yearIndices[i] = 0;
    years[i] = new std::vector<int>;
    diffs[i] = new std::vector<float>;
    if(relevantStations[0]->climate->diffObservable(relevantStations[i]->climate,
                      *(years[i]), *(diffs[i]), HI_TEMP_VALID, offsetof(ClimateDay, hiTemp)))
      skipStations[i] = false;
    else
      skipStations[i] = true;
   }
  
  // Table header
  unless(stationTableHeader(serv, latLong, relevantStations, skipStations))
    return false;

  // Lay out the main table of yearly rows
  int rows = relevantStations[0]->climate->endYear - relevantStations[0]->climate->startYear;
  for(int j=0; j < rows; j++)
   {
    int thisYear = j + relevantStations[0]->climate->startYear;
    httPrintf("<tr><td>%d</td>", thisYear);
    for(int i=0; i<N; i++)
     {
      if(skipStations[i])
        continue;
      fprintf(stderr, "i: %d, Comparing %d to %d.\n", i, (*(years[i]))[yearIndices[i]], thisYear);
      while((*(years[i]))[yearIndices[i]] < thisYear)
        yearIndices[i]++;
      
      if((*(years[i]))[yearIndices[i]] > thisYear)
       {
        httPrintf("<td></td>");
       }
      else
       {
        httPrintf("<td>%.3f</td>", (*(diffs[i]))[yearIndices[i]]);
       }
     }
    httPrintf("</tr>");
   }
  
  // Finish up the table and the page
  httPrintf("</table>")
  unless(serv->endResponsePage())
    return false;

  return true;
}


/// =======================================================================================
/// @brief Output HTML table of available curves of some climate observable near
/// a location.
/// 
/// @returns True if all was well writing to the buffer.  If false, it indicates the 
/// buffer was not big enough and the output will have been truncated/incomplete.
/// @param serv A pointer to the HttpServThread managing the HTTP response.
/// @param url A string hopefully indicating the latitude, longtitude, and year.  At 
/// this point in processing, it could be hostile.
/// @param urlStub A string pointing to the part of the URL before this function, needed
/// for logging (because multiple urls can call this function.
/// @param andFlagMask An unsigned mask to check which flags we are searching for in
/// ClimateYears and ClimateDays.
/// @param obsOffset  The offset within a ClimateDay of the float to use in populating
/// the table.
/// @param titleObsName The name of the variable for use in page title.

bool ClimateDatabase::processObservationCurvesRequest(HttpServThread* serv, char* url,
                                  char* urlStub, unsigned andFlagMask, unsigned obsOffset,
                                  char* titleObsName)
{
  // Process the URL and extract and check parameters
  float latLongYear[3]; // (lat, long, year) 
  unless(extractColonVecN(url, 3, latLongYear))
   {
    LogRequestErrors("Bad %s curve request: %s?%s\n", titleObsName, urlStub, url);
    return false;
   }
  unless(checkLatLong(latLongYear))
   {
    LogRequestErrors("Bad lat long to %s curve request: %s?%s\n",
                                                              titleObsName, urlStub, url);
    return false;
   }
  int year = (int)latLongYear[2];
  unless(year >= 1800 && year < 3000)
   {
    LogRequestErrors("Out of range year %d in %s curve request: %s?%s\n",
                                                        year, titleObsName, urlStub, url);
    return false;
   }
  
  // Start the HTML page
  char title[128];
  snprintf(title, 128, "Available %s Curves for %d near %.3f, %.3f",
                                      titleObsName, year, latLongYear[0], latLongYear[1]);
  unless(serv->startResponsePage(title))
    return false;

  // Find the relevant stations
  std::vector<GHCNStation*> relevantStations;
  std::vector<unsigned> indices;
  ghcnDatabase->searchStations(latLongYear[0], latLongYear[1], 
                                            relevantStations, indices, andFlagMask, year);

  // Table header
  unless(stationTableHeader(serv, latLongYear, relevantStations))
    return false;
  
  // Main table of the temperature info
  int N = relevantStations.size();
  int days = DaysInYear(year);
  for(int j=0; j<days; j++)
   {
    httPrintf("<tr><td>%d</td>", j)
    for(int s=0; s<N; s++)
     {
      ClimateInfo* clim = relevantStations[s]->climate;
      if(clim->climateYears[indices[s]]->climateDays[j].flags & andFlagMask)
       {
        httPrintf("<td>%.2f</td>",
                *((float*)((char*)(&(clim->climateYears[indices[s]]->climateDays[j])) + obsOffset)));
       }
      else
       {
        httPrintf("<td></td>");          
       }
     }
    httPrintf("</tr>")    
   }
  
  // Finish up the table and the page
  httPrintf("</table>")
  unless(serv->endResponsePage())
    return false;

  return true;
}


// =======================================================================================
