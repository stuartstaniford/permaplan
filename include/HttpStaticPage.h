// Copyright Staniford Systems.  All Rights Reserved.  December 2022 -

#ifndef HTTP_STATIC_PAGE_H
#define HTTP_STATIC_PAGE_H


// =======================================================================================
// Forward declarations

class HttpServThread;
class HttpPageSet;


// =======================================================================================
/// @brief This class is for the storage of static objects that will be served via the 
/// HTTP servers.
///
/// On construction, we will load our object from disk, but keep it around so we can
/// be served as needed.

class HttpStaticPage
{
  friend HttpServThread;
  friend HttpPageSet;
  
public:
  
  // Instance variables - public
  
  // Member functions - public
  HttpStaticPage(char* objectPath);
  ~HttpStaticPage(void);
  char* getResponse(void);
  
private:
  
  // Instance variables - private
  unsigned            respBufSize;
  unsigned            headBufSize;
  char*               respBuf;
  char*               headBuf;
  char*               originalPath;
  
  // Member functions - private
  /// @brief Prevent copy-construction.
  HttpStaticPage(const HttpStaticPage&);       
  /// @brief Prevent assignment.
  HttpStaticPage& operator=(const HttpStaticPage&);      
};


// =======================================================================================

#endif




