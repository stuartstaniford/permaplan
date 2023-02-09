// Copyright Staniford Systems.  All Rights Reserved.  December 2022 -
// Singleton class for mappping file name extensions to mimetypes. For example, 
// .html is mapped to text/html and so on

#include "MimeTypeMaps.h"


// =======================================================================================
// Static variables, etc.

ExtensionMimeTypeMap* ExtensionMimeTypeMap::theOnlyMap = NULL;


// =======================================================================================
/// @brief Constructor

ExtensionMimeTypeMap::ExtensionMimeTypeMap(void)
{
  if(theOnlyMap)
    return;
  else
    theOnlyMap = this;

  // Text types
  insert({"html", "text/html"});
  insert({"js", "text/javascript"});
  insert({"css", "text/css"});
  insert({"txt", "text/plain"});

  // Image types
  insert({"gif", "image/gif"});
  insert({"jpg", "image/jpeg"});
  insert({"jpeg", "image/jpeg"});
  
  // Application types
  insert({"json", "application/json"});
}


// =======================================================================================
/// @brief Destructor

ExtensionMimeTypeMap::~ExtensionMimeTypeMap(void)
{
}


// =======================================================================================
