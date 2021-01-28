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

#define LOG_BUF_SIZE 512
#define sprintBuf(...) snprintf(logBuf, LOG_BUF_SIZE, __VA_ARGS__)


// =======================================================================================
// Class variable initialization

enum JSONType {OLDF, OTDL};

class JSONStructureChecker
{
  public:
  
  // Instance variables - public
  char*    sourcePhrase;

  // Member functions - public
  JSONStructureChecker(char* sPhrase, JSONType jType);
  ~JSONStructureChecker(void);
  void makeLog(bool isErr);
  bool validateStringMemberExists(rapidjson::Value& thisObject, char* objName, char* member);
  bool validateFileTime(rapidjson::Value& containObj);
  bool validateVersion(rapidjson::Value& containObj);
  bool validateGenusName(char* objName, const char* genus);
  bool validateSpeciesName(char* objName, const char* species);

private:

  // Member functions - private
  
  // Instance variables - private
  JSONType type;
  char     logBuf[LOG_BUF_SIZE];
  int      expectedVersion[3];

  JSONStructureChecker(const JSONStructureChecker&);            // Prevent copy-construction
  JSONStructureChecker& operator=(const JSONStructureChecker&); // Prevent assignment

};


// =======================================================================================
// Utility functions not in class.


#endif

