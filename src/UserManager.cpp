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
/// @brief Process a login request.
/// 
/// Note this could be the result of a manual login (via UserManager::getLoginPage below)
/// or it could be the result of a permaplan mediated login.
/// @returns True if all was well writing to the buffer.  If false, it indicates the 
/// buffer was not big enough and the output will have been truncated/incomplete.
/// @param serv A pointer to the HttpServThread managing the HTTP response.

bool UserManager::doLogin(HttpServThread* serv, char* url)
{
  serv->errorPage("Login not implemented.");
  return true;
}


// =======================================================================================
/// @brief Return the page with the form for manual login.
/// 
/// @returns True if all was well writing to the buffer.  If false, it indicates the 
/// buffer was not big enough and the output will have been truncated/incomplete.
/// @param serv A pointer to the HttpServThread managing the HTTP response.

bool UserManager::getLoginPage(HttpServThread* serv)
{
  // Start the HTML page and the table header
  unless(serv->startResponsePage((char*)"Login to Permaserv"))
    return false;
  
  // Open Login Form
  httPrintf("<center>\n");
  httPrintf("<form action=\"login\" method=\"post\">\n");
  
  // Open the main user/pass section
  httPrintf("<div class=\"container\">\n");

  // Username
  httPrintf("<label for=\"uname\"><b>Username</b></label>\n");
  httPrintf("<input type=\"text\" placeholder=\"Enter Username\" "
                                                          "name=\"uname\" required>\n");
  httPrintf("<br><br>\n");
  
  // Password
  httPrintf("<label for=\"psw\"><b>Password</b></label>\n");
  httPrintf("<input type=\"password\" placeholder=\"Enter Password\" "
                                                          "name=\"psw\" required>\n");
  httPrintf("<br><br>\n");
  
  // Login button
  httPrintf("<button type=\"submit\">Login</button>\n");

  // End user/pass section
  httPrintf("</div>\n");
  
  // End the form
  httPrintf("</form></center>\n");

  // Finish up the page
  unless(serv->endResponsePage())
    return false;

  return true;
}


// =======================================================================================
