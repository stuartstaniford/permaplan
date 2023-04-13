// Copyright Staniford Systems.  All Rights Reserved.  December 2022 -
// Singleton class for mappping file name extensions to mimetypes. For example, 
// .html is mapped to text/html and so on

#include "MimeTypeMaps.h"


// =======================================================================================
// Static variables, etc.

ExtensionMimeTypeMap* ExtensionMimeTypeMap::theOnlyMap = NULL;
MimeTypeMap* MimeTypeMap::theOnlyMap = NULL;

ExtensionMimeTypeMap  extensionMap;
MimeTypeMap           mimeTypeMap;


// =======================================================================================
// The map from MimeType to char*.  Keep synchronized with enum MimeType

const char* MimeTypeMap::inverseMap[] = 
{
        (const char*)"none",      // NoMimeType

        // Text types
        (const char*)"text/html",       // TextHtml,
        (const char*)"text/javascript", // TextJavascript,
        (const char*)"text/css",        // TextCss,
        (const char*)"text/plain",      // TextPlain,
       
        // Image types
        (const char*)"image/gif",       // ImageGif,
        (const char*)"image/jpeg",      // ImageJpeg,
        (const char*)"image/png",       // ImagePng,
        (const char*)"image/x-icon",    // ImageIco,
       
        // Application types
        (const char*)"application/json",                   // ApplicationJson,
        (const char*)"application/x-www-form-urlencoded", // ApplicationXWWWFormUrlEncoded,
        (const char*)"application/octet-stream",          // ApplicationOctetStream,
};


// =======================================================================================
/// @brief MimeTypeMap Constructor

MimeTypeMap::MimeTypeMap(void)
{
  if(theOnlyMap)
    return;
  else
    theOnlyMap = this;

  // Text types
  insert({"text/html", TextHtml});
  insert({"text/javascript", TextJavascript});
  insert({"text/css", TextCss});
  insert({"text/plain", TextPlain});

  // Image types
  insert({"image/gif",    ImageGif});
  insert({"image/jpeg",   ImageJpeg});
  insert({"image/png",    ImagePng});
  insert({"image/x-icon", ImageIco});
  
  // Application types
  insert({"application/json", ApplicationJson});
  insert({"application/x-www-form-urlencoded", ApplicationXWWWFormUrlEncoded});
  insert({"application/octet-stream", ApplicationOctetStream});
}


// =======================================================================================
/// @brief MimeTypeMap Destructor

MimeTypeMap::~MimeTypeMap(void)
{
}


// =======================================================================================
/// @brief ExtensionMimeTypeMap Constructor

ExtensionMimeTypeMap::ExtensionMimeTypeMap(void)
{
  if(theOnlyMap)
    return;
  else
    theOnlyMap = this;

  // Text types
  insert({"html", TextHtml});
  insert({"js", TextJavascript});
  insert({"css", TextCss});
  insert({"txt", TextPlain});

  // Image types
  insert({"gif", ImageGif});
  insert({"jpg", ImageJpeg});
  insert({"jpeg", ImageJpeg});
  insert({"png", ImagePng});
  insert({"ico", ImageIco});
  
  // Application types
  insert({"json", ApplicationJson});
}


// =======================================================================================
/// @brief ExtensionMimeTypeMap Destructor

ExtensionMimeTypeMap::~ExtensionMimeTypeMap(void)
{
}


// =======================================================================================
