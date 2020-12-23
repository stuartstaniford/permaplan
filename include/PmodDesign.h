// Copyright Staniford Systems.  All Rights Reserved.  Apr 2020 -
// Main object that encapsulates the design of a given piece of land.  This is basically
// the contents of the main json file given via -d.  Generally the rest of the command
// line is captured in PmodConfig, not in here.

#ifndef PMODDESIGN_H
#define PMODDESIGN_H

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/error/en.h"
#include "PmodConfig.h"
#include "Timeval.h"

// =======================================================================================
// Class variable initialization


class PmodDesign
{
  public:
  
  // Instance variables - public
  rapidjson::Document doc;
  int                 baseYear;
  Timeval             fileTime;
  
private:
  
  static PmodDesign* design;
  
public:

  // Member functions - public
  PmodDesign(void);
  bool validateOLDF(void);
  ~PmodDesign(void);
  static PmodDesign& getDesign(void) // Get the singleton instance
  {
   return *design;
  }
private:

  // Member functions - private
  bool validateStringMemmberExists(rapidjson::Value& thisObject, char* objName, char* member);
  bool validateOptionalStringMember(rapidjson::Value& thisObject, char* objName, char* member);
  bool validateIntroductoryData(void);
  bool validateSpaceUnits(rapidjson::Value& introductoryData);
  bool validateBaseYear(rapidjson::Value& introductoryData);
  bool validateVersion(rapidjson::Value& introductoryData);
  bool validateFileTime(rapidjson::Value& introductoryData);
  PmodDesign(const PmodDesign&);                 // Prevent copy-construction
  PmodDesign& operator=(const PmodDesign&);      // Prevent assignment

};

#endif

