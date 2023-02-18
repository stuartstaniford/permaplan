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

ClimateDatabase::ClimateDatabase(float fileSpacing)
{
  ghcnDatabase = new GHCNDatabase(ghcnPath);
  if(fileSpacing >= 0.0f)
    ghcnDatabase->loadAll(fileSpacing);
}


// =======================================================================================
/// @brief Destructor

ClimateDatabase::~ClimateDatabase(void)
{
}


/// =======================================================================================
/// @brief Output HTML table of all climate database API options to main index page.
/// 
/// @returns True if all was well writing to the buffer.  If false, it indicates the 
/// buffer was not big enough and the output will have been truncated/incomplete.
/// @param serv A pointer to the HttpServThread managing the HTTP response.

bool ClimateDatabase::indexPageTable(HttpServThread* serv)
{
  httPrintf("<hr>\n");
  httPrintf("<center>\n");
  httPrintf("<h2>Climate Database Options</h2>\n");
  unless(serv->startTable())
    return false;
  httPrintf("<tr><th>Link</th><th>notes</th></tr>\n");

  // Climate data near a particular point
  httPrintf("<tr><td><a href=\"/climate/climate?42.421:-76.347:20:\">"
                 "/climate/climate?lat:long:years:</a></td>");
  httPrintf("<td>Years of climate information near location.</td></tr>\n");

  // Climate diagnostics near a particular point
  httPrintf("<tr><td><a href=\"/climate/climateDiagnostic?42.421:-76.347:20:\">"
                 "/climate/climateDiagnostic?lat:long:years:</a></td>");
  httPrintf("<td>Climate station diagnostic near location.</td></tr>\n");

  // Temperature max curves for some year near a particular point
  httPrintf("<tr><td><a href=\"/climate/tMaxYear?42.421:-76.347:2005:\">"
                 "/climate/tMaxYear?lat:long:year:</a></td>");
  httPrintf("<td>Available max temperature curves for year near location.</td></tr>\n");

  // Temperature min curves for some year near a particular point
  httPrintf("<tr><td><a href=\"/climate/tMinYear?42.421:-76.347:2005:\">"
                 "/climate/tMinYear?lat:long:year:</a></td>");
  httPrintf("<td>Available min temperature curves for year near location.</td></tr>\n");

  // Station comparison for daily highs near a particular point
  httPrintf("<tr><td><a href=\"/climate/stationCompHigh?42.421:-76.347:\">"
                 "/climate/stationCompHigh?lat:long:</a></td>");
  httPrintf("<td>Compare stations near location.</td></tr>\n");

  // Station comparison for daily lows near a particular point
  httPrintf("<tr><td><a href=\"/climate/stationCompLow?42.421:-76.347:\">"
                 "/climate/stationCompLow?lat:long:</a></td>");
  httPrintf("<td>Compare stations near location.</td></tr>\n");
  
  // End table
  httPrintf("</table></center>\n");

  return true;
}


/// =======================================================================================
/// @brief Gateway to handling all requests coming in under /climate/.  
/// 
/// This just routes to the appropriate private method to handle the different specific
/// cases.
/// 
/// @returns True if all was well writing to the buffer.  If false, it indicates the 
/// buffer was not big enough and the output will have been truncated/incomplete.
/// @param serv A pointer to the HttpServThread managing the HTTP response.
/// @param url A string with the balance of the request url after "/user/".

bool ClimateDatabase::processHttpRequest(HttpServThread* serv, char* url)
{
  int strlenUrl = strlen(url);
  bool retVal = false;
  
  // climateDiagnostic
  if(strlenUrl >= 22 && strncmp(url, "climateDiagnostic?", 18) == 0)
   {
    LogPermaservOpDetails("Processing climate diagnostic request for %s.\n", url+19);
    retVal = processClimateRequest(serv, url+18, true);
   }

  // climateStation
  // http://127.0.0.1:2091/climate/climateStation/US1NYTM0018
  else if( strlenUrl == 26 && strncmp(url, "climateStation/", 15) == 0)
   {
    LogPermaservOpDetails("Processing climate station request for %s.\n", url+15);
    retVal = processStationDiagnosticRequest(serv, url+15);
   }
  
  // climate
  else if( strlenUrl >= 12 && strncmp(url, "climate?", 8) == 0)
   {
    LogPermaservOpDetails("Processing climate request for %s.\n", url+8);
    retVal = processClimateRequest(serv, url+8);
   }  
  
  // stationCompHigh
  else if( strlenUrl >= 26 && strncmp(url, "stationCompHigh?", 16) == 0)
   {
    LogPermaservOpDetails("Processing station high temp comparison query for %s.\n", 
                                                                                url+16);
    retVal = processStationComparisonRequest(serv, url+16, 
                    (char*)"stationCompHigh", HI_TEMP_VALID, offsetof(ClimateDay, hiTemp));
   }

  // stationCompLow
  else if( strlenUrl >= 25 && strncmp(url, "stationCompLow?", 15) == 0)
   {
    LogPermaservOpDetails("Processing station low temp comparison query for %s.\n", 
                                                                                url+15);
    retVal = processStationComparisonRequest(serv, url+15, (char*)"stationCompLow", 
                                            LOW_TEMP_VALID, offsetof(ClimateDay, lowTemp));
   }

  // tMinYear
  else if( strlenUrl >= 19 && strncmp(url, "tMinYear?", 9) == 0)
   {
    LogPermaservOpDetails("Processing temperature min query for %s.\n", url+9);
    retVal = processObservationCurvesRequest(serv, url+9, (char*)"tMinYear", 
                LOW_TEMP_VALID, offsetof(ClimateDay, lowTemp), (char*)"Min. Temperature");
  }

  // tMaxYear
  else if( strlenUrl >= 19 && strncmp(url, "tMaxYear?", 9) == 0)
   {
    LogPermaservOpDetails("Processing temperature max query for %s.\n", url+9);
    retVal = processObservationCurvesRequest(serv, url+9, (char*)"tMaxYear", 
                HI_TEMP_VALID, offsetof(ClimateDay, hiTemp), (char*)"Max. Temperature");
   }

  // Default - failure
  else
   {
    LogRequestErrors("Request for unknown /user/ resource %s\n", url);
    retVal = serv->errorPage("Resource not found");
   }

  return retVal;
}


// =======================================================================================
/// @brief Process the case of a request for climate information.
/// @param url The balance of the URL that we are to deal with (ie after the '?')
/// @param diagnostic A bool that if true provides HTML diagnostic pages rather than 
/// the JSON API.  Used for troubleshooting the climate data and the permaserv code 
/// handling it.
/// @returns True if all went well, false if we couldn't correctly write a good object.

bool ClimateDatabase::processClimateRequest(HttpServThread* serv, char* url, bool diagnostic)
{
  // Extract the information from the URL
  float latLongYear[3];
  unless(extractColonVecN(url, 3, latLongYear))
   {
    if(diagnostic)
     {
      LogRequestErrors("Bad climateDiagnostic request: /climateDiagnostic?%s\n", url);
     }
    else
      LogRequestErrors("Bad climate request: /climate?%s\n", url);
    return false;
   }
  unless(checkLatLong(latLongYear))
   {
    LogRequestErrors("Bad latlong parameters to climate diagnostic request: "
                                                    "/climateDiagnostic?%s\n", url);
    return false;
   }
  int year = (int)latLongYear[2];
  unless(year >= 0 && year < 200)
   {
    LogRequestErrors("Bad year parameter %d to climate diagnostic request: "
                                              "/climateDiagnostic?%s\n", year, url);
    return false;
   }

  if(diagnostic)
   {
    unless(printStationDiagnosticTable(serv, 
                            latLongYear[0], latLongYear[1], (unsigned)latLongYear[2]))
     {
      LogClimateDbErr("Overflow in html response to climate diagnostic request "
                      "/climateDiagnostic?%s.\n", url);
      serv->respBufOverflow = true; 
      return false;
     }
    LogPermaservOps("Serviced climate diagnostic request for %f,%f (%u years) "
                  "from client on port %u.\n", latLongYear[0], latLongYear[1], 
                  (unsigned)latLongYear[2], serv->clientP);
   }
  else
   {
    if( (serv->respPtr += printClimateJson(serv->respPtr, serv->respEnd - serv->respPtr, 
                                  latLongYear[0], latLongYear[1], (unsigned)latLongYear[2]))
                    >= serv->respEnd)
     {
      LogClimateDbErr("Overflow in json response to climate request /climate?%s.\n", url);
      serv->respBufOverflow = true; 
      return false;
     }
    LogPermaservOps("Serviced climate request for %f,%f (%u years) from client on port %u.\n", 
                  latLongYear[0], latLongYear[1], (unsigned)latLongYear[2], serv->clientP);
   }
  return true;
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
  ghcnDatabase->getStations(lat, longt, 10);
  
  int N = ghcnDatabase->stationResults.size();
  
  for(int i=0; i<N; i++)
   {
    GHCNStation* station = ghcnDatabase->stationResults[i];
    unless(station->climate)
     {
      ghcnDatabase->checkOrFetchCSVFile(station);
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
  ghcnDatabase->getStations(lat, longt, 7);
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
      ghcnDatabase->checkOrFetchCSVFile(station);
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
                            std::vector<GHCNStation*>& relevantStations, 
                            std::vector<bool>& skipStations)
{
  int N = relevantStations.size();
  
  // Start the table
  unless(serv->startTable((char*)"Stations"))
    return false;
  
  // Header row with station ids
  httPrintf("<tr><th>Id.</th>");
  for(int i=0; i<N; i++)
   {
    if(skipStations[i])
      continue;
    httPrintf("<th>%s</th>", relevantStations[i]->id);
   }
  httPrintf("</tr>");

  // Row of latitude offset data
  httPrintf("<tr><td>Lat Offset (deg)</td>")
  for(int i=0; i<N; i++)
   {
    if(skipStations[i])
      continue;
    httPrintf("<td>%.3f</td>", relevantStations[i]->latLong[0] - latLong[0]);
   }
  httPrintf("</tr>");

  // Row of longtitude offset data
  httPrintf("<tr><td>Long. Offset (deg)</td>")
  for(int i=0; i<N; i++)
   {
    if(skipStations[i])
      continue;
    httPrintf("<td>%.3f</td>", relevantStations[i]->latLong[1] - latLong[1]);
   }
  httPrintf("</tr>");

  // Row of Elevation Data
  httPrintf("<tr><td>El. (m)</td>")
  for(int i=0; i<N; i++)
   {
    if(skipStations[i])
      continue;
    httPrintf("<td>%.0f</td>", relevantStations[i]->elevation);
   }
  httPrintf("</tr>");

  // Row of station names
  httPrintf("<tr><td>Name</td>")
  for(int i=0; i<N; i++)
   {
    if(skipStations[i])
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

bool ClimateDatabase::processStationComparisonRequest(HttpServThread* serv, char* url,
                                        char* urlStub, unsigned andMask, unsigned offset)
{
  // Process the URL and extract and check parameters
  float latLong[2]; // (lat, long)
  unless(extractColonVecN(url, 2, latLong))
   {
    LogRequestErrors("Bad station comparison request: %s?%s\n", urlStub, url);
    return serv->errorPage("Bad request");
   }
  unless(checkLatLong(latLong))
   {
    LogRequestErrors("Bad lat/long in request %s?%s\n", urlStub, url);
    return serv->errorPage("Bad request");
   }
  
  int resultHitGoal = 20;
  int searchHitGoal = resultHitGoal*1.2;
  int N;
  int base;
  int hitsAchieved;
  std::vector<GHCNStation*> relevantStations;
  std::vector<unsigned> indices;
  std::vector<bool> skipStations;
  std::vector<std::vector<int>*> years;
  std::vector<std::vector<float>*> diffs;
  std::vector<int> yearIndices;

  // Loop finding/analyzing stations till we get enough
  while(1)
   {
    // Find the relevant stations
    ghcnDatabase->searchStations(latLong[0], latLong[1], relevantStations, indices, 
                                 searchHitGoal, andMask, 0u);
    N = relevantStations.size();
    LogClimateCompDetails("Found %d stations after goal of %d.\n", N, searchHitGoal); 

    // Find the best base year
    base = -1;
    int biggestYearCount = 0;
    //float closestLatLongDist  = 180.0f;
    skipStations.resize(N);
    LogClimateCompDetails("Finding best base station in %d stations.\n", N); 
    for(int i=0; i<N; i++)
     {
      ClimateInfo* info = relevantStations[i]->climate;
      unless(info->nYears)
       {
        LogClimateCompDetails("Ignoring base %d:%s as no valid years.\n", i, 
                                                                    relevantStations[i]->id);
        continue;
       }
      if(info->nYears > biggestYearCount)
       {
        LogClimateCompDetails("Switching to base %d:%s as %d/%d years is better than"
                            " %d/%d.\n", i, relevantStations[i]->id, info->nYears, 
                            info->endYear - info->startYear, biggestYearCount,
                            info->endYear - info->startYear); 
        biggestYearCount = info->nYears;
        base = i;
        continue;
       }
     }
 
    // Figure out if we have a first valid station
    if(base < 0 || base >= N)
     {
      LogClimateCompDetails("Pass failed for %.3f, %.3f: no valid stations in %d.\n", 
                                                          latLong[0], latLong[1], N);
      // Increase the number of stations to search for and try again
      goto TryAgain;
     }
 
    // Gather the data for the table.
    LogClimateCompDetails("Comparing %d stations.\n", N); 
    years.resize(N);
    diffs.resize(N);
    yearIndices.resize(N);
    hitsAchieved = 0;
    for(int i=0; i<N; i++)
     {
      years[i] = new std::vector<int>;
      diffs[i] = new std::vector<float>;
      LogClimateCompDetails("About to compare %d:%s to %d:%s.\n", base,             
                                    relevantStations[base]->id, i, relevantStations[i]->id);
      if(relevantStations[base]->climate->diffObservable(relevantStations[i]->climate,
                      *(years[i]), *(diffs[i]), andMask, offset)
         && diffs[i]->size() > 5)
       {
        LogClimateCompDetails("Valid comparison of %d:%s to %d:%s.\n", base, 
                                    relevantStations[base]->id, i, relevantStations[i]->id); 
        skipStations[i] = false;
        hitsAchieved++;
       }
      else
       {
        LogClimateCompDetails("Failed comparison of %d:%s to %d:%s.\n", base, 
                                    relevantStations[base]->id, i, relevantStations[i]->id); 
        skipStations[i] = true;
       }
     }
  
    if(hitsAchieved >= resultHitGoal)
      break; // success.  
    
    // Rest of loop is only reached on failure or from 'goto TryAgain'
    
TryAgain:    
    searchHitGoal *= 1.2;
    if(relevantStations.size())
      relevantStations.clear();
    if(indices.size())
      indices.clear();
    if(skipStations.size())
      skipStations.clear();
    if(yearIndices.size())
      yearIndices.clear();
    if(years.size())
     {
      for(int i=0; i<N; i++)
        delete years[i];
      years.clear();
     }
    if(diffs.size())
     {
      for(int i=0; i<N; i++)
        delete diffs[i];
      diffs.clear();
     }
   }
  
  // Compute the average differences
  float totals[N];
  int counts[N];
  for(int i=0; i<N; i++)
   {
    if(skipStations[i])
      continue;
    yearIndices[i] = 0;
    counts[i] = 0;
    totals[i] = 0.0f;
    int M = diffs[i]->size();
    for(int j=0; j<M; j++)
     {
      counts[i]++;
      totals[i] += -(*(diffs[i]))[j];
     }
   }
  
  // Start the HTML page
  char title[128];
  snprintf(title, 128, "Comparing temps %s for stations near %.3f, %.3f",
                                                      urlStub, latLong[0], latLong[1]);
  unless(serv->startResponsePage(title))
    return false;

  // Table header
  unless(stationTableHeader(serv, latLong, relevantStations, skipStations))
    return false;
 
  // Lay out the main table of yearly rows
  int rows = relevantStations[base]->climate->endYear - 
                                                relevantStations[base]->climate->startYear;
  for(int j=0; j < rows; j++)
   {
    int thisYear = j + relevantStations[base]->climate->startYear;
    httPrintf("<tr><td>%d</td>", thisYear);
    for(int i=0; i<N; i++)
     {
      if(skipStations[i])
        continue;
      while((*(years[i]))[yearIndices[i]] < thisYear)
        yearIndices[i]++;
      
      if((*(years[i]))[yearIndices[i]] > thisYear)
       {
        LogClimateCompDetails("j: %d,i: %d, No year available for %d.\n", j, i, thisYear);
        httPrintf("<td></td>");
       }
      else
       {
        LogClimateCompDetails("j: %d,i: %d, Comparing %d[%d] to %d.\n", j, i, 
                                        (*(years[i]))[yearIndices[i]], yearIndices[i], thisYear);
        httPrintf("<td>%.3f</td>", -(*(diffs[i]))[yearIndices[i]]);
       }
     }
    httPrintf("</tr>");
   }
  
  // Row of averages
  httPrintf("<tr><td><b>Averages</b></td>")
  for(int i=0; i<N; i++)
   {
    if(skipStations[i])
      continue;
    httPrintf("<td>%.3f</td>", totals[i]/counts[i]);
   }
  httPrintf("</tr>");

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
  std::vector<bool> skipStations;
  int N = relevantStations.size();
  for(int i=0; i< N; i++)
    skipStations.push_back(false);
  unless(stationTableHeader(serv, latLongYear, relevantStations, skipStations))
    return false;
  
  // Main table of the temperature info
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
