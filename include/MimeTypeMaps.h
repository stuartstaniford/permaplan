// Copyright Staniford Systems.  All Rights Reserved.  December 2022 -

#ifndef MIME_TYPE_MAPS_H
#define MIME_TYPE_MAPS_H

#include <unordered_map>
#include <string>


// =======================================================================================
/// @brief Enum of MimeTypes we know about. 

enum MimeType
{
  NoMimeType,

  // Text types
  TextHtml,
  TextJavascript,
  TextCss,
  TextPlain,
  
  // Image types
  ImageGif,
  ImageJpeg,
  
  // Application types
  ApplicationJson
};


// =======================================================================================
/// @brief Singleton class for mappping text mimetypes to enum MimeType. 

class MimeTypeMap: public std::unordered_map<std::string, MimeType>
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  MimeTypeMap(void);
  ~MimeTypeMap(void);
  static MimeTypeMap& getMap(void) {return *theOnlyMap;} // Get singleton instance

private:
  
  // Private static (class) variables
  static MimeTypeMap* theOnlyMap;

  // Instance variables - private
  
  // Member functions - private
  /// @brief Prevent copy-construction.
  MimeTypeMap(const MimeTypeMap&);       
  /// @brief Prevent assignment.
  MimeTypeMap& operator=(const MimeTypeMap&);      
};


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
  static ExtensionMimeTypeMap& getMap(void) {return *theOnlyMap;} // Get singleton instance

private:
  
  // Private static (class) variables
  static ExtensionMimeTypeMap* theOnlyMap;

  // Instance variables - private
  
  // Member functions - private
  /// @brief Prevent copy-construction.
  ExtensionMimeTypeMap(const ExtensionMimeTypeMap&);       
  /// @brief Prevent assignment.
  ExtensionMimeTypeMap& operator=(const ExtensionMimeTypeMap&);      
};


// =======================================================================================

#endif




