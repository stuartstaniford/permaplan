// Copyright Staniford Systems.  All Rights Reserved.  June 2023 -
// Code to read in json version of the iTree database

#include "iTreeList.h"
#include "loadFileToBuf.h"
#include "Logging.h"
#include "Global.h"

#ifdef ITREE_JSON_VERSION
using namespace rapidjson;
char* iTreeFileLocation = (char*)"web/trees.json";
#else
char* iTreeFileLocation = (char*)"web/itree-SpeciesList_28-10-2020-11-10-57-AM.txt";
#endif


// =======================================================================================
/// @brief Constructor

iTreeList::iTreeList(void)
{
  unsigned bufSize;
  char* buf = loadFileToBuf(iTreeFileLocation, &bufSize);
  
#ifdef ITREE_JSON_VERSION
  ParseResult ok = doc.ParseInsitu<kParseCommentsFlag>(buf);
  unless(ok)
   {
    fprintf(stderr, "JSON parse error on iTree json file (%s): %s (%u)\n",
            iTreeFileLocation, GetParseError_En(ok.Code()), (unsigned)(ok.Offset()));
    exit(1);
   }
  if(!doc.IsArray())
    err(-1, "Base of tree json file %s is not JSON array.\n", iTreeFileLocation);
#else
  parseItreeFile(buf, bufSize);
#endif
}


// =======================================================================================
/// @brief Parse the itree file directly (rather than getting JSON from a script).
/// @param buf A buffer containing the file loaded into memory.
/// @param bufSize The number of bytes in the file.

void iTreeList::parseItreeFile(char* buf, unsigned bufSize)
{
  
}


// =======================================================================================
/// @brief Destructor

iTreeList::~iTreeList(void)
{
}


// =======================================================================================
