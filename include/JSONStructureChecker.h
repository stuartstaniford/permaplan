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
#include "Version.h"

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
  bool validateOptionalStringMember(rapidjson::Value& thisObject, char* objName,
                                                                              char* member);
  bool validateStringMemberExists(rapidjson::Value& thisObject, char* objName, char* member);
  bool validateFileTime(rapidjson::Value& containObj);
  bool validateVersion(rapidjson::Value& containObj);
  bool validateNumberArray(rapidjson::Value& array, int desiredSize, char* objName);
  bool validateIntegerArray(rapidjson::Value& array, int desiredSize, char* objName);
  bool validateRGBArray(rapidjson::Value& array, char* objName);
  bool validateGenusName(char* objName, const char* genus);
  bool validateSpeciesName(char* objName, const char* species);
  bool validateOptionalStringOrArrayString(rapidjson::Value& thisObject, char* objName, char* member);
  bool loadRFC5646LanguageTags(void);
  bool checkLanguageObject(rapidjson::Value& thisObject, char* objName);
  
private:

  // Member functions - private
  
  // Instance variables - private
  JSONType              type;
  char                  logBuf[LOG_BUF_SIZE];
  Version               expectedVersion;
  rapidjson::Document*  languageTags;

  JSONStructureChecker(const JSONStructureChecker&);            // Prevent copy-construction
  JSONStructureChecker& operator=(const JSONStructureChecker&); // Prevent assignment

};


// =======================================================================================
// Utility functions not in class.

unsigned colorFromRGBArray(rapidjson::Value& array);
void RGBArrayFromColor(unsigned color, char buf[]);

#endif

