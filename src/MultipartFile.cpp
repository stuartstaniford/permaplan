// Copyright Staniford Systems.  All Rights Reserved.  April 2023 -
// Class for handling web uploads of files.  Is mainly a C++ wrapper around 
// multipart_parser.h/.c

#include "MultipartFile.h"
#include "Logging.h"
#include <string.h>


// =======================================================================================
// Static variable initialization, etc

bool MultipartFile::classInitDone = false;
multipart_parser_settings MultipartFile::callbacks;


// =======================================================================================
/// @brief Constructor
///
/// @param contentTypeValue The value of the Content-Type header after the multipart/. 
/// This is required in order to extract the boundary string.

MultipartFile::MultipartFile(char* contentTypeValue): isValid(true)
{
  // Set up the callbacks once at class initialization time
  unless(classInitDone)
   {
    memset(&callbacks, 0, sizeof(multipart_parser_settings));
    callbacks.on_header_field = read_header_name;
    callbacks.on_header_value = read_header_value;
    classInitDone = true;
   }
  
  // Parse the Content-Type value, boundary, etc
  // Our string starts after the Content-type: multipart/
  if(strcmp(contentTypeValue, "form-data;") == 0)
   {
    // UP TO HERE
   }
  else
   {
    LogRequestErrors("UP TO HERE"); 
    isValid = false;
    return;
   }
}


// =======================================================================================
/// @brief Destructor

MultipartFile::~MultipartFile(void)
{
}


// =======================================================================================

int read_header_name(multipart_parser* p, const char *at, size_t length)
{
   printf("%.*s: ", (int)length, at);
   return 0;
}

// =======================================================================================

int read_header_value(multipart_parser* p, const char *at, size_t length)
{
   printf("%.*s\n", (int)length, at);
   return 0;
}

// =======================================================================================
