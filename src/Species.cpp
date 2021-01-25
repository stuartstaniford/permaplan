// Copyright Staniford Systems.  All Rights Reserved.  Jan 2021 -
// This class is a container for the factors governing the evolution of a particular
// species (mainly extracted from an OTDL JSON object).

#include "Species.h"
#include <err.h>
#include "loadFileToBuf.h"
#include "GlobalMacros.h"

using namespace rapidjson;

rapidjson::Document Species::speciesIndex;

// =======================================================================================
// Constructors.

Species::Species(void)
{
  unless(speciesIndex.IsObject())
    speciesIndex.SetObject();
}


// =======================================================================================
// Destructor

Species::~Species(void)
{
}


// =======================================================================================
// Read a particular file containing an OTDL description of the type of tree we are.

using namespace rapidjson;

void Species::readOTDLFromFile(char* fileName)
{
  unsigned bufSize;
  char* buf = loadFileToBuf(fileName, &bufSize);
  Document doc;

  ParseResult ok = doc.ParseInsitu<kParseCommentsFlag>(buf);
  if (!ok)
   {
    fprintf(stderr, "JSON parse error on OTDL file %s: %s (%u)\n", fileName,
          GetParseError_En(ok.Code()), (unsigned)(ok.Offset()));
    exit(1);
   }
  if(!doc.IsObject())
    err(-1, "Base of OTDL file %s is not JSON object.\n", fileName);
  if(!validateOTDL(doc))
    err(-1, "Invalid OTDL file %s - see log for details\n", fileName);
}


// =======================================================================================
// Validate OTDL/JSON structure of the type of tree we are.

bool Species::validateOTDL(Document& doc)
{
  bool retVal = true;
  
  return retVal;
}


// =======================================================================================
// Static function to find the entry for a particular "genus/species".

Species* Species::getSpeciesByPath(char* speciesPath)
{
  return NULL;
}


// =======================================================================================
// Tell callers our name at runtime.

const char* Species::objectName(void)
{
  static char* name = (char*)"Species";
  return name;
}


// =======================================================================================
// We assume we are part of a table of visual objects and we just contribute one row
// about this particular object.

bool Species::diagnosticHTML(HttpDebug* serv)
{
  serv->addResponseData("<tr><td>Species</td>");
  return true;
}


// =======================================================================================
