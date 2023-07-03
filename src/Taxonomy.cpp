// Copyright Staniford Systems.  All Rights Reserved.  June 2023 -
// Models a particular biological classification system.  This class should be used 
// for common features, while subclasses should deal with the details of parsing 
// particular sources of taxonomic information

#include "Taxonomy.h"
#include "Logging.h"
#include "BioClass.h"


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
