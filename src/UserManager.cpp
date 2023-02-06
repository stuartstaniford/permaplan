// Copyright Staniford Systems.  All Rights Reserved.  January 2023 -
// This class manages user records and session information.  All records about 
// permaserv users are stored in here, and access to things mediated through 
// user authentication must go through this class.  Currently the back end is
// just a file - presumably will be an SQL database some day.

#include "UserManager.h"
#include "Logging.h"
#include "HttpServThread.h"


// =======================================================================================
// Static variables, etc.

UserManager* UserManager::theUserManager = NULL;


// =======================================================================================
/// @brief Constructor

UserManager::UserManager(void)
{
  if(theUserManager)
    return;
  else
    theUserManager = this;
}


// =======================================================================================
/// @brief Destructor

UserManager::~UserManager(void)
{
}


// =======================================================================================
/// @brief Return the page with the form for logging in

bool UserManager::getLoginPage(HttpServThread* serv)
{
  return false;
}


// =======================================================================================
