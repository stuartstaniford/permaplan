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
#include "LandSurface.h"
#include "PmodConfig.h"
#include "Timeval.h"
#include "JSONStructureChecker.h"

// =======================================================================================
// Class variable initialization

class Species;

class PmodDesign
{
  friend Species; //XX temp hack - need to generalize validation function instead.
  public:
  
  // Instance variables - public
  rapidjson::Document doc;
  int                   baseYear;
  Timeval               fileTime;
  FILE*                 writeFile;
  bool                  metricUnits;
  JSONStructureChecker* jCheck;
  
private:
  
  static PmodDesign* design;
  
public:

  // Member functions - public
  PmodDesign(void);
  bool validateOLDF(void);
  void writeOLDFFile(LandSurface& land);
  ~PmodDesign(void);
  static PmodDesign& getDesign(void) // Get the singleton instance
  {
   return *design;
  }
private:

  // Member functions - private
  
  // Functions associated with validating the OLDF file.
  bool validateJSONUnixTime(rapidjson::Value& object, char* objName);
  bool validateOptionalStringMember(rapidjson::Value& thisObject, char* objName, char* member);
  bool validateOptionalStringOrArrayString(rapidjson::Value& thisObject, char* objName, char* member);
  bool validateTreeDiameter(rapidjson::Value& treeDiamArray, char* objName);
  bool validateIntroductoryData(void);
  bool validateSpaceUnits(rapidjson::Value& introductoryData);
  bool validateBaseYear(rapidjson::Value& introductoryData);
  bool validateVersion(rapidjson::Value& introductoryData);
  bool validateFileTime(rapidjson::Value& introductoryData);
  bool validateReferencePoint(rapidjson::Value& boundaries);
  bool validateArcs(rapidjson::Value& boundaries);
  bool validateBoundaries(void);
  bool validateTexture(rapidjson::Value& landSurface);
  bool validateAltitudes(rapidjson::Value& landSurface);
  bool validateLandSurface(void);
  bool validatePlants(void);

  // Functions associated with writing out the OLDF file
  void writeIntroductoryData(char* insert);

  PmodDesign(const PmodDesign&);                 // Prevent copy-construction
  PmodDesign& operator=(const PmodDesign&);      // Prevent assignment

};


#endif

