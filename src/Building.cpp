// Copyright Staniford Systems.  All Rights Reserved.  July 2021 -
// This class exists to allow us to track the relationship of a series of gables, sheds,
// etc that are managed as a single things (unless disaggregated to manage the
// relationship of the individual pieces.

#include "Building.h"


// =======================================================================================
/// @brief Constructor

Building::Building(VisualObject* firstComponent):
                          ObjectGroup(firstComponent)
{
}


// =======================================================================================
/// @brief Destructor

Building::~Building(void)
{
}


// =======================================================================================
