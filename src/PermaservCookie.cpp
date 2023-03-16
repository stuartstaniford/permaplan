// Copyright Staniford Systems.  All Rights Reserved.  March 2023 -
// Keep track of the stuff that will go in a cookie to be served by Permaserv

#include "PermaservCookie.h"
#include "HttpServThread.h"
#include "Logging.h"
#include <ctype.h>


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
/// @brief Function to deal with a single cookie-pair .  
/// 
/// So that processRequestCookies() doesn't get too long.
///
/// @param name A pointer to the cookie-name.
/// @param value A pointer to the cookie-value.

void PermaservCookie::processOneCookie(char* name, char* value)
{
  // RFC 6265 Sec Sec 4.1.1
  // cookie-name       = token
  // cookie-value      = *cookie-octet / ( DQUOTE *cookie-octet DQUOTE )
  // cookie-octet      = %x21 / %x23-2B / %x2D-3A / %x3C-5B / %x5D-7E
  //                      ; US-ASCII characters excluding CTLs,
  //                      ; whitespace DQUOTE, comma, semicolon,
  //                      ; and backslash

  if(strcmp(name, "sessionId") == 0)
   {
    for(char* p = value; *p; p++)
      unless(isdigit((int)*p))
        goto BAD_COOKIE;
    char* endP;
    unsigned long long s = strtoull(value, &endP, 16);
    unless(s)
      goto BAD_COOKIE;
    sessionId = s;
    flags |= VALID_SESSION_ID;
   }
  else
   {
    // When we get a cookie we don't recognize, we tell the client to delete it, so
    // that any old/stale cookies don't hang around.
    goto BAD_COOKIE;
   }
  
BAD_COOKIE:  
  LogRequestErrors("Unknown or bad cookie %s with value %s.  Recording for deletion.\n",
                   name, value)
  flags |= BAD_COOKIES_PRESENT;
  return;
}

// =======================================================================================
/// @brief Entry point to the class to process cookies in an HTTP request.  
/// 
/// This is called after the header is completely parsed by HttpRequestParser, but
/// before request processing is done in processRequestHeader() by the approcpiate
/// subclass of HttpServThread.
/// Quoth RFC 6265: "When the user agent generates an HTTP request, the user 
/// agent MUST NOT attach more than one Cookie header field."
///
/// @param cookieValue A pointer to the string after the Cookie: header in the request.
/// This function will modify it by changing ';', '=' and some WS to '\0';

void PermaservCookie::processRequestCookies(char* cookieValue)
{
  // RFC 6265 Sec 4.2.1 
  // cookie-header = "Cookie:" OWS cookie-string OWS
  // cookie-string = cookie-pair *( ";" SP cookie-pair )
  // IBID Sec 4.1.1
  // cookie-pair       = cookie-name "=" cookie-value
  // cookie-name       = token
  // cookie-value      = *cookie-octet / ( DQUOTE *cookie-octet DQUOTE )
  // cookie-octet      = %x21 / %x23-2B / %x2D-3A / %x3C-5B / %x5D-7E
  //                      ; US-ASCII characters excluding CTLs,
  //                      ; whitespace DQUOTE, comma, semicolon,
  //                      ; and backslash

  // Set up pointers to the beginning and end of string
  char* end = rindex(cookieValue, '\0');
  char* p = cookieValue;
  
  // Deal with optional white space at beginning and end of cookie-string
  while(isspace(*p))
    p++;
  while(end > p && isspace(*(end-1)))
    *(--end) = '\0';
    
  // Loop over all the cookie-pairs
  while(p < end)
   {
    // Find the end of the cookie-pair
    char* pairEnd = index(p, ';');
    if(pairEnd)
      *pairEnd = '\0';
    else
      pairEnd = end;
    
    // Separate the cookie-name from the cookie-value
    char* cookieVal = index(p, '=');
    if(cookieVal)
     {
      *cookieVal = '\0';
      if(cookieVal < end-1)
       {
        cookieVal++;
       }
      else
       {
        // Cookie token ends in '='
        LogRequestErrors("Bad Cookie %s ends in '='.\n", p);
        return;
       }
     }
    else
     {
      // No '=' in Cookie
      LogRequestErrors("Bad Cookie %s has no '='.\n", p);
      return;
     }
    
    // At this point, p is the cookie-name, and cookieVal is the cookie-value
    processOneCookie(p, cookieValue);
    
    // Test if we are done, or else increment p
    if(pairEnd == end)
      break;
    else
     {
      p = pairEnd + 1;
      while(isspace(*p))  // space after semi-colon
        p++;
     }
   }
}


// =======================================================================================
/// @brief Output our Set-Cookie header to a buffer.
///
/// Also sets the flags to invalid on anything we are sending, so it doesn't get sent
/// twice.  Caller is responsible for the buffer being big enough
/// @returns The number of characters written.
/// @param buf The buffer to write to.


int PermaservCookie::sprint(char* buf)
{  
  char* p = buf;
  if(flags)
   {
    if(flags & VALID_SESSION_ID)
     {
      p += sprintf(p, "Set-Cookie: sessionId=%llX", sessionId);

      // Print attributes here
      p += sprintf(p, "\r\n");    
      flags &= ~VALID_SESSION_ID; // don't set it again after we set it.
     }
   }
  
  return p - buf;
}


// =======================================================================================
