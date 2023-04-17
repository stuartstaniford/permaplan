// Copyright Staniford Systems.  All Rights Reserved.  March 2023 -
// Keep track of the stuff that will go in a cookie to be served by Permaserv

#include "PermaservCookie.h"
#include "HttpServThread.h"
#include "Logging.h"
#include <ctype.h>


// =======================================================================================
/// @brief Constructor

PermaservCookie::PermaservCookie(void)
{
  clearState();
}


// =======================================================================================
/// @brief Destructor

PermaservCookie::~PermaservCookie(void)
{
}


// =======================================================================================
/// @brief Set the session id

void PermaservCookie::setSessionId(unsigned long long id, unsigned duration)
{
  flags             |= VALID_SESSION_ID;
  sessionId         = id;
  sessionIdDuration = duration;
}


// =======================================================================================
/// @brief Clear the state of this object

void PermaservCookie::clearState(void)
{
  sessionId = 0u;
  flags     = 0u;
  badCookieList.clear();
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
      unless(isxdigit((int)*p))
       {
        LogRequestParsing("Bad character in sessionId value: %c.\n", *p);
        goto BAD_COOKIE;
       }
    char* endP;
    unsigned long long s = strtoull(value, &endP, 16);
    unless(s)
     {
      LogRequestParsing("Zero value for sessionId from %s.\n", value);
      goto BAD_COOKIE;
     }
    sessionId = s;
    flags |= VALID_SESSION_ID;
   }
  else
   {
    // When we get a cookie we don't recognize, we tell the client to delete it, so
    // that any old/stale cookies don't hang around.
    LogRequestParsing("Unrecognized cookie type %s with value %s.\n", name, value);
    goto BAD_COOKIE;
   }
  
  return;
  
BAD_COOKIE:  
  LogRequestErrors("Unknown or bad cookie %s with value %s.  Recording for deletion.\n",
                   name, value);
  badCookieList.push_back(name);
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
    processOneCookie(p, cookieVal);
    
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
  // For reference on the syntax of the Set_Cookie header, see 
  // https://www.rfc-editor.org/rfc/rfc6265#section-4
  
  char* p = buf;
  
  // Output the Session ID
  if(flags & VALID_SESSION_ID)
   {
    p += sprintf(p, "Set-Cookie: sessionId=%llX; Max-Age=%u; Path=/\r\n", 
                                                sessionId, sessionIdDuration);
    flags &= ~VALID_SESSION_ID; // don't set it again after we set it.
   }

  // Unset/clean-up any cookies we don't recognize
  for(char* name: badCookieList)
   {
    p += sprintf(p, "Set-Cookie: %s=0; Max-Age=-1; Path=/\r\n", name);
      
    // Print attributes here
   }
  
  return p - buf;
}


// =======================================================================================
