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
