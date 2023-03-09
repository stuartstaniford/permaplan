// Copyright Staniford Systems.  All Rights Reserved.  March 2023 -
// Keep track of the stuff that will go in a cookie to be served by Permaserv

#include "PermaservCookie.h"


// =======================================================================================
/// @brief Constructor

PermaservCookie::PermaservCookie(void)
{
}


// =======================================================================================
/// @brief Destructor

PermaservCookie::~PermaservCookie(void)
{
}


// =======================================================================================
/// @brief Destructor

void PermaservCookie::setSessionId(unsigned long long id)
{
  flags |= VALID_SESSION_ID;
  sessionId = id;
}


// =======================================================================================
