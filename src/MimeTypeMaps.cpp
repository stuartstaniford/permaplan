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

  insert({"html", "text/html"});
  insert({"js", "text/javascript"});
  insert({"css", "text/css"});
  insert({"gif", "image/gif"});
  insert({"jpg", "image/jpeg"});
  insert({"jpeg", "image/jpeg"});
  insert({"json", "application/json"});
  insert({"txt", "text/plain"});
}


// =======================================================================================
/// @brief Destructor

ExtensionMimeTypeMap::~ExtensionMimeTypeMap(void)
{
}


// =======================================================================================
