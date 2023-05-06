// Copyright Staniford Systems.  All Rights Reserved.  April 2023 -
// Class for handling web uploads of files.  Is mainly a C++ wrapper around 
// multipart_parser.h/.c
// Relevant RFCs used in developing this code include: 
// https://datatracker.ietf.org/doc/html/rfc7578
// https://datatracker.ietf.org/doc/html/rfc7231
// https://datatracker.ietf.org/doc/html/rfc6838

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

MultipartFile::MultipartFile(char* contentTypeValue): isValid(false), 
                                                      boundaryString(nullptr),
                                                      charsetString(nullptr)
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
  if(strcmp(contentTypeValue, "form-data") == 0)
   {
    if(parseFormDataHeader(contentTypeValue + 9))
      isValid = true;
    else
     {
      LogRequestErrors("Couldn't parse multipart content-type %s.\n", contentTypeValue);       
     }
   }
  else
   {
    LogRequestErrors("Unsupported multipart content-type %s.\n", contentTypeValue); 
   }
}


// =======================================================================================
/// @brief Destructor

MultipartFile::~MultipartFile(void)
{
}


// =======================================================================================
/// @brief Constructor
///
/// @param headerString The value of the Content-Type header after the multipart/form-data. 
/// This is required in order to extract the boundary string.

bool MultipartFile::parseFormDataHeader(char* headerString) 
{
  char* token = strtok(headerString, ";");
    
  token = strtok(NULL, ";");
    
  while(token) 
   {
    while (*token == ' ')
      token++;
        
    if(strncmp(token, "boundary=", 9) == 0)
      boundaryString = token + 9;
    else if(strncmp(token, "charset=", 8) == 0)
      charsetString = token + 8;
    else
     {
      LogRequestErrors("Unknown term in multipart content-type %s.\n", token);
      return false;
     }
    token = strtok(NULL, ";");
   }
  if(boundaryString)
    return true;
  else
    return false;
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
