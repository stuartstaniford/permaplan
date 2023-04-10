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


// =======================================================================================
