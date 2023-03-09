// Copyright Staniford Systems.  All Rights Reserved.  January 2023 -
// This class manages user records and session information.  All records about 
// permaserv users are stored in here, and access to things mediated through 
// user authentication must go through this class.  Currently the back end is
// just a file - presumably will be an SQL database some day.

#include "UserManager.h"
#include "Logging.h"
#include "HttpServThread.h"
#include "HTMLForm.h"
#include "CryptoAlgorithms.h"
#include "UserSession.h"
#include <assert.h>


// =======================================================================================
// Static variables, etc.

UserManager*        UserManager::theUserManager = NULL;
char*               userFileName                = (char*)"userdb";


// =======================================================================================
/// @brief Constructor from a newly supplier user/password combination.
///
/// @param uname A char* pointer to the username for the new user record
/// @param pwd A char* pointer to the cleartext password for the new user record.  Note
/// that this will be zero-ed out in the course of setting up the record - only the hash
/// of the password is stored persistently in memory or on disk.

UserRecord::UserRecord(char* uname, char* pwd): pwdHash(pwd, salt), userName(uname)
{
  creation.now();
  pwdLastChange.now();
}


// =======================================================================================
/// @brief Constructor while reading from a file.
///
/// @param file The stdio.h FILE* pointer to the open file.
/// @todo Annoying C++ ism forces us to do an extra copy - is this avoidable somehow?

UserRecord::UserRecord(FILE* file, unsigned userNameLen): salt(file), 
                                                          pwdHash(file),
                                                          creation(file),
                                                          pwdLastChange(file),
                                                          fileReadOk(false)
{
  char buf[userNameLen + 1];
  buf[userNameLen] = '\0';
  unless(fread(buf, sizeof(char), userNameLen, file) == userNameLen)
   err(-1, "Broken record format in user file %s.\n", userFileName);
  userName = buf; // extra copy
  unless(fseek(file, 2, SEEK_CUR) == 0) // read past the \r\n
    err(-1, "Couldn't seek past record end in file %s.\n", userFileName);
  fileReadOk = true;
  LogUserDetails("Read record for %s of length %d from file %s.\n", userName.c_str(),
                                                                userNameLen, userFileName);
}


// =======================================================================================
/// @brief Writes this user record to disk.
/// 
/// The format on disk is as follows:
/// * Three ASCII digits for the total length of this record (including the final \r\n)
/// * The password salt (in binary)
/// * The password hash (in binary)
/// * The creation time (binary Timeval)
/// * The last password change time (binary Timeval)
/// * The username
/// * \r\n
/// 
/// @returns True if successfully written, false otherwise.
/// @param file The stdio.h FILE* pointer to the open file.

bool UserRecord::writeFile(FILE* file)
{  
  fprintf(file, "%03d", diskLength());
  salt.outputToFile(file);
  pwdHash.outputToFile(file);
  creation.writeBinaryToDisk(file);
  pwdLastChange.writeBinaryToDisk(file);
  fprintf(file, "%s\r\n", userName.c_str());
  return true;
}


// =======================================================================================
/// @brief Compute the length of this record on disk.
/// 
/// @returns The length of the record, in bytes, as it will be written to disk.

int UserRecord::diskLength(void)
{

  return (PasswordSalt::diskLength() + PasswordHash::diskLength() +
                                      2*sizeof(Timeval) + userName.length() + 5);
}


// =======================================================================================
/// @brief Compute the username length of this record on disk from the text length.
/// 
/// @returns The length of the record, in bytes, as it will be written to disk.

int UserRecord::userNameLength(char* textLen)
{

  return atoi(textLen) - PasswordSalt::diskLength() - PasswordHash::diskLength() 
                                                                - 2*sizeof(Timeval) - 5;
}


// =======================================================================================
/// @brief Verify if the password matches the hash.
/// 
/// After checking, the supplied string will be zero'd out.
/// @param pwd A pointer to the C-string with the password attempt.
/// @returns True if the password matches, false otherwise.

bool UserRecord::checkPassword(char* pwd)
{
  return pwdHash.checkMatch(pwd, salt);
}


// =======================================================================================
/// @brief Constructor

UserManager::UserManager(void)
{
  if(theUserManager)
    return;
  else
    theUserManager = this;
  unless(readFile())
    err(-1, "Couldn't read user file %s.\n", userFileName);
}


// =======================================================================================
/// @brief Destructor

UserManager::~UserManager(void)
{
}


// =======================================================================================
/// @brief Writes the user database to disk.
///
/// @returns True if successfully written, false otherwise.

bool UserManager::writeFile(void)
{
  FILE* file = fopen(userFileName, "w");
  
  for (auto& iter: *this) 
   {
    iter.second->writeFile(file);
   }

  fclose(file);
  return true;
}


// =======================================================================================
/// @brief Reads the user database from disk.  
/// 
/// NB.  Currently only called on startup.
/// @returns True if successfully read, false otherwise.

bool UserManager::readFile(void)
{
  char textLen[4];
  textLen[3] = '\0';
  int userNameLen;

  FILE* file = fopen(userFileName, "r");
  unless(file)
   {
    LogUserErrors("No file %s found, treating as new system.\n", userFileName);
    return true;
   }  
  
  int line = 1;
  while(1)
   {
    unless(fread(textLen, sizeof(char), 3, file) == 3)
      break;
    userNameLen = UserRecord::userNameLength(textLen);
    assert(userNameLen > 0);
    UserRecord* ur = new UserRecord(file, userNameLen);
    unless(ur->fileReadOk)
      err(-1, "User file %s corrupted at line %d.\n", userFileName, line);
    insert({ur->userName, ur});
    line++;
   }
  fclose(file);
  return true;
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
  
  // Login
  httPrintf("<tr><td><a href=\"/user/loginPage\">"
                 "/user/loginPage</a></td>");
  httPrintf("<td>Page with form to login to an existing account.</td></tr>\n");

  // Change Password
  httPrintf("<tr><td><a href=\"/user/changePasswordPage\">"
                 "/user/changePasswordPage</a></td>");
  httPrintf("<td>Page with form to change a password while logged in.</td></tr>\n");
  
  // List the users
  httPrintf("<tr><td><a href=\"/user/userList\">"
                 "/user/userList</a></td>");
  httPrintf("<td>List of user accounts with summary information.</td></tr>\n");
  
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

bool UserManager::processHttpRequest(HttpServThread* serv, char* url, 
                                                                UserSessionGroup* sessions)
{
  HttpRequestParser& reqParser = serv->reqParser;
  int strlenUrl = strlen(url);
  bool retVal = false;

  // change password page
  if(strlenUrl == 18 && strncmp(url, "changePasswordPage", 18) == 0)
   {
    LogPermaservOpDetails("Processing request for password change page.\n");
    retVal = getChangePasswordPage(serv);
   }

  // change password request
  if(strlenUrl == 14 && strncmp(url, "changePassword", 14) == 0)
   {
    LogPermaservOpDetails("Processing request to change password.\n");
    retVal = doChangePassword(serv, (HTMLForm*)reqParser.parsedBody);
   }
  
  // create account request
  else if(reqParser.requestMethod == POST && strlenUrl == 6 
                                                    && strncmp(url, "create", 6) == 0)
   {
    LogPermaservOpDetails("Processing create request.\n");
    retVal = doCreate(serv, (HTMLForm*)reqParser.parsedBody);
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
    retVal = doLogin(serv, (HTMLForm*)reqParser.parsedBody, sessions);
   }

  // login page
  else if( strlenUrl == 9 && strncmp(url, "loginPage", 9) == 0)
   {
    LogPermaservOpDetails("Processing request for Login page.\n");
    retVal = getLoginPage(serv);
   }
  
  // user list page
  else if( strlenUrl == 8 && strncmp(url, "userList", 8) == 0)
   {
    LogPermaservOpDetails("Processing request for user listing page.\n");
    retVal = getUserListPage(serv);
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
/// Also note this operation should be the only way to get a new session-id.
/// @returns True if all was well writing to the buffer.  If false, it indicates the 
/// buffer was not big enough and the output will have been truncated/incomplete.
/// @param serv A pointer to the HttpServThread managing the HTTP response.
/// @param form A pointer to the HTML form object from parsing the request body.

bool UserManager::doLogin(HttpServThread* serv, HTMLForm* form, UserSessionGroup* sessions)
{
  // Sanity check the form
  if(!form || form->getDynamicType() != TypeHTMLForm)
   {
    LogRequestErrors("Bad form in login Request.\n");
    return serv->errorPage("Login Error.");
   }
  
  //Make sure we have a username
  unless(form->count("uname"))
   {
    LogUserErrors("No username in login Request.\n");
    return serv->errorPage("Login Error.");
   }
  
  // Make sure we have a password
  unless(form->count("psw"))
   {
    LogUserErrors("No password in login Request.\n");
    return serv->errorPage("Login Error.");
   }
  
  // Check that the username isn't too long.
  unless(strlen((*form)["uname"]) < MAX_USERNAME_LEN)
   {
    (*form)["uname"][MAX_USERNAME_LEN - 1] = '\0';
    LogUserErrors("Username starting with %s exceeds max length of %d.\n", 
                  (*form)["uname"], MAX_USERNAME_LEN - 1);
    return serv->errorPage("Login Error.");
   }

  // Check that the username exists.
  unless(count((*form)["uname"]))
   {      
    LogUserErrors("Attempted login on unknown username %s.\n", (*form)["uname"]);
    return serv->errorPage("Login Error.");
   }
  
  // Get the user record
  UserRecord* ur = (*this)[(*form)["uname"]];
  
  // Check the password hash matches and then zero the password string out
  if(ur->checkPassword((*form)["psw"]))
   {
    LogUserOps("Successful login by user %s.\n", (*form)["uname"]);
   }
  else
   {
    LogUserErrors("Failed login by user %s.\n", (*form)["uname"]);
    return serv->errorPage("Login Error.");
   }
  
  // Generate a session-id for this login, and tell the servThread about it.
  unsigned long long sessionId = sessions->newSession((*form)["uname"]);
  serv->cookies.setSessionId(sessionId);
  
  return true;
}


// =======================================================================================
/// @brief Creates the form for manual login (which is used by several pages).
/// 
/// @returns True if all was well writing to the buffer.  If false, it indicates the 
/// buffer was not big enough and the output will have been truncated/incomplete.
/// @param serv A pointer to the HttpServThread managing the HTTP response.

bool UserManager::addLoginFormToPage(HttpServThread* serv)
{  
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

  return true;
}


// =======================================================================================
/// @brief Return the main page with the form for manual login.
/// 
/// @returns True if all was well writing to the buffer.  If false, it indicates the 
/// buffer was not big enough and the output will have been truncated/incomplete.
/// @param serv A pointer to the HttpServThread managing the HTTP response.

bool UserManager::getLoginPage(HttpServThread* serv)
{
  // Start the HTML page and the table header
  unless(serv->startResponsePage((char*)"Login to Permaserv"))
    return false;
  
  unless(addLoginFormToPage(serv))
    return false;
  
  // Finish up the page
  unless(serv->endResponsePage())
    return false;

  return true;
}


// =======================================================================================
/// @brief Return the page used when a previously successful session login has expired.
/// 
/// @returns True if all was well writing to the buffer.  If false, it indicates the 
/// buffer was not big enough and the output will have been truncated/incomplete.
/// @param serv A pointer to the HttpServThread managing the HTTP response.

bool UserManager::sessionExpiredPage(HttpServThread* serv)
{
  // Start the HTML page and the table header
  unless(serv->startResponsePage((char*)"Session Expired"))
    return false;
  
  httPrintf("<p>Your session has expired.  Login again to resume work.");
  
  unless(addLoginFormToPage(serv))
    return false;
  
  // Finish up the page
  unless(serv->endResponsePage())
    return false;

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
  httPrintf("<input type=\"password\" placeholder=\"Repeat Password\" "
                                                          "name=\"psw2\" required>\n");
  httPrintf("<br><br>\n");
  
  // Action button
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
/// @brief Return a page on errors in account creation.
/// 
/// This page exists to give the user some hints as to what might have gone wrong, while
/// not offering too much specific help to bad guys.
/// @returns True if all was well writing to the buffer.  If false, it indicates the 
/// buffer was not big enough and the output will have been truncated/incomplete.
/// @param serv A pointer to the HttpServThread managing the HTTP response.

bool UserManager::getCreateErrorPage(HttpServThread* serv)
{
  // Start the HTML page and the table header
  unless(serv->startResponsePage((char*)"Account Creation Error on Permaserv"))
    return false;
  
  httPrintf("We were not able to create the account as requested.<br><br>\n");
  httPrintf("Please hit the back button on your browser and try again.<br><br>\n");
  httPrintf("Some hints:<br><ul>\n");

  httPrintf("<li>Usernames must be unique from all other usernames.\n");
  httPrintf("<li>Usernames must start with a letter, and may only include letters, ");
  httPrintf("numbers, and the underscore character \"_\".");
  httPrintf("<li>Usernames are not case sensitive.");
  httPrintf("<li>Usernames cannot be more than %d characters in length.\n",
                                                                  MAX_USERNAME_LEN - 1);
  httPrintf("<li>Passwords <b>are</b> case sensitive and must have at least eight ");
  httPrintf("characters.");
  httPrintf("<li>Passwords must contain at least one upper-case and one lower-case letter,");
  httPrintf(" at least one number, and at least one symbol character from %s", 
                                                                  HTMLForm::allowedSymbols);
  httPrintf("</ul>\n");

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

bool UserManager::doCreate(HttpServThread* serv, HTMLForm* form)
{
  if(!form || form->getDynamicType() != TypeHTMLForm)
   {
    LogRequestErrors("Bad form in create Request.\n");
    return getCreateErrorPage(serv);
   }
  
  //Make sure we have a username
  unless(form->count("uname"))
   {
    LogUserErrors("No username in create Request.\n");
    return getCreateErrorPage(serv);    
   }
  
  // Check the username is allowed
  unless(checkUsername((*form)["uname"]))
   {
    LogUserErrors("Invalid username in create Request.\n");
    return getCreateErrorPage(serv);    
   }

  //Make sure we have a first password
  unless(form->count("psw1"))
   {
    LogUserErrors("No first password in create Request.\n");
    return getCreateErrorPage(serv);    
   }
  
  // Check the password is valid
  unless(checkPasswordComplexity((*form)["psw1"]))
   {
    LogUserErrors("Invalid password in create Request.\n");
    return getCreateErrorPage(serv);    
   }
  
  // Make sure we have a second password
  unless(form->count("psw2"))
   {
    LogUserErrors("No second password in create Request.\n");
    return getCreateErrorPage(serv);    
   }

  // Check the second password matches the first
  unless(strcmp((*form)["psw1"], (*form)["psw2"]) == 0)
   {
    LogUserErrors("Passwords don't match in create Request.\n");
    return getCreateErrorPage(serv);    
   }
  
  // Delete the second password from memory as we no longer need it
  bzero((*form)["psw2"], strlen((*form)["psw2"]));

  // Ok, all is well, create the account
  UserRecord* urec = new UserRecord((*form)["uname"], (*form)["psw1"]);
  insert({(*form)["uname"], urec});
  LogUserOps("Created account for username %s.\n", (*form)["uname"]);

  // Rewrite the whole file to disk.  Note this would get inefficient if the
  // file was large, but by then we should move to a database implementation
  // for the user table in any case.
  writeFile();
  
  return true;
}


// =======================================================================================
/// @brief Check that a username on a newly created account is valid and doesnt already
/// exist.
/// 
/// Usernames may only have alphanumeric characters plus "_", must start with an 
/// alphabet character, and are not case sensitive.
/// 
/// @returns True if username meets the rules, and doesn't already exist, false otherwise.
/// @param uname A char* pointer to the null-terminated username string.

bool UserManager::checkUsername(char* uname)
{
  unless(strlen(uname) < MAX_USERNAME_LEN)
   {
    uname[MAX_USERNAME_LEN - 1] = '\0';
    LogUserErrors("Username starting with %s exceeds max length of %d.\n", 
                  uname, MAX_USERNAME_LEN - 1);
    return false;
   }
  
  // First character must be alpha.  We convert to lowercase.
  unless(isalpha(*uname))
   {
    LogUserErrors("Username %s does not start with alphabet character.\n", uname);
    return false;
   }
  *uname = tolower(*uname);
  
  // Subsequent characters must be alphanumeric or '_'.  We convert to lower if nec.
  for(char* p = uname+1; *p; p++)
   {
    unless(isalnum(*p) || *p == '_')
     {
      LogUserErrors("Username %s has illegal characters.\n", uname);
      return false;
     }
    if(isupper(*p))
      *p = tolower(*p);
   }
  
  // Check if the username is already in use.
  if(count(uname))
   {      
    LogUserErrors("Username %s is already in use.\n", uname);
    return false;
   }
  
  return true; 
}


// =======================================================================================
/// @brief Check that a password on a newly created account is sufficient.
/// 
/// Note we earlier checked that only allowable characters for form values are in here,
/// and passwords allow all such characters.  Also note that we don't log password
/// related errors in much detail as we don't want password text sitting in the logs.
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
/// @brief Return the page with the form for changing the password.
/// 
/// @returns True if all was well writing to the buffer.  If false, it indicates the 
/// buffer was not big enough and the output will have been truncated/incomplete.
/// @param serv A pointer to the HttpServThread managing the HTTP response.

bool UserManager::getChangePasswordPage(HttpServThread* serv)
{
  // Start the HTML page and the table header
  unless(serv->startResponsePage((char*)"Change Password on Permaserv"))
    return false;
  
  // Open Form
  httPrintf("<center>\n");
  httPrintf("<form action=\"changePassword\" method=\"post\">\n");
  
  // Open the main user/pass section
  httPrintf("<div class=\"container\">\n");

  // Old Password
  httPrintf("<label for=\"oldpsw\"><b>Current Password</b></label>\n");
  httPrintf("<input type=\"password\" placeholder=\"Enter Password\" "
                                                          "name=\"psw1\" required>\n");
  httPrintf("<br><br>\n");

  // New Password
  httPrintf("<label for=\"psw1\"><b>Enter New Password</b></label>\n");
  httPrintf("<input type=\"password\" placeholder=\"New Password\" "
                                                          "name=\"psw1\" required>\n");
  httPrintf("<br><br>\n");

  // Repeat new pwd
  httPrintf("<label for=\"psw2\"><b>Repeat new password</b></label>\n");
  httPrintf("<input type=\"password\" placeholder=\"Repeat Password\" "
                                                          "name=\"psw2\" required>\n");
  httPrintf("<br><br>\n");
  
  // Action button
  httPrintf("<button type=\"submit\">Be the Change!</button>\n");

  // End pass section
  httPrintf("</div>\n");
  
  // End the form
  httPrintf("</form></center>\n");

  // Finish up the page
  unless(serv->endResponsePage())
    return false;

  return true;
}


// =======================================================================================
/// @brief Process a password change request.
/// 
/// Note this could be the result of a manual request, or it could be the result 
/// of a permaplan mediated request.
/// @returns True if all was well writing to the buffer.  If false, it indicates the 
/// buffer was not big enough and the output will have been truncated/incomplete.
/// @param serv A pointer to the HttpServThread managing the HTTP response.
/// @param form A pointer to the HTML form object from parsing the request body.

bool UserManager::doChangePassword(HttpServThread* serv, HTMLForm* form)
{
  // Sanity check the form
  if(!form || form->getDynamicType() != TypeHTMLForm)
   {
    LogRequestErrors("Bad form in change password Request.\n");
    return serv->errorPage("Change Password Error.");
   }
  
  //Get the username from the session-id
  /*unless(form->count("uname"))
   {
    LogUserErrors("No username in login Request.\n");
    return serv->errorPage("Change Password Error.");
   }*/
  
  // Get the user record
  UserRecord* ur = (*this)[(*form)["uname"]];

  // Make sure we have the old password
  unless(form->count("oldpsw"))
   {
    LogUserErrors("No password in change password Request.\n");
    return serv->errorPage("Change Password Error.");
   }
  
  // Make sure we have a second password
  
  // Make sure the two passwords match
  
  // UP TO HERE

  // Check the password hash matches and then zero the password string out
  if(ur->checkPassword((*form)["psw"]))
   {
    LogUserOps("Successful login by user %s.\n", (*form)["uname"]);
   }
  else
   {
    LogUserErrors("Failed login by user %s.\n", (*form)["uname"]);
    return serv->errorPage("Change Password Error.");
   }
    
  return true;
}


/// =======================================================================================
/// @brief Output HTML table of all users.
/// 
/// @returns True if all was well writing to the buffer.  If false, it indicates the 
/// buffer was not big enough and the output will have been truncated/incomplete.
/// @param serv A pointer to the HttpServThread managing the HTTP response.

bool UserManager::getUserListPage(HttpServThread* serv)
{
  // Start the HTML page and the table header
  unless(serv->startResponsePage((char*)"List of Permaserv User Accounts"))
    return false;
  httPrintf("<center>\n");
  unless(serv->startTable())
    return false;
  httPrintf("<tr><th>UserName</th><th>Creation Date</th>"
                                                      "<th>Password Changed</th></tr>\n");

  // Loop over the user accounts  
  for (auto& iter: *this) 
   {
    // Username
    httPrintf("<tr><td>%s</td>", iter.first.c_str());

    // Creation Date
    char dateBuf[16];
    iter.second->creation.ctimeDateOnly(dateBuf);
    httPrintf("<td>%s</td>", dateBuf);
    
    // Password Change Date
    iter.second->pwdLastChange.ctimeDateOnly(dateBuf);
    httPrintf("<td>%s</td>", dateBuf);    
    
    // End the row and loop around
    httPrintf("</tr>");
   }

  // End table
  httPrintf("</table></center>\n");

  // Finish up the page
  unless(serv->endResponsePage())
    return false;

  return true;
}


// =======================================================================================
