// Copyright Staniford Systems.  All Rights Reserved.  June 2023 -
// Code to read in json version of the iTree database

#include "iTreeList.h"
#include "loadFileToBuf.h"
#include "Logging.h"
#include "Global.h"
#include <strings.h>
#include <stdio.h>

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
  // What the start of file is like:
  //Genus Name  Species Name  Synonym  Family  Order  Class  Common Name  Species Code  Growth Form  Percent Leaf Type  Leaf Type  Growth Rate  Longevity  Height at Maturity (feet)
  //Abarema  cochliocarpos    Fabaceae  Fabales  Rosidae  Abarema cochliocarpos  ABCO1  Shrub or small tree  Hardwood  Evergreen    
  //Abarema  glauca    Fabaceae  Fabales  Rosidae  Abarema glauca  ABGL  Tree  Hardwood  Evergreen      

  char* line, *lineEnd;
  char* bufEnd = buf + bufSize;
  int count = 0;
  unsigned* tokens;
  unsigned tokenCount;
  
  buf[bufSize-1] = '\0';
  line = buf;
  while(line >= buf && line < bufEnd)
   {
    // Find the end of this line
    lineEnd = index(line, '\n');
    if(lineEnd)
      *lineEnd = '\0';
    
    // Split into tokens
    tokens = splitTokens(line, '\t', tokenCount);
    fprintf(stderr, "Found species %s %s.\n", line + tokens[0], line + tokens[1]);
    
    
    // Ok, done with tokens array (NB not efficient but not performance relevant loop)
    free(tokens);
    
    // Go round again
    line = lineEnd + 1;
    count++;
   }
  fprintf(stderr, "Counted %d lines in file %s.\n", count, iTreeFileLocation);
}


// =======================================================================================
/// @brief Destructor

iTreeList::~iTreeList(void)
{
}


// =======================================================================================
