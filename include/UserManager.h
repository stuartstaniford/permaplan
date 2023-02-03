// Copyright Staniford Systems.  All Rights Reserved.  January 2023 -

#ifndef USER_MANAGER_H
#define USER_MANAGER_H


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
  
private:
  
  // Instance variables - private
  
  // Member functions - private
  /// @brief Prevent copy-construction.
  UserManager(const UserManager&);       
  /// @brief Prevent assignment.
  UserManager& operator=(const UserManager&);      
};


// =======================================================================================

#endif




