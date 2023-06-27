// Copyright Staniford Systems.  All Rights Reserved.  June 2023 -

#ifndef I_TREE_LIST_H
#define I_TREE_LIST_H

#define ITREE_JSON_VERSION 1

#include "Global.h"

#ifdef ITREE_JSON_VERSION
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/error/en.h"
#endif


// =======================================================================================
/// @brief Code to read in json version of the iTree database.
///
/// Also has code to allow permaserv to print a table of it.

class iTreeList
{
public:
  
  // Instance variables - public
#ifdef ITREE_JSON_VERSION
  rapidjson::Document   doc;
#endif
  
  // Member functions - public
  iTreeList(void);
  ~iTreeList(void);
  void parseItreeFile(char* buf, unsigned bufSize);

private:
  
  // Instance variables - private
  
  // Member functions - private
  PreventAssignAndCopyConstructor(iTreeList);
};


// =======================================================================================

#endif




