// Copyright Staniford Systems.  All Rights Reserved.  December 2022 -

#ifndef EMPTY_CLASS_H
#define EMPTY_CLASS_H


// =======================================================================================
/// @brief This class is for the storage of static objects that will be served via the 
/// HTTP servers.
///
/// On construction, we will load our object from disk, but keep it around so we can
/// be served as needed.

class HttpStaticPage
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  HttpStaticPage(void);
  ~HttpStaticPage(void);
  
private:
  
  // Instance variables - private
  
  // Member functions - private
  /// @brief Prevent copy-construction.
  HttpStaticPage(const HttpStaticPage&);       
  /// @brief Prevent assignment.
  HttpStaticPage& operator=(const HttpStaticPage&);      
};


// =======================================================================================

#endif




