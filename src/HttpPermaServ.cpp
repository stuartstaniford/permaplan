// Copyright Staniford Systems.  All Rights Reserved.  January 2022 -
// This class ..... is a template for real classes

#include "HttpPermaServ.h"
#include "Logging.h"
#include "SolarDatabase.h"
#include "SoilDatabase.h"
#include "HttpLBPermaserv.h"


// =======================================================================================
/// @brief Constructor

HttpPermaServ::HttpPermaServ(unsigned index, SolarDatabase* solarD, SoilDatabase* soilD, 
                                                                    HttpLoadBalancer* parent):
                                          HttpServThread(index, parent),
                                          solarDatabase(solarD),
                                          soilDatabase(soilD)
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

  // Soil profile at a particular point
  internalPrintf("<tr><td><a href=\"/soilPoint?42.441570:-76.498665:/\">"
                                                            "soilPoint?lat:long:</a></td>");
  internalPrintf("<td>Soil Profile at location (json).</td></tr>\n");

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
  float dif = solarDatabase->getDIFValue(latLong[0], latLong[1]);
  internalPrintf("DIF: %.3f\n", dif);
  LogPermaservOps("Serviced DIF request (%.3f) for %f,%f from client on port %u.\n", 
                                                  dif, latLong[0], latLong[1], clientP);
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
  float dni = solarDatabase->getDNIValue(latLong[0], latLong[1]);
  internalPrintf("DNI: %.3f\n", dni);
  LogPermaservOps("Serviced DNI request (%.3f) for %f,%f from client on port %u.\n", 
                                                      dni, latLong[0], latLong[1], clientP);
  return true;
}


// =======================================================================================
/// @brief Process the case of a request for a soil profile at a particular location.
/// @param url The balance of the URL that we are to deal with (ie after the '?')
/// @returns True if all went well, false if we couldn't correctly write a good page.

bool HttpPermaServ::processSoilPointRequest(char* url)
{
  float latLong[2];
  unless(extractColonVecN(url, 2, latLong))
   {
    LogRequestErrors("Bad soilPoint request: dni?%s\n", url);
    return false;
   }
  if( (respPtr += soilDatabase->printJsonSoilProfileAtPoint(respPtr, 
                respEnd-respPtr, latLong[0], latLong[1])) >= respEnd)
   {
    respBufOverflow = true; 
    return false;
   }
  LogPermaservOps("Serviced soilPoint request for %f,%f from client on port %u.\n", 
                                                      latLong[0], latLong[1], clientP);
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
  
  if( (strlenUrl == 1 && url[0] == '/') || strncmp(url, "/index.", 7) == 0)
    return indexPage();

  // Possible paths (in alphabetical order  
  else if( strlenUrl == 7 && strncmp(url, "/alive/", 7) == 0)
   {
    internalPrintf("OK at time %ld\n", time(NULL));
    retVal = true;
   }

  else if( strlenUrl == 6 && strncmp(url, "/quit/", 6) == 0)
   {
    internalPrintf("Quitting at time %ld\n", time(NULL));
    timeToDie = true;
    retVal = true;
   }

  else if( strlenUrl == 13 && strncmp(url, "/compileTime/", 13) == 0)
   {
    internalPrintf("compileTime: %ld\n", ((HttpLBPermaserv*)parentLB)->compileTime);
    retVal = true;
   }

  else if( strlenUrl >= 9 && strncmp(url, "/dif?", 5) == 0)
   {
    retVal = processDIFRequest(url+5);
   }

  else if( strlenUrl >= 9 && strncmp(url, "/dni?", 5) == 0)
   {
    retVal = processDNIRequest(url+5);
   }

  else if( strlenUrl >= 15 && strncmp(url, "/soilPoint?", 11) == 0)
   {
    retVal = processSoilPointRequest(url+5);
   }

  else
   {
    LogRequestErrors("Request for unknown resource %s\n", url);
    errorPage("Resource not found");
   }
  
  return retVal;
}


// =======================================================================================
