// Copyright Staniford Systems.  All Rights Reserved.  January 2023 -

#ifndef USER_MANAGER_H
#define USER_MANAGER_H


// =======================================================================================
// Forward declarations

class HttpServThread;


// =======================================================================================
/// @brief Manages user records and session information.
///
/// All records about permaserv users are stored in here, and access to things mediated
/// through user authentication must go through this class.  Currently the back end is
/// just a file - presumably will be an SQL database some day.

class UserManager
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  UserManager(void);
  ~UserManager(void);
  bool doLogin(HttpServThread* serv);
  bool getLoginPage(HttpServThread* serv);
  static UserManager& getUserManager(void) // Get the singleton instance
   { return *theUserManager; }

private:
  
  // Private static (class) variables
  static UserManager* theUserManager;

  // Instance variables - private
  
  // Member functions - private
  /// @brief Prevent copy-construction.
  UserManager(const UserManager&);       
  /// @brief Prevent assignment.
  UserManager& operator=(const UserManager&);      
};


// =======================================================================================

#endif




