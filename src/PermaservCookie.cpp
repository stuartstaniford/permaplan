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
/// Caller is responsible for the buffer being big enough
/// @returns The number of characters written.
/// @param buf The buffer to write to.


int PermaservCookie::sprint(char* buf)
{  
  char* p = buf;
  if(flags)
   {
    p += sprintf(p, "Set-Cookie: ");
    if(flags & VALID_SESSION_ID)
     {
      p += sprintf(p, "sessionId=%llu, ", sessionId);
     }
    p += sprintf(p, "\r\n");    
   }
  
  return p - buf;
}


// =======================================================================================
