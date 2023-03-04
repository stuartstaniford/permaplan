// Copyright Staniford Systems.  All Rights Reserved.  March 2023 -

#ifndef USER_SESSION_H
#define USER_SESSION_H

#include "Global.h"
#include "Lockable.h"
#include "TimeoutMap.h"


// =======================================================================================
// Forward declarations

class UserManager;


// =======================================================================================
/// @brief A record of information for managing a single user session.
///

class UserSession: private Timeoutable
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
/// @brief A group of user sessions that can be remembered over time to maintain state.
/// 

class UserSessionGroup: private TimeoutMap<unsigned long long>
{
  friend UserManager; // No other class should mess with these, so everything is private

private:

  // Instance variables - private
  
  // Member functions - private
  UserSessionGroup(void);
  ~UserSessionGroup(void);
  PreventAssignAndCopyConstructor(UserSessionGroup);
};


// =======================================================================================

#endif




