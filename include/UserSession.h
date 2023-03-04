// Copyright Staniford Systems.  All Rights Reserved.  March 2023 -

#ifndef USER_SESSION_H
#define USER_SESSION_H

#include "Global.h"
#include "Lockable.h"


// =======================================================================================
// Forward declarations

class UserManager;


// =======================================================================================
/// @brief A record of information for managing a single user session.
///

class UserSession
{
  friend UserManager; // No other class should mess with these, so everything is private

private:

  // Instance variables - private
  unsigned long long theId;
  
  // Static variables
  static Lockable idLock;
  static unsigned long long masterId;
  
  // Member functions - private
  UserSession(void);
  ~UserSession(void);
  PreventAssignAndCopyConstructor(UserSession);  
};


// =======================================================================================

#endif




