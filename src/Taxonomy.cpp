// Copyright Staniford Systems.  All Rights Reserved.  June 2023 -
// Models a particular biological classification system.  This class should be used 
// for common features, while subclasses should deal with the details of parsing 
// particular sources of taxonomic information

#include "Taxonomy.h"
#include "Logging.h"
#include "BioClass.h"
#include "Order.h"
#include "Family.h"
#include "Genus.h"
#include "Species.h"
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
  bool retval = false;
  
  if(bioClassesByName.count(bioClass))
   {
    retval = bioClassesByName[bioClass]->add(species, genus, family, order);
   }
  else
   {
    BioClass* newClass = new BioClass(bioClass);
    bioClassesByName.insert({std::string(bioClass), newClass});
    LogTaxonDetails("Adding new Class %s to taxonomy.\n", bioClass);
    retval = newClass->add(species, genus, family, order);
   }
  if(retval)
   {
    Order* orderObj     = (*(bioClassesByName[bioClass]))[order];
    ordersByName[order] = orderObj;
    Family* familyObj   = (*orderObj)[family];
    Genus* genusObj     = (*familyObj)[genus];
    generaByName[genus] = genusObj;
    Species* speciesObj = (*genusObj)[species];
    speciesByBinomial[std::string(genus) + std::string("_") + std::string(species) ] 
                        = speciesObj;
   }
  
  return retval;
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

  // Details of an Order
  httPrintf("<tr><td><a href=\"/taxonomy/order/Pinales\">"
                                                  "/taxonomy/order/Pinales</a></td>");
  httPrintf("<td>Get details for a specific order</td></tr>\n");
  
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

  // order/  
  if(strlenUrl >= 12  && strncmp(url, "order/", 6) == 0) // Arales is shortest
   {
    LogPermaservOpDetails("Processing taxonomy order request for %s.\n", url+6);
    unless(ordersByName.count(url+6) > 0)
     {
      LogRequestErrors("Request for unknown taxonomic order %s\n", url+6);
      retVal = serv->errorPage("Resource not found");
     }
    else
     {
      Order* orderObj = ordersByName[url+6];
      retVal = orderObj->provideOrderPage(serv);
     }
   }

  // genus/  
  if(strlenUrl >= 9  && strncmp(url, "genus/", 6) == 0) // Shortest have 3 letters (Zea)
   {
    LogPermaservOpDetails("Processing taxonomy genus request for %s.\n", url+6);
    unless(generaByName.count(url+6) > 0)
     {
      LogRequestErrors("Request for unknown genus %s\n", url+6);
      retVal = serv->errorPage("Resource not found");
     }
    else
     {
      Genus* genusObj = generaByName[url+6];
      retVal = genusObj->provideGenusPage(serv);
     }
   }

  // species/  
  if(strlenUrl >= 15  && strncmp(url, "species/", 8) == 0)
   {
    LogPermaservOpDetails("Processing taxonomy species request for %s.\n", url+8);
    unless(speciesByBinomial.count(url+8) > 0)
     {
      LogRequestErrors("Request for unknown species %s\n", url+8);
      retVal = serv->errorPage("Resource not found");
     }
    else
     {
      Species* speciesObj = speciesByBinomial[url+8];
      retVal = speciesObj->diagnosticHTML(serv);
     }
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
