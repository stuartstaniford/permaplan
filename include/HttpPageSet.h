// Copyright Staniford Systems.  All Rights Reserved.  December 2022 -

#ifndef HTTP_PAGE_SET_H
#define HTTP_PAGE_SET_H

#include "Lockable.h"
#include <unordered_map>
#include <string>

#define STAT_URL_BUF_SIZE 256

// =======================================================================================
// Needed forward declarations

class HttpStaticPage;
class HttpServThread;


// =======================================================================================
/// @brief Maps a set of paths to a set of instances of HttpStaticPage.
///
/// Used for resolving calls to get pages that will be loaded on first request and then
/// cached in memory.  We do not just serve anything found in a directory, but only
/// things explicitly coded to be served.  Inherits from and unordered_map from
/// the paths we should serve to the HttpStaticPage instances that should be served
/// for that path.  Also inherits from Lockable to mediate access from multiple server
/// threads.

class HttpPageSet: public std::unordered_map<std::string, HttpStaticPage*>, public Lockable
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  HttpPageSet(char* path);
  ~HttpPageSet(void);
  bool processPageRequest(HttpServThread* serv, char* url);
  
private:
  
  // Instance variables - private
  char urlBuf[STAT_URL_BUF_SIZE];
  char* urlBufPtr;
  
  // Member functions - private
  /// @brief Prevent copy-construction.
  HttpPageSet(const HttpPageSet&);       
  /// @brief Prevent assignment.
  HttpPageSet& operator=(const HttpPageSet&);      
};


// =======================================================================================

#endif




