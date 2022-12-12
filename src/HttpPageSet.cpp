// Copyright Staniford Systems.  All Rights Reserved.  December 2022 -
// Maps a set of paths to a set of instances of HttpStaticPage.  Used for resolving 
// calls to get pages that will be loaded on first request and then cached in memory.  
// We do not just serve anything found in a directory, but only things explicitly 
// coded to be served

#include "HttpPageSet.h"


// =======================================================================================
/// @brief Constructor

HttpPageSet::HttpPageSet(void)
{
}


// =======================================================================================
/// @brief Destructor

HttpPageSet::~HttpPageSet(void)
{
}


// =======================================================================================
