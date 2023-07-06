// Copyright Staniford Systems.  All Rights Reserved.  June 2023 -
// Holder for a taxonomic family (containing genera, which contain species).

#include "Family.h"
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
