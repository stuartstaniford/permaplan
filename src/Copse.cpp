// Copyright Staniford Systems.  All Rights Reserved.  Mar 2021 -
// This class is to capture groups of trees

#include "Copse.h"
#include "BoundingBox.h"


// =======================================================================================
/// @brief Constructor

Copse::Copse(void)
{
  box = new BoundingBox(); // will hugevalify
}


// =======================================================================================
/// @brief Destructor

Copse::~Copse(void)
{
  delete box;
}


// =======================================================================================
