// Copyright Staniford Systems.  All Rights Reserved.  March 2023 -
// Implements an unordered_map of things that need to have a timeout

#include "TimeoutMap.h"


// =======================================================================================
/// @brief Constructor

Timeoutable::Timeoutable(void)
{
  lastAccess.now();
}


// =======================================================================================
/// @brief Destructor

Timeoutable::~Timeoutable(void)
{
}


// =======================================================================================
