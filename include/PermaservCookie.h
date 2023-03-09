// Copyright Staniford Systems.  All Rights Reserved.  March 2023 -

#ifndef PERMASERV_COOKIE_H
#define PERMASERV_COOKIE_H

#include "Global.h"


// =======================================================================================
/// @brief Keep track of the stuff that will go in a cookie to be served by Permaserv.
///

class PermaservCookie
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  PermaservCookie(void);
  ~PermaservCookie(void);
  
private:
  
  // Instance variables - private
  
  // Member functions - private
  PreventAssignAndCopyConstructor(PermaservCookie);
};


// =======================================================================================

#endif




