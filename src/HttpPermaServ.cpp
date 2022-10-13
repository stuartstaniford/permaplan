// Copyright Staniford Systems.  All Rights Reserved.  January 2022 -
// This class ..... is a template for real classes

#include "HttpPermaServ.h"
#include "Logging.h"
#include "SolarDatabase.h"
#include "SoilDatabase.h"
#include "HttpLBPermaserv.h"
#include "ClimateDatabase.h"


// =======================================================================================
/// @brief Constructor

HttpPermaServ::HttpPermaServ(unsigned index, SolarDatabase* solarD, SoilDatabase* soilD, 
                                        ClimateDatabase* climateD, HttpLoadBalancer* parent):
                                          HttpServThread(index, parent),
                                          solarDatabase(solarD),
                                          soilDatabase(soilD),
                                          climateDatabase(climateD)
{
}


// =======================================================================================
/// @brief Destructor

HttpPermaServ::~HttpPermaServ(void)
{
}


// =======================================================================================
/// @brief Generate the index page.
/// 
/// This generates the root navigation page for PermaServ, which serves to document the
/// API (and which would not generally be called by permaplan instances, but is here for
/// the benefit of passing humans).
/// @returns True if we successfully generated the HTML, false if we ran out of space
/// (which should never happen...)

bool HttpPermaServ::indexPage(void)
{
  unless(startResponsePage("PermaServ API"))
    return false;
  
  // Beginning of table
  internalPrintf("<center>\n");
  unless(startTable())
    return false;
  internalPrintf("<tr><th>Link</th><th>notes</th></tr>\n");

  // Alive (test server is up)
  internalPrintf("<tr><td><a href=\"/alive/\">alive/</a></td>");
  internalPrintf("<td>Confirm server is up and functioning.</td></tr>\n");

  // Quit (shut server down)
  internalPrintf("<tr><td><a href=\"/quit/\">quit/</a></td>");
  internalPrintf("<td>Shut the server down in an orderly manner.</td></tr>\n");

  // Compile time (When was the running program compiled?)
  internalPrintf("<tr><td><a href=\"/compileTime/\">compileTime/</a></td>");
  internalPrintf("<td>Determine when the running server was compiled.</td></tr>\n");
  
  // DIF at a particular point
  internalPrintf("<tr><td><a href=\"/dif?42.441570:-76.498665:\">dif?lat:long:</a></td>");
  internalPrintf("<td>Average diffuse horizontal irradiation at location (kWh/m²/day).</td></tr>\n");

  // DNI at a particular point
  internalPrintf("<tr><td><a href=\"/dni?42.441570:-76.498665:/\">dni?lat:long:</a></td>");
  internalPrintf("<td>Average direct normal irradiation at location (kWh/m²/day).</td></tr>\n");

  // Soil profiles in some region
  internalPrintf("<tr><td><a href=\"/soil?42.441570:42.441870:-76.498665:-76.496665:\">"
                                              "soil?loLat:hiLat:loLong:hiLong:</a></td>");
  internalPrintf("<td>Soil Profiles in region (json).</td></tr>\n");

  // Climate data near a particular point
  internalPrintf("<tr><td><a href=\"/climate?42.421:-76.347:20:\">"
                 "climate?lat:long:years:</a></td>");
  internalPrintf("<td>Years of climate information near location.</td></tr>\n");

  // Climate diagnostics near a particular point
  internalPrintf("<tr><td><a href=\"/climateDiagnostic?42.421:-76.347:20:\">"
                 "climateDiagnostic?lat:long:years:</a></td>");
  internalPrintf("<td>Climate station diagnostic near location.</td></tr>\n");

  // End table and page
  internalPrintf("</table></center>\n");
  endResponsePage();
  
  LogPermaservOps("Served index page to client on port %u.\n", clientP);
  
  return true;
}


// =======================================================================================
/// @brief Process the case of a request for a DIF value.
/// @param url The balance of the URL that we are to deal with (ie after the '?')
/// @returns True if all went well, false if we couldn't correctly write a good page.

bool HttpPermaServ::processDIFRequest(char* url)
{
  float latLong[2];
  unless(extractColonVecN(url, 2, latLong))
   {
    LogRequestErrors("Bad dif request: /dif?%s\n", url);
    return false;
   }
  float dif;
  if(solarDatabase)
   {
    dif = solarDatabase->getDIFValue(latLong[0], latLong[1]);
    LogPermaservOps("Serviced DIF request (%.3f) for %f,%f from client on port %u.\n", 
                                                    dif, latLong[0], latLong[1], clientP);
   }
  else
   {
    dif = 3.0f; // This is for the -s option to permaserv to run with no solar database.
    LogPermaservOps("Default value for DIF request to permaserv -s "
                                                  "from client on port %u.\n", clientP);
   }
  internalPrintf("DIF: %.3f\n", dif);
  return true;
}


// =======================================================================================
/// @brief Process the case of a request for a DNI value.
/// @param url The balance of the URL that we are to deal with (ie after the '?')
/// @returns True if all went well, false if we couldn't correctly write a good page.

bool HttpPermaServ::processDNIRequest(char* url)
{
  float latLong[2];
  unless(extractColonVecN(url, 2, latLong))
   {
    LogRequestErrors("Bad dni request: dni?%s\n", url);
    return false;
   }
  float dni;
  if(solarDatabase)
   {
    dni = solarDatabase->getDNIValue(latLong[0], latLong[1]);
    LogPermaservOps("Serviced DNI request (%.3f) for %f,%f from client on port %u.\n", 
                                                        dni, latLong[0], latLong[1], clientP);
   }
  else
   {
    dni = 3.0f; // This is for the -s option to permaserv to run with no solar database.
    LogPermaservOps("Default value for DNI request to permaserv -s "
                                                  "from client on port %u.\n", clientP);

   }
  internalPrintf("DNI: %.3f\n", dni);
  return true;
}


// =======================================================================================
/// @brief Process the case of a request for climate information.
/// @param url The balance of the URL that we are to deal with (ie after the '?')
/// @param diagnostic A bool that if true provides HTML diagnostic pages rather than 
/// the JSON API.  Used for troubleshooting the climate data and the permaserv code 
/// handling it.
/// @returns True if all went well, false if we couldn't correctly write a good object.

bool HttpPermaServ::processClimateRequest(char* url, bool diagnostic)
{
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
  if(diagnostic)
   {
    unless(climateDatabase->printStationDiagnosticTable(this, 
                            latLongYear[0], latLongYear[1], (unsigned)latLongYear[2]))
     {
      LogClimateDbErr("Overflow in html response to climate diagnostic request "
                      "/climateDiagnostic?%s.\n", url);
      respBufOverflow = true; 
      return false;
     }
    LogPermaservOps("Serviced climate diagnostic request for %f,%f (%u years) "
                  "from client on port %u.\n", 
                  latLongYear[0], latLongYear[1], (unsigned)latLongYear[2], clientP);
   }
  else
   {
    if( (respPtr += climateDatabase->printClimateJson(respPtr, respEnd-respPtr, 
                                  latLongYear[0], latLongYear[1], (unsigned)latLongYear[2]))
                    >= respEnd)
     {
      LogClimateDbErr("Overflow in json response to climate request /climate?%s.\n", url);
      respBufOverflow = true; 
      return false;
     }
    LogPermaservOps("Serviced climate request for %f,%f (%u years) from client on port %u.\n", 
                  latLongYear[0], latLongYear[1], (unsigned)latLongYear[2], clientP);
   }
  return true;
}


// =======================================================================================
/// @brief Process the case of a request for the soil profiles available in some specific
/// region of lat/long space.
/// @param url The balance of the URL that we are to deal with (ie after the '?')
/// @returns True if all went well, false if we couldn't correctly write a good page.

bool HttpPermaServ::processSoilRequest(char* url)
{
  float latLongRegion[4]; // (loLat, hiLat, loLong, hiLong) 
  unless(extractColonVecN(url, 4, latLongRegion))
   {
    LogRequestErrors("Bad soil request: soil?%s\n", url);
    return false;
   }
  if( (respPtr += soilDatabase->printJsonSoilProfiles(respPtr, respEnd-respPtr, 
                    latLongRegion[0], latLongRegion[1], latLongRegion[2], latLongRegion[3]))
          >= respEnd)
   {
    LogSoilDbErr("Overflow in json response to soil request %s.\n", url);
    respBufOverflow = true; 
    return false;
   }
  LogPermaservOps("Serviced soil request for %f,%f,%f,%f from client on port %u.\n", 
                  latLongRegion[0], latLongRegion[1], latLongRegion[2], latLongRegion[3],
                  clientP);
  return true;
}


// =======================================================================================
/// @brief Process a single header, and construct the response.
/// 
/// Calls the HTTRequestParser instance to extract the URL, and then routes the request
/// to whatever the apppropriate other object is to handle it.
/// @returns True if all went well, false if we couldn't correctly write a good page.

bool HttpPermaServ::processRequestHeader(void)
{
  bool retVal   = false;
  char* url     = reqParser.getUrl();
  int strlenUrl = strlen(url);
  
  LogPermaservOpDetails("Got request for url %s.\n", url);

  if( (strlenUrl == 1 && url[0] == '/') || strncmp(url, "/index.", 7) == 0)
   {
    LogPermaservOpDetails("Processing index request.\n");
    return indexPage();
   }
  
  // Possible paths (in alphabetical order  
  else if( strlenUrl == 7 && strncmp(url, "/alive/", 7) == 0)
   {
    LogPermaservOpDetails("Processing alive check request.\n");
    internalPrintf("OK at time %ld\n", time(NULL));
    retVal = true;
   }

  else if( strlenUrl >= 23 && strncmp(url, "/climateDiagnostic?", 19) == 0)
   {
    LogPermaservOpDetails("Processing climate diagnostic request for %s.\n", url+19);
    retVal = processClimateRequest(url+19, true);
   }

  // http://127.0.0.1:2091/climateStation/US1NYTM0018
  else if( strlenUrl == 27 && strncmp(url, "/climateStation/", 16) == 0)
   {
    LogPermaservOpDetails("Processing climate station request for %s.\n", url+16);
    retVal = climateDatabase->processStationDiagnosticRequest(this, url+16);
   }
  
  else if( strlenUrl >= 13 && strncmp(url, "/climate?", 9) == 0)
   {
    LogPermaservOpDetails("Processing climate request for %s.\n", url+9);
    retVal = processClimateRequest(url+9);
   }

   else if( strlenUrl == 13 && strncmp(url, "/compileTime/", 13) == 0)
   {
    LogPermaservOpDetails("Processing compileTime request.\n");
    internalPrintf("compileTime: %ld\n", ((HttpLBPermaserv*)parentLB)->compileTime);
    retVal = true;
   }

   else if( strlenUrl >= 9 && strncmp(url, "/dif?", 5) == 0)
    {
     LogPermaservOpDetails("Processing DIF request for %s.\n", url+5);
     retVal = processDIFRequest(url+5);
    }

   else if( strlenUrl >= 9 && strncmp(url, "/dni?", 5) == 0)
   {
    LogPermaservOpDetails("Processing DNI request for %s.\n", url+5);
    retVal = processDNIRequest(url+5);
   }

  else if( strlenUrl == 6 && strncmp(url, "/quit/", 6) == 0)
   {
    LogPermaservOpDetails("Processing quit request.\n");
    internalPrintf("Quitting at time %ld\n", time(NULL));
    timeToDie = true;
    retVal = true;
   }

 else if( strlenUrl >= 14 && strncmp(url, "/soil?", 6) == 0)
   {
    LogPermaservOpDetails("Processing soil request for %s.\n", url+6);
    retVal = processSoilRequest(url+6);
   }

  else
   {
    LogRequestErrors("Request for unknown resource %s\n", url);
    errorPage("Resource not found");
   }
    
  return retVal;
}


// =======================================================================================
