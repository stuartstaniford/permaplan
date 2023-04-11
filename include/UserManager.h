// Copyright Staniford Systems.  All Rights Reserved.  January 2023 -

#ifndef USER_MANAGER_H
#define USER_MANAGER_H

#include "Global.h"
#include "CryptoAlgorithms.h"
#include "Timeval.h"
#include "Lockable.h"
#include <string>
#include <unordered_map>
#include <stdio.h>
#include <TimeoutMap.h>


// =======================================================================================
// Forward declarations

class HttpServThread;
class HTMLForm;
class UserManager;
class UserSessionGroup;
class PmodDesign;


// =======================================================================================
/// @brief A record for a single user.
/// @todo There is currently no locking on user records, which is asking for trouble.

class UserRecord
{
  friend HttpServThread;
  friend UserManager; // No other classes should mess with these, so everything is private
  
  private:
  
  // Instance variables - private
  PasswordSalt  salt;
  PasswordHash  pwdHash;
  Timeval       creation;
  Timeval       pwdLastChange;
  bool          fileReadOk;   // constructors can't return NULL
  std::string   userName;
  PmodDesign*   design;
  
  // Static functions - private
  static int userNameLength(char* textLen);
  
  // Member functions - private
  UserRecord(char* uname, char* pwd);
  UserRecord(FILE* file, unsigned userNameLen);
  bool writeFile(FILE* file);
  bool checkPassword(char* pwd);
  void setPassword(char* pwd);
  int diskLength(void);
  PreventAssignAndCopyConstructor(UserRecord);
};


// =======================================================================================
/// @brief Manages user records and session information.
///
/// All records about permaserv users are stored in here, and access to things mediated
/// through user authentication must go through this class.  Currently the back end is
/// just a file - presumably will be an SQL database some day.

class UserManager: private std::unordered_map<std::string, UserRecord*>
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  UserManager(void);
  ~UserManager(void);
  bool indexPageTable(HttpServThread* serv);
  bool processHttpRequest(HttpServThread* serv, char* url, UserSessionGroup* sessions);
  static UserManager& getUserManager(void) // Get the singleton instance
   { return *theUserManager; }
  UserRecord* getRecord(unsigned long long sessionId, EntryStatus& sessionStatus,   
                        UserSessionGroup* sessions);

private:
  
  // Private static (class) variables
  static UserManager* theUserManager;

  // Instance variables - private
  
  // Member functions - private
    
  // Login
  bool doLogin(HttpServThread* serv, HTMLForm* form, UserSessionGroup* sessions);
  bool getLoginPage(HttpServThread* serv);
  bool addLoginFormToPage(HttpServThread* serv);
  bool sessionExpiredPage(HttpServThread* serv);
  bool doubleLoginPage(HttpServThread* serv);

  // Account creation
  bool getCreatePage(HttpServThread* serv);
  bool doCreate(HttpServThread* serv, HTMLForm* form);
  bool getCreateErrorPage(HttpServThread* serv);
  bool checkPasswordComplexity(char* pwd);
  bool checkUsername(char* uname);

  // Change password
  bool getChangePasswordPage(HttpServThread* serv);
  bool doChangePassword(HttpServThread* serv, HTMLForm* form);

  // User management information
  bool getUserListPage(HttpServThread* serv);

  // Misc
  bool writeFile(void);
  bool readFile(void);
  PreventAssignAndCopyConstructor(UserManager);
};


// =======================================================================================

#endif




