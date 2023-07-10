// Copyright Staniford Systems.  All Rights Reserved.  June 2023 -
// Models a biological genus as part of a Taxonomy.

#include "Genus.h"
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
