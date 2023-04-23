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
  
  // Make sure we are logged in, and get the username
  const char* loginName = serv->getLoggedInUserName();
  unless(loginName)
   {
    LogResponseErrors("pmodServer request for %s when not logged in.\n", url);
    return serv->errorPage("Need to login to do that.");
   }
  
  // manualUpload.html
  if(strlenUrl == 17 && strncmp(url, "manualUpload.html", 17) == 0)
   {
    LogPermaservOpDetails("Processing pmodServer manual upload request for user %s.\n",
                                                                                loginName);
    retVal = provideManualUploadForm(serv);
   }

  // fileUpload
  else if(serv->reqParser.requestMethod == POST && strlenUrl == 10 
                                                && strncmp(url, "fileUpload", 10) == 0)
   {
    LogPermaservOpDetails("Processing upload of file from user %s.\n", loginName);
    retVal = processFileUploadRequest(serv);
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
/// @brief Accept and check an uploaded OLDF file.  
/// 
/// @returns True if all was well writing to the buffer.  If false, it indicates the 
/// buffer was not big enough and the output will have been truncated/incomplete.
/// @param serv A pointer to the HttpServThread managing the HTTP response.

bool PmodServer::processFileUploadRequest(HttpServThread* serv)
{
  
  // UP TO HERE
  
  // How are uploaded files encoded?  Is it in a form?
  return false;
}



/// =======================================================================================
/// @brief Provide a form which a user can use to upload an OLDF file to their account.  
/// 
/// @returns True if all was well writing to the buffer.  If false, it indicates the 
/// buffer was not big enough and the output will have been truncated/incomplete.
/// @param serv A pointer to the HttpServThread managing the HTTP response.

bool PmodServer::provideManualUploadForm(HttpServThread* serv)
{
  // Start the HTML page and the table header
  unless(serv->startResponsePage((char*)"Manually Upload an OLDF file"))
    return false;
  
  // Open Upload Form
  httPrintf("<center>\n");
  httPrintf("<form action=\"fileUpload\" method=\"post\" "
                                                  " enctype=\"multipart/form-data\">\n");
  
  // Open the main section
  httPrintf("<div class=\"container\">\n");
  
  // File to upload
  httPrintf("<label for=\"file\"><b>Choose a file to upload:</b></label>\n");
  httPrintf("<input type=\"file\" id=\"file\" name=\"uploaded_file\"  required>\n");
  httPrintf("<br><br>\n");
    
  // Upload button
  httPrintf("<button type=\"submit\">Upload</button>\n");

  // End section
  httPrintf("</div>\n");
  
  // End the form
  httPrintf("</form></center>\n");
  
  // Finish up the page
  unless(serv->endResponsePage())
    return false;

  return true;  
}


// =======================================================================================
