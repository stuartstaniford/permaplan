// Copyright Staniford Systems.  All Rights Reserved.  March 2023 -
// Implements an unordered_map of things that need to have a timeout

#include "TimeoutMap.h"


// =======================================================================================
/// @brief Constructor.  
/// 
/// Records the time of creation.  Does not do anything with locks.

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
/// @brief Update the last access to the present.  

void Timeoutable::updateAccessToNow(void)
{
  lastAccess.now();
}


// =======================================================================================
