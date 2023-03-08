// Copyright Staniford Systems.  All Rights Reserved.  March 2023 -
// This class ..... is a template for real classes

#include "UserSession.h"
#include <sys/random.h>


// =======================================================================================
// Static variables etc

unsigned long long  UserSession::masterId       = 0u;
Lockable            UserSession::idLock;


// =======================================================================================
/// @brief Constructor for a new user session.
/// 
/// @returns The length of the record, in bytes, as it will be written to disk.

UserSession::UserSession(void)
{
  idLock.lock();
  unless(masterId)
    getentropy(&masterId, sizeof(unsigned long long));
  theId = masterId++;
  unless(theId)
    theId = masterId++; // 0 is never a valid session id
  idLock.unlock();
}


// =======================================================================================
/// @brief Destructor

UserSession::~UserSession(void)
{
}


// =======================================================================================
/// @brief Constructor

UserSessionGroup::UserSessionGroup(void)
{
}


// =======================================================================================
/// @brief Destructor

UserSessionGroup::~UserSessionGroup(void)
{
}


// =======================================================================================
/// @brief Create a new session and return the id
///
/// @returns An unsigned long long which is the session id, and can be used to find
/// the session again.

unsigned long long UserSessionGroup::newSession(void)
{
  return 1ULL;
}


// =======================================================================================
/// @brief Find a session based on the id
///
/// @returns A pointer to the UserSession object, which will be returned locked.  Caller
/// is responsible for unlocking the component

UserSession* UserSessionGroup::findSession(unsigned long long id)
{
  return NULL;
}


// =======================================================================================
