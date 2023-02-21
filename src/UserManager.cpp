// Copyright Staniford Systems.  All Rights Reserved.  January 2023 -
// This class manages user records and session information.  All records about 
// permaserv users are stored in here, and access to things mediated through 
// user authentication must go through this class.  Currently the back end is
// just a file - presumably will be an SQL database some day.

#include "UserManager.h"
#include "Logging.h"
#include "HttpServThread.h"
#include "HTMLForm.h"


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


/// =======================================================================================
/// @brief Output HTML table of all user manager API options to main index page.
/// 
/// @returns True if all was well writing to the buffer.  If false, it indicates the 
/// buffer was not big enough and the output will have been truncated/incomplete.
/// @param serv A pointer to the HttpServThread managing the HTTP response.

bool UserManager::indexPageTable(HttpServThread* serv)
{
  httPrintf("<hr>\n");
  httPrintf("<center>\n");
  httPrintf("<h2>User Manager Options</h2>\n");
  unless(serv->startTable())
    return false;
  httPrintf("<tr><th>Link</th><th>notes</th></tr>\n");

  // Page to create a new Account
  httPrintf("<tr><td><a href=\"/user/createPage\">"
                 "/user/createPage</a></td>");
  httPrintf("<td>Page with form to create a new account.</td></tr>\n");
  httPrintf("<tr><td><a href=\"/user/loginPage\">"
                 "/user/loginPage</a></td>");
  httPrintf("<td>Page with form to login to an existing account.</td></tr>\n");
  
  // End table
  httPrintf("</table></center>\n");

  return true;
}


/// =======================================================================================
/// @brief Gateway to handling all requests coming in under /user/.  
/// 
/// This just routes to the appropriate private method to handle the different specific
/// cases.
/// 
/// @returns True if all was well writing to the buffer.  If false, it indicates the 
/// buffer was not big enough and the output will have been truncated/incomplete.
/// @param serv A pointer to the HttpServThread managing the HTTP response.
/// @param url A string with the balance of the request url after "/user/".

bool UserManager::processHttpRequest(HttpServThread* serv, char* url)
{
  HttpRequestParser& reqParser = serv->reqParser;
  int strlenUrl = strlen(url);
  bool retVal = false;
  
  // create account request
  if(reqParser.requestMethod == POST && strlenUrl == 6 
                                                    && strncmp(url, "create", 6) == 0)
   {
    LogPermaservOpDetails("Processing create request.\n");
    retVal = doCreate(serv, url+6);
   }

  // create account page
  else if( strlenUrl == 10 && strncmp(url, "createPage", 10) == 0)
   {
    LogPermaservOpDetails("Processing request for account creation page.\n");
    retVal = getCreatePage(serv);
   }

  // login request
  else if(reqParser.requestMethod == POST && strlenUrl == 5 
                                                    && strncmp(url, "login", 5) == 0)
   {
    LogPermaservOpDetails("Processing login request.\n");
    retVal = doLogin(serv, url+5);
   }

  // login page
  else if( strlenUrl == 9 && strncmp(url, "loginPage", 9) == 0)
   {
    LogPermaservOpDetails("Processing request for Login page.\n");
    retVal = getLoginPage(serv);
   }
  
  //Default - failure
  else
   {
    LogRequestErrors("Request for unknown /user/ resource %s\n", url);
    retVal = serv->errorPage("Resource not found");
   }

  return retVal;
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
/// @brief Process a create account request.
/// 
/// Note this could be the result of a manual creation (via UserManager::getCreatePage 
/// below) or it could be the result of a permaplan mediated creation.
/// @returns True if all was well writing to the buffer.  If false, it indicates the 
/// buffer was not big enough and the output will have been truncated/incomplete.
/// @param serv A pointer to the HttpServThread managing the HTTP response.

bool UserManager::doCreate(HttpServThread* serv, char* url)
{
  serv->errorPage("Create not implemented.");
  return true;
}


// =======================================================================================
/// @brief Return the page with the form for account creation.
/// 
/// @returns True if all was well writing to the buffer.  If false, it indicates the 
/// buffer was not big enough and the output will have been truncated/incomplete.
/// @param serv A pointer to the HttpServThread managing the HTTP response.

bool UserManager::getCreatePage(HttpServThread* serv)
{
  // Start the HTML page and the table header
  unless(serv->startResponsePage((char*)"Create an Account on Permaserv"))
    return false;
  
  // Open Login Form
  httPrintf("<center>\n");
  httPrintf("<form action=\"create\" method=\"post\">\n");
  
  // Open the main user/pass section
  httPrintf("<div class=\"container\">\n");

  // Username
  httPrintf("<label for=\"uname\"><b>Username</b></label>\n");
  httPrintf("<input type=\"text\" placeholder=\"Enter Username\" "
                                                          "name=\"uname\" required>\n");
  httPrintf("<br><br>\n");
  
  // Password
  httPrintf("<label for=\"psw1\"><b>Password</b></label>\n");
  httPrintf("<input type=\"password\" placeholder=\"Enter Password\" "
                                                          "name=\"psw1\" required>\n");
  httPrintf("<br><br>\n");

  httPrintf("<label for=\"psw2\"><b>Repeat password</b></label>\n");
  httPrintf("<input type=\"password\" placeholder=\"Repeate Password\" "
                                                          "name=\"psw2\" required>\n");
  httPrintf("<br><br>\n");
  
  // Login button
  httPrintf("<button type=\"submit\">Create Account</button>\n");

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
/// @brief Check that a password on a newly created account is sufficient.
/// 
/// @returns True if password meets the rules, false otherwise.
/// @param pwd A char* pointer to the null-terminated password string.

#define PWD_MIN_SIZE 8

bool UserManager::checkPasswordComplexity(char* pwd)
{
  bool upperPresent  = false;
  bool lowerPresent  = false;
  bool digitPresent  = false;
  bool symbolPresent = false;
  int  count          = 0;

  for(char* p = pwd; *p; p++)
   {
    count++;
    if(!upperPresent && isupper(*p))
      upperPresent = true;
    if(!lowerPresent && islower(*p))
      lowerPresent = true;
    if(!digitPresent && isdigit(*p))
      digitPresent = true;
    if(!symbolPresent)
      for(char* q = HTMLForm::allowedSymbols; *q; q++)
        if(*p == *q)
          symbolPresent = true;
   }
  
   return upperPresent && lowerPresent && digitPresent && symbolPresent 
                                                        && (count >= PWD_MIN_SIZE); 
}


// =======================================================================================
