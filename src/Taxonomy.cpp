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
    BioClass* newClass = new BioClass;
    bioClassesByName.insert({std::string(bioClass), newClass});
    LogTaxonDetails("Adding new Class %s to taxonomy.\n", bioClass);
   }
  return true;
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


// =======================================================================================
