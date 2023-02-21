// Copyright Staniford Systems.  All Rights Reserved.  January 2023 -

#ifndef USER_MANAGER_H
#define USER_MANAGER_H

#include <string>
#include <unordered_map>

#define HASH_BYTES 16
#define SALT_BYTES 16


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
  
  std::string   userName;
  unsigned char salt[SALT_BYTES];
  unsigned char passwordHash[HASH_BYTES];
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
  bool doLogin(HttpServThread* serv, HTMLForm* form);
  bool doCreate(HttpServThread* serv, HTMLForm* form);
  bool getLoginPage(HttpServThread* serv);
  bool getCreatePage(HttpServThread* serv);
  bool checkPasswordComplexity(char* pwd);
  bool checkUsername(char* uname);
  /// @brief Prevent copy-construction.
  UserManager(const UserManager&);       
  /// @brief Prevent assignment.
  UserManager& operator=(const UserManager&);      
};


// =======================================================================================

#endif




