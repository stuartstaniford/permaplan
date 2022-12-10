// Copyright Staniford Systems.  All Rights Reserved.  December 2022 -

#ifndef EMPTY_CLASS_H
#define EMPTY_CLASS_H

#include <unordered_map>
#include <string>


// =======================================================================================
/// @brief Singleton class for mappping file name extensions to mimetypes. 
///
/// For example, .html is mapped to text/html and so on.

class ExtensionMimeTypeMap: public std::unordered_map<std::string, std::string>
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  ExtensionMimeTypeMap(void);
  ~ExtensionMimeTypeMap(void);
  
private:
  
  // Instance variables - private
  
  // Member functions - private
  /// @brief Prevent copy-construction.
  ExtensionMimeTypeMap(const ExtensionMimeTypeMap&);       
  /// @brief Prevent assignment.
  ExtensionMimeTypeMap& operator=(const ExtensionMimeTypeMap&);      
};


// =======================================================================================

#endif




