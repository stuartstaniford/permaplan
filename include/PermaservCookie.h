// Copyright Staniford Systems.  All Rights Reserved.  March 2023 -

#ifndef PERMASERV_COOKIE_H
#define PERMASERV_COOKIE_H

#include "Global.h"
#include <vector>

#define VALID_SESSION_ID 0x00000001


// =======================================================================================
/// @brief Forward declarations.

class HttpServThread;


// =======================================================================================
/// @brief Keep track of the stuff that will go in a cookie to be served by Permaserv.
///
/// See https://www.rfc-editor.org/rfc/rfc6265 for how cookies work.
/// 
/// Note the variables in here are being used both to store state found in requests,
/// and also to prepare corresponding responses.  Care needs to be taken to avoid
/// bleedover.

class PermaservCookie
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  PermaservCookie(void);
  ~PermaservCookie(void);
  void processRequestCookies(char* cookieValue);
  void setSessionId(unsigned long long id);
  void clearState(void);
  int sprint(char* buf);

private:
  
  // Instance variables - private
  unsigned long long sessionId;
  unsigned            flags;
  std::vector<char*>  badCookieList;
  
  // Member functions - private
  void processOneCookie(char* name, char* value);
  PreventAssignAndCopyConstructor(PermaservCookie);
};


// =======================================================================================

#endif




