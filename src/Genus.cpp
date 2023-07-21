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
    //Species* newSpecies = new Species(species);
    //insert({std::string(species), newSpecies});
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

  // Loop over the species
/*  int famIndex = 1;
  for (auto i : *this)
   {
    char title[128];
    snprintf(title, 128, "Family %d: %s", famIndex++, i.first.c_str());
    unless(serv->newSection(title))
      return false;
    unless(i.second->generaHTMLTable(serv))
      return false;
   }*/
  
  // Finish up the page
  unless(serv->endResponsePage())
    return false;

  return true;
}


// =======================================================================================
