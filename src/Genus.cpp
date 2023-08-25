// Copyright Staniford Systems.  All Rights Reserved.  June 2023 -
// Models a biological genus as part of a Taxonomy.

#include "Genus.h"
#include "Species.h"
#include "HttpServThread.h"
#include "Logging.h"
#include <string.h>


// =======================================================================================
/// @brief Constructor

Genus::Genus(char* genusName)
{
  name = strdup(genusName);
}


// =======================================================================================
/// @brief Destructor

Genus::~Genus(void)
{
}


// =======================================================================================
/// @brief (Possibly) add a new species to this genus.
/// @returns True if we added something, false otherwise.
/// @param species The name of the species
/// @param genus The name of the genus

bool Genus::add(char* species)
{
  if(count(species))
   {
    return true;
   }
  else
   {
    Species* newSpecies = new Species(name, species);
    insert({std::string(species), newSpecies});
    LogTaxonDetails("Adding new Species %s to Genus %s.\n", species, name);
    return true; 
   }
}


/// =======================================================================================
/// @brief Output HTML page of genus with species known to us.
/// 
/// @returns True if all was well writing to the buffer.  If false, it indicates the 
/// buffer was not big enough and the output will have been truncated/incomplete.
/// @param serv A pointer to the HttpServThread managing the HTTP response.

bool Genus::provideGenusPage(HttpServThread* serv)
{  
  // Start the HTML page and the table header
  char title[256];
  snprintf(title, 256, "Species in Genus %s", name);
  unless(serv->startResponsePage(title))
    return false;
  httPrintf("<center>"); 
  unless(serv->startTable(title))
    return false;
  httPrintf("<tr><th>Row</th><th>Species Name</th><th>Common Name</th></tr>\n");
  
  // Loop over the species
  int rowIndex = 1;
  for (auto i : *this)
   {
    httPrintf("<tr><td>%d</td><td><a href=\"/taxonomy/species/%s_%s\">%s %s</a></td><td>", 
                    rowIndex++, name, i.first.c_str(), name, i.first.c_str()); 
    if(i.second->commonName)
     {
      httPrintf("%s", i.second->commonName);
     }      
    httPrintf("</td></tr>\n");
   }

  // Finish up the table
  httPrintf("</table></center>"); 
  
  // Finish up the page
  serv->endResponsePage();
  return true;
}


// =======================================================================================
