// Copyright Staniford Systems.  All Rights Reserved.  April 2023 -

#ifndef MULTIPART_FILE_H
#define MULTIPART_FILE_H

#include "Global.h"


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
  
  // Instance variables - private
  
  // Member functions - private
  PreventAssignAndCopyConstructor(MultipartFile);
};


// =======================================================================================

#endif




