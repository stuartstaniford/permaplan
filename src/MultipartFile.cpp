// Copyright Staniford Systems.  All Rights Reserved.  April 2023 -
// Class for handling web uploads of files.  Is mainly a C++ wrapper around 
// multipart_parser.h/.c

#include "MultipartFile.h"
#include <string.h>


// =======================================================================================
// Static variable initialization, etc

bool MultipartFile::classInitDone = false;
multipart_parser_settings MultipartFile::callbacks;


// =======================================================================================
/// @brief Constructor

MultipartFile::MultipartFile(void)
{
  // Set up the callbacks once at class initialization time
  unless(classInitDone)
   {
    memset(&callbacks, 0, sizeof(multipart_parser_settings));
    callbacks.on_header_field = read_header_name;
    callbacks.on_header_value = read_header_value;
    classInitDone = true;
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
