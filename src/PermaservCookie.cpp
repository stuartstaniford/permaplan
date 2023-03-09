// Copyright Staniford Systems.  All Rights Reserved.  March 2023 -
// Keep track of the stuff that will go in a cookie to be served by Permaserv

#include "PermaservCookie.h"
#include "HttpServThread.h"


// =======================================================================================
/// @brief Constructor

PermaservCookie::PermaservCookie(void): 
                                        sessionId(0u),
                                        flags(0u)
{
}


// =======================================================================================
/// @brief Destructor

PermaservCookie::~PermaservCookie(void)
{
}


// =======================================================================================
/// @brief Set the session id

void PermaservCookie::setSessionId(unsigned long long id)
{
  flags |= VALID_SESSION_ID;
  sessionId = id;
}


// =======================================================================================
/// @brief Output our Set-Cookie header to a buffer
///
/// @returns false if not everything would fit, true if it did.
/// @param buf The buffer to write to.
/// @param bufLen The space in that buffer (in bytes).


bool PermaservCookie::snprint(HttpServThread* serv, char* buf, unsigned bufLen)
{  
  if(flags)
   {
    httPrintf("Set-Cookie: ");
    if(flags & VALID_SESSION_ID)
     {
      httPrintf("sessionId=%llu, ", sessionId);
     }
    httPrintf("\r\n");    
   }
  
  return true;
}


// =======================================================================================
