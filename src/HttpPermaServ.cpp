// Copyright Staniford Systems.  All Rights Reserved.  January 2022 -
// This class ..... is a template for real classes

#include "HttpPermaServ.h"
#include "Logging.h"
#include "SolarDatabase.h"


// =======================================================================================
/// @brief Constructor

HttpPermaServ::HttpPermaServ(unsigned index, SolarDatabase* solarD):
                            HttpServThread(index),
                            solarDatabase(solarD)

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
  
  // DIF
  internalPrintf("<tr><td><a href=\"/dif?42.441570,-76.498665/\">dif?lat,long</a></td>");
  internalPrintf("<td>Average diffuse horizontal irradiation at location (kWh/m²/day).</td></tr>\n");

  // DNI
  internalPrintf("<tr><td><a href=\"/dni?42.441570,-76.498665/\">dni?lat,long</a></td>");
  internalPrintf("<td>Average direct normal irradiation at location (kWh/m²/day).</td></tr>\n");

  // End table and page
  internalPrintf("</table></center>\n");
  endResponsePage();
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
  char* url = reqParser.getUrl();
  
  if( (strlen(url) == 1 && url[0] == '/') || strncmp(url, "/index.", 7) == 0)
    return indexPage();

  bool retVal = false;

  // Possible paths (in alphabetical order  
  if( strlen(url) == 7 && strncmp(url, "/alive/", 7) == 0)
   {
    internalPrintf("OK\n");
    retVal = true;
   }

  else if( strlen(url) >= 8 && strncmp(url, "/dif?", 5) == 0)
   {
    internalPrintf("DIF request\n");
    retVal = true;
   }

  else if( strlen(url) >= 8 && strncmp(url, "/dni?", 5) == 0)
   {
    internalPrintf("DNI request\n");
    retVal = true;
   }
  
  else
   {
    LogRequestErrors("Request for unknown resource %s\n", url);
    errorPage("Resource not found");
   }
  
  return retVal;
}


// =======================================================================================
