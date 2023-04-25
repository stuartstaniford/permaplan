// Copyright Staniford Systems.  All Rights Reserved.  April 2023 -

#ifndef MULTIPART_FILE_H
#define MULTIPART_FILE_H

#include "Global.h"
#include "multipart_parser.h"


// =======================================================================================
/// @brief Class for handling web uploads of files.  
///
/// Is mainly a C++ wrapper around multipart_parser.h/.c

class MultipartFile
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  MultipartFile(void);
  ~MultipartFile(void);
  
private:

  // Static class variables - private
  static bool                       classInitDone;
  static multipart_parser_settings  callbacks;

  
  // Instance variables - private
  
  // Member functions - private
  PreventAssignAndCopyConstructor(MultipartFile);
};


// =======================================================================================
// C callback signatures

int read_header_name(multipart_parser* p, const char *at, size_t length);
int read_header_value(multipart_parser* p, const char *at, size_t length);


// =======================================================================================

#endif
