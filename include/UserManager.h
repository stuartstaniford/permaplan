// Copyright Staniford Systems.  All Rights Reserved.  January 2023 -

#ifndef USER_MANAGER_H
#define USER_MANAGER_H

#include "Global.h"
#include "CryptoAlgorithms.h"
#include <string>
#include <unordered_map>


// =======================================================================================
// Forward declarations

class HttpServThread;
class HTMLForm;


// =======================================================================================
/// @brief A record for a single user.
///

class UserRecord
{
  public: 
  
  // Member functions - public
  UserRecord(char* uname, char* pwd);

  private:
  
  // Instance variables - private
  std::string   userName;
  PasswordSalt  salt;
  PasswordHash  pwdHash;
};


// =======================================================================================
/// @brief Manages user records and session information.
///
/// All records about permaserv users are stored in here, and access to things mediated
/// through user authentication must go through this class.  Currently the back end is
/// just a file - presumably will be an SQL database some day.

class UserManager: public std::unordered_map<std::string, UserRecord*>
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  UserManager(void);
  ~UserManager(void);
  bool indexPageTable(HttpServThread* serv);
  bool processHttpRequest(HttpServThread* serv, char* url);
  static UserManager& getUserManager(void) // Get the singleton instance
   { return *theUserManager; }

private:
  
  // Private static (class) variables
  static UserManager* theUserManager;

  // Instance variables - private
  
  // Member functions - private
  
  // Login
  bool doLogin(HttpServThread* serv, HTMLForm* form);
  bool getLoginPage(HttpServThread* serv);

  // Account creation
  bool getCreatePage(HttpServThread* serv);
  bool doCreate(HttpServThread* serv, HTMLForm* form);
  bool getCreateErrorPage(HttpServThread* serv);
  bool checkPasswordComplexity(char* pwd);
  bool checkUsername(char* uname);

  // Misc
  bool writeFile(void);
  bool readFile(void);
  PreventAssignAndCopyConstructor(UserManager);
};


// =======================================================================================

#endif




