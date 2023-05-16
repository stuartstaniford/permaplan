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
#include <assert.h>


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
                                                      charsetString(nullptr),
                                                      parser(nullptr)
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
  if(strncmp(contentTypeValue, "form-data", 9) == 0)
   {
    if(parseFormDataHeader(contentTypeValue + 9))
     {
      isValid = true;
      parser = multipart_parser_init(boundaryString, &callbacks);
      unless(parser)
       {
        LogRequestErrors("Initialization failed in multipart_parser_init.\n");
        isValid = false;
       }
      else
       {
        LogRequestParsing("Successfully created MultipartFile object.");
       }
     }
    else
     {
      LogRequestErrors("Couldn't parse multipart Content-Type: %s.\n", contentTypeValue);
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
  if(parser)
    multipart_parser_free(parser);
}


// =======================================================================================
/// @brief Consume a new chunk of data and pass it to the multipart_parser code.
///
/// @returns The number of bytes actually consumed by the multipart_parser.
/// @param buf A char* pointer to the data to be consumed.
/// @param nBytes The count of bytes at the buf pointer.

unsigned MultipartFile::gotNewData(char* buf, int nBytes)
{
  assert(parser);
  size_t parsed = multipart_parser_execute(parser, buf, nBytes);

  LogRequestParsing("multipart_parser consumed %zu bytes of %u given.\n", parsed, nBytes);

  if(parsed == nBytes) 
   {
    LogRequestParsing("Multipart parsing complete!\n");
   }
  
  return (unsigned)parsed;
}


// =======================================================================================
/// @brief Function to parse the Content-Type in a multipart data Post.
///
/// @param headerString The value of the Content-Type header after the multipart/form-data. 
/// This is required in order to extract the boundary string.

bool MultipartFile::parseFormDataHeader(char* headerString) 
{
  char* lasts;
  char* token = strtok_r(headerString, ";", &lasts);
        
  while(token) 
   {
    unless(*token)
     {
      // ; at start of string
      LogRequestParsing("Skipping empty token in multipart Content-Type.\n"); 
      goto ROUND_AGAIN;
     }
    LogRequestParsing("Parsing token %s in multipart Content-Type.\n", token); 
    while (*token == ' ')
      token++;
    if(strncmp(token, "boundary=", 9) == 0)
     {
      boundaryString = token + 9;
      LogRequestParsing("Found boundary string %s in multipart Content-Type.\n", 
                                                                          boundaryString);
     }
    else if(strncmp(token, "charset=", 8) == 0)
     {
      charsetString = token + 8;
      LogRequestParsing("Found charset string %s in multipart Content-Type.\n", 
                                                                            charsetString);
     }
    else
     {
      LogRequestErrors("Unknown term in multipart Content-Type %s.\n", token);
      return false;
     }
ROUND_AGAIN:
    token = strtok_r(NULL, ";", &lasts);
   }
  
  if(boundaryString)
    return true;
  else
    return false;
}


// =======================================================================================
/// @brief C callback function from multipart_parser on header names.
///
/// @returns Zero on success, non zero on error.
/// @param at The header name we are passed.
/// @param length The number of bytes in the header name.

int read_header_name(multipart_parser* p, const char *at, size_t length)
{
  LogRequestParsing("Found multipart header: %.*s: ", (int)length, at);
  return 0;
}


// =======================================================================================
/// @brief C callback function from multipart_parser on header values.
///
/// @returns Zero on success, non zero on error.
/// @param at The value we are passed.
/// @param length The number of bytes in the header name.

int read_header_value(multipart_parser* p, const char *at, size_t length)
{
  LogRequestParsing("Found multipart value: %.*s\n", (int)length, at);
  return 0;
}


// =======================================================================================
