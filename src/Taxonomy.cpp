// Copyright Staniford Systems.  All Rights Reserved.  June 2023 -
// Models a particular biological classification system.  This class should be used 
// for common features, while subclasses should deal with the details of parsing 
// particular sources of taxonomic information

#include "Taxonomy.h"
#include "Logging.h"


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

bool Taxonomy::add(char* species, char* genus, char* family, char* order, char* bioClass)
{
  LogTaxonDetails("Adding %s %s to taxonomy.\n", genus, species);
  return true;
}


// =======================================================================================
