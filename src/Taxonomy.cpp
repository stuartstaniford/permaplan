// Copyright Staniford Systems.  All Rights Reserved.  June 2023 -
// Models a particular biological classification system.  This class should be used 
// for common features, while subclasses should deal with the details of parsing 
// particular sources of taxonomic information

#include "Taxonomy.h"
#include "Logging.h"
#include "BioClass.h"
#include "HttpServThread.h"


// =======================================================================================
/// @brief Constructor

Taxonomy::Taxonomy(void)
{
}


// =======================================================================================
/// @brief Destructor

Taxonomy::~Taxonomy(void)
{
}


// =======================================================================================
/// @brief Function to add a new species into the taxonomy.
/// @returns True if we added something, false otherwise.
/// @param species The name of the species
/// @param genus The name of the genus
/// @param family The name of the family
/// @param order The name of the order
/// @param bioClass The name of the taxonomic class

bool Taxonomy::add(char* species, char* genus, char* family, char* order, char* bioClass)
{
  if(bioClassesByName.count(bioClass))
   {
    return bioClassesByName[bioClass]->add(species, genus, family, order);
   }
  else
   {
    BioClass* newClass = new BioClass(bioClass);
    bioClassesByName.insert({std::string(bioClass), newClass});
    LogTaxonDetails("Adding new Class %s to taxonomy.\n", bioClass);
    return newClass->add(species, genus, family, order);
   }
}


// =======================================================================================
/// @brief Output HTML table of all taxonomy API options to main index page.
/// 
/// @returns True if all was well writing to the buffer.  If false, it indicates the 
/// buffer was not big enough and the output will have been truncated/incomplete.
/// @param serv A pointer to the HttpServThread managing the HTTP response.

bool Taxonomy::indexPageTable(HttpServThread* serv)
{
  httPrintf("<hr>\n");
  httPrintf("<center>\n");
  httPrintf("<h2>Taxonomy operations</h2>\n");
  unless(serv->startTable())
    return false;
  httPrintf("<tr><th>Link</th><th>notes</th></tr>\n");

  // Class list
  httPrintf("<tr><td><a href=\"/taxonomy/classes.html\">"
                                                  "/taxonomy/classes.html</a></td>");
  httPrintf("<td>Get a list of taxonomic classes we know of</td></tr>\n");

  // End table
  httPrintf("</table></center>\n");

  return true;
}


/// =======================================================================================
/// @brief Output HTML table of taxonomic classes known to us.
/// 
/// @returns True if all was well writing to the buffer.  If false, it indicates the 
/// buffer was not big enough and the output will have been truncated/incomplete.
/// @param serv A pointer to the HttpServThread managing the HTTP response.

bool Taxonomy::provideClassList(HttpServThread* serv)
{  
  // Start the HTML page and the table header
  unless(serv->startResponsePage("Classes in this Taxonomy"))
    return false;

  // Loop over the classes
  int classIndex = 1;
  for (auto i : bioClassesByName)
   {
    char title[128];
    snprintf(title, 128, "Class %d: %s", classIndex++, i.first.c_str());
    unless(serv->newSection(title))
      return false;
    unless(i.second->orderHTMLTable(serv))
      return false;
   }
  
  // Finish up the page
  unless(serv->endResponsePage())
    return false;

  return true;
}


/// =======================================================================================
/// @brief Gateway to handling all requests coming in under /taxonomy/.  
/// 
/// This just routes to the appropriate private method to handle the different specific
/// cases.
/// 
/// @returns True if all was well writing to the buffer.  If false, it indicates the 
/// buffer was not big enough and the output will have been truncated/incomplete.
/// @param serv A pointer to the HttpServThread managing the HTTP response.
/// @param url A string with the balance of the request url after "/oldf/".

bool Taxonomy::processHttpRequest(HttpServThread* serv, char* url)
{
  int strlenUrl = strlen(url);
  bool retVal = false;
  
  // Make sure we are logged in, and get the username

  // classes.html
  if(strlenUrl == 12 && strncmp(url, "classes.html", 12) == 0)
   {
    LogPermaservOpDetails("Processing taxonomy classes request.\n");
    retVal = provideClassList(serv);
   }

  // Default - failure
  else
   {
    LogRequestErrors("Request for unknown /taxonomy/ resource %s\n", url);
    retVal = serv->errorPage("Resource not found");
   }

  return retVal;
}


// =======================================================================================
