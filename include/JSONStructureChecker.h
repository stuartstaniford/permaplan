// Copyright Staniford Systems.  All Rights Reserved.  Apr 2020 -

#ifndef JSON_STRUCTURE_CHECKER_H
#define JSON_STRUCTURE_CHECKER_H

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/error/en.h"
#include "Timeval.h"
#include "Version.h"

// =======================================================================================
// Useful macros

#define spaceUnitsText(x) ((x)?(char*)"meters":(char*)"feet")

#define LOG_BUF_SIZE 512
#define sprintBuf(...) snprintf(logBuf, LOG_BUF_SIZE, __VA_ARGS__)


// =======================================================================================
/// @brief Enum to distinguish OLDF flavored JSON from OTDL flavor.

enum JSONType {OLDF, OTDL};


// =======================================================================================
/// @brief This class provides support for checking various generic patterns that show up 
/// OTDL and OLDF a lot.

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
  bool validateFloatMemberExists(rapidjson::Value& thisObject, char* objName, char* member);
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
  bool checkMandatoryHeritableFloatValue(rapidjson::Value& jsonObject,
                                                              void* parent, char* name);
  bool checkMandatoryHeritableStringValue(rapidjson::Value& jsonObject,
                                                              void* parent, char* name);
  bool checkMandatoryHeritableUnsignedValue(rapidjson::Value& jsonObject,
                                                              void* parent, char* name);
                                                                      
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

