// Copyright Staniford Systems.  All Rights Reserved.  June 2023 -

#ifndef I_TREE_LIST_H
#define I_TREE_LIST_H

#include "Global.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/error/en.h"


// =======================================================================================
/// @brief Code to read in json version of the iTree database.
///
/// Also has code to allow permaserv to print a table of it.

class iTreeList
{
public:
  
  // Instance variables - public
  rapidjson::Document   doc;

  // Member functions - public
  iTreeList(void);
  ~iTreeList(void);
  
private:
  
  // Instance variables - private
  
  // Member functions - private
  PreventAssignAndCopyConstructor(iTreeList);
};


// =======================================================================================

#endif




