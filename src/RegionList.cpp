// Copyright Staniford Systems.  All Rights Reserved.  Jun 2021 -
// This class is for managing lists of species and lists of lists of species for use
// in selecting based on region/type

#include "RegionList.h"
#include "loadFileToBuf.h"
#include "Global.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/error/en.h"
#include <err.h>

RegionList* RegionList::root = NULL;


// =======================================================================================
// Constructor

RegionList::RegionList(const char* fileName)
{
  using namespace rapidjson;

  rapidjson::Document doc;
  unsigned bufSize;
  char* buf = loadFileToBuf(fileName, &bufSize);
    
  ParseResult ok = doc.ParseInsitu<kParseCommentsFlag>(buf);
  if (!ok)
   {
    fprintf(stderr, "JSON parse error on %s: %s (%u)\n", fileName,
              GetParseError_En(ok.Code()), (unsigned)(ok.Offset()));
    exit(1);
   }
  unless(doc.IsObject())
    err(-1, "Base of file %s is not JSON object.\n", fileName);
  for (Value::ConstMemberIterator itr = doc.MemberBegin(); itr != doc.MemberEnd(); ++itr)
   {
    std::string name = itr->name.GetString();
    const char* value = itr->value.GetString();
    RegionList* child = new RegionList(value);
    (*this)[name] = child;
   }
}


// =======================================================================================
// Destructor

RegionList::~RegionList(void)
{
}


// =======================================================================================
// Singleton-ish structure where the global root is available anywhere through this
// accessor method.

RegionList& RegionList::getRoot(void)
{
  return *root;
}


// =======================================================================================
// This is the entry point on initialization that sets up the tree structure.

void RegionList::loadRoot(void)
{
  root = new RegionList((char*)"Materials/Trees/Regions/global.json");
}


// =======================================================================================
