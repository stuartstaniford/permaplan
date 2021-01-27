// Copyright Staniford Systems.  All Rights Reserved.  Apr 2020 -
// Main object that encapsulates the design of a given piece of land.  This is basically
// the contents of the main json file given via -d.  Generally the rest of the command
// line is captured in PmodConfig, not in here.

#ifndef JSON_STRUCTURE_CHECKER_H
#define JSON_STRUCTURE_CHECKER_H

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/error/en.h"
#include "Timeval.h"

#define spaceUnitsText(x) ((x)?(char*)"meters":(char*)"feet")

// =======================================================================================
// Class variable initialization

class JSONStructureChecker
{
  public:
  
  // Instance variables - public

  // Member functions - public
  JSONStructureChecker(void);
  ~JSONStructureChecker(void);

private:

  // Member functions - private
  
  // Instance variables - private
  rapidjson::Document doc;

  JSONStructureChecker(const JSONStructureChecker&);            // Prevent copy-construction
  JSONStructureChecker& operator=(const JSONStructureChecker&); // Prevent assignment

};

#endif

