// Copyright Staniford Systems.  All Rights Reserved.  April 2023 -
// This class coordinates operations for the handling of PmodDesign related HTTP
// operations in Permaserv.

#include "PmodServer.h"
#include "HttpServThread.h"
#include "loadFileToBuf.h"
#include "Logging.h"


// =======================================================================================
/// @brief Constructor

PmodServer::PmodServer(void)
{
}


// =======================================================================================
/// @brief Destructor

PmodServer::~PmodServer(void)
{
}


// =======================================================================================
/// @brief Output HTML table of all pmod server API options to main index page.
/// 
/// @returns True if all was well writing to the buffer.  If false, it indicates the 
/// buffer was not big enough and the output will have been truncated/incomplete.
/// @param serv A pointer to the HttpServThread managing the HTTP response.

bool PmodServer::indexPageTable(HttpServThread* serv)
{
  httPrintf("<hr>\n");
  httPrintf("<center>\n");
  httPrintf("<h2>Pmod Server (OLDF) operations</h2>\n");
  unless(serv->startTable())
    return false;
  httPrintf("<tr><th>Link</th><th>notes</th></tr>\n");

  // Manual Upload of an OLDF file
  httPrintf("<tr><td><a href=\"/oldf/manualUpload.html\">"
                                                  "/oldf/manualUpload.html</a></td>");
  httPrintf("<td>Form to manually upload an OLDF file for the current user.</td></tr>\n");

  // End table
  httPrintf("</table></center>\n");

  return true;
}


/// =======================================================================================
/// @brief Gateway to handling all requests coming in under /oldf/.  
/// 
/// This just routes to the appropriate private method to handle the different specific
/// cases.
/// 
/// @returns True if all was well writing to the buffer.  If false, it indicates the 
/// buffer was not big enough and the output will have been truncated/incomplete.
/// @param serv A pointer to the HttpServThread managing the HTTP response.
/// @param url A string with the balance of the request url after "/oldf/".

bool PmodServer::processHttpRequest(HttpServThread* serv, char* url)
{
  int strlenUrl = strlen(url);
  bool retVal = false;
  
  // manualUpload.html
  if(strlenUrl == 17 && strncmp(url, "manualUpload.html", 17) == 0)
   {
    LogPermaservOpDetails("Processing pmodServer manual upload request.\n");
    retVal = provideManualUploadForm(serv);
   }

  // XXXX
  else if( strlenUrl == 26 && strncmp(url, "climateStation/", 15) == 0)
   {
    LogPermaservOpDetails("Processing climate station request for %s.\n", url+15);
    //retVal = processStationDiagnosticRequest(serv, url+15);
   }
  
  // Default - failure
  else
   {
    LogRequestErrors("Request for unknown /oldf/ resource %s\n", url);
    retVal = serv->errorPage("Resource not found");
   }

  return retVal;
}


/// =======================================================================================
/// @brief Provide a form which a user can use to upload an OLDF file to their account.  
/// 
/// @returns True if all was well writing to the buffer.  If false, it indicates the 
/// buffer was not big enough and the output will have been truncated/incomplete.
/// @param serv A pointer to the HttpServThread managing the HTTP response.

bool PmodServer::provideManualUploadForm(HttpServThread* serv)
{
  return false;  
}


// =======================================================================================
