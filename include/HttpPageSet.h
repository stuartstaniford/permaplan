// Copyright Staniford Systems.  All Rights Reserved.  December 2022 -

#ifndef HTTP_PAGE_SET_H
#define HTTP_PAGE_SET_H

#include "Lockable.h"
#include <unordered_map>
#include <string>


// =======================================================================================
// Needed forward declarations

class HttpStaticPage;


// =======================================================================================
/// @brief Maps a set of paths to a set of instances of HttpStaticPage.
///
/// Used for resolving calls to get pages that will be loaded on first request and then
/// cached in memory.  We do not just serve anything found in a directory, but only
/// things explicitly coded to be served.

class HttpPageSet: public std::unordered_map<std::string, HttpStaticPage*>, public Lockable
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  HttpPageSet(void);
  ~HttpPageSet(void);
  
private:
  
  // Instance variables - private
  
  // Member functions - private
  /// @brief Prevent copy-construction.
  HttpPageSet(const HttpPageSet&);       
  /// @brief Prevent assignment.
  HttpPageSet& operator=(const HttpPageSet&);      
};


// =======================================================================================

#endif




