// Copyright Staniford Systems.  All Rights Reserved.  June 2023 -
// Holder for a taxonomic family (containing genera, which contain species).

#include "Family.h"
#include "Genus.h"
#include "Logging.h"
#include <string.h>


// =======================================================================================
/// @brief Constructor

Family::Family(char* familyName)
{
  name = strdup(familyName);
}


// =======================================================================================
/// @brief Destructor

Family::~Family(void)
{
  free(name);
}


// =======================================================================================
/// @brief (Possibly) add a new element to this family.
/// @returns True if we added something, false otherwise.
/// @param species The name of the species
/// @param genus The name of the genus

bool Family::add(char* species, char* genus)
{
  if(count(genus))
   {
    //(*this)[genus]->add(species);
    return true;
   }
  else
   {
    Genus* newGenus = new Genus(genus);
    insert({std::string(genus), newGenus});
    LogTaxonDetails("Adding new Genus %s to Family %s.\n", genus, name);
    //newGenus->add(species, genus);
    return true; 
   }
}


// =======================================================================================
