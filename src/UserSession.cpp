// Copyright Staniford Systems.  All Rights Reserved.  March 2023 -
// This class ..... is a template for real classes

#include "UserSession.h"
#include "loadFileToBuf.h"
#include "Logging.h"
#include <sys/random.h>


// =======================================================================================
// Static variables etc

unsigned long long  UserSession::masterId       = 0u;
Lockable            UserSession::idLock;


// =======================================================================================
/// @brief Constructor for a new user session.
/// 
/// @returns The length of the record, in bytes, as it will be written to disk.

UserSession::UserSession(char* userName)
{
  // Get the necessary locks
  idLock.lock();
  lock();
  
  // Set the master ID the first time
  unless(masterId)
    getentropy(&masterId, sizeof(unsigned long long));
  
  // Get a unique session ID for this session
  theId = masterId++;
  unless(theId)
    theId = masterId++; // 0 is never a valid session id
  idLock.unlock();
  
  // Record the session User name for the session
  strncpy(sessionUser, userName, MAX_USERNAME_LEN);
  
  // Check for and open the home directory for the user
  char dirName[MAX_USERNAME_LEN + 7];
  snprintf(dirName, MAX_USERNAME_LEN + 6, "users/%s", userName);
  unless(directoryExists(dirName))
   {
    if(createDirectory(dirName))
     {
      LogUserOps("Created home directory %s.\n", dirName);
     }
    else
     {
      LogUserErrors("Could not create home directory %s.\n", dirName);
     }
   }
  unless( (homeDir = opendir(dirName)) )
   {
    // Bad scene - cannot get the users home directory.  Nothing will go well for them.
    LogUserErrors("Cannot open home directory %s.\n", dirName);
   }
         
  unlock();
}


// =======================================================================================
/// @brief Destructor

UserSession::~UserSession(void)
{
  if(homeDir)
    closedir(homeDir);
}


// =======================================================================================
/// @brief Constructor

UserSessionGroup::UserSessionGroup(void):TimeoutMap<unsigned long long>(SESSION_TIMEOUT)
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

unsigned long long UserSessionGroup::newSession(char* userName)
{
  UserSession* S = new UserSession(userName);
  addEntry(S->theId, S);
  return S->theId;
}


// =======================================================================================
/// @brief Find a session based on the id
///
/// @returns A pointer to the UserSession object, which will be returned locked.  Caller
/// is responsible for unlocking the component

UserSession* UserSessionGroup::findSession(unsigned long long id, EntryStatus& status)
{
  return (UserSession*)findEntry(id, status);
}


// =======================================================================================
