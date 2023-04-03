// Copyright Staniford Systems.  All Rights Reserved.  March 2023 -

#ifndef USER_SESSION_H
#define USER_SESSION_H

#include "Global.h"
#include "Lockable.h"
#include "TimeoutMap.h"

#define SESSION_TIMEOUT 1500


// =======================================================================================
// Forward declarations

class UserManager;
class UserSessionGroup;
class HttpServThread;
class HttpLoadBalancer;


// =======================================================================================
/// @brief A record of information for managing a single user session.
///

class UserSession: private Timeoutable
{
  friend UserSessionGroup; // No other class should mess with these, so everything is private
  friend UserManager; 

private:

  // Instance variables - private
  unsigned long long  theId;
  char                 sessionUser[MAX_USERNAME_LEN];
  
  // Static variables
  static Lockable idLock;
  static unsigned long long masterId;
  
  // Member functions - private
  UserSession(char* userName);
  ~UserSession(void);
  PreventAssignAndCopyConstructor(UserSession);  
};


// =======================================================================================
/// @brief A group of user sessions that can be remembered over time to maintain state.
/// 

class UserSessionGroup: private TimeoutMap<unsigned long long>
{
  friend UserManager; 
  friend HttpServThread;
  friend HttpLoadBalancer;
  
  // No other class should mess with these, so everything is private
  
private:

  // Instance variables - private
  
  // Member functions - private
  UserSessionGroup(void);
  ~UserSessionGroup(void);
  unsigned long long newSession(char* userName);
  UserSession* findSession(unsigned long long id, EntryStatus& status);
  PreventAssignAndCopyConstructor(UserSessionGroup);
};


// =======================================================================================

#endif




