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
#include "Boundary.h"

// These unit things are used all over the place, so we have them in the global namespace
// for syntactic convenience
#define MM_PER_FOOT 304.8f
#ifndef PMOD_DESIGN_IMPLEMENTATION
extern float          mmPerSpaceUnit;
extern char*          spaceUnitName;
extern char           spaceUnitAbbr;
#endif

// tolerances for how far things can be allowed to be out of the design box
#define TOLERANCE_UP      600.0f
#define TOLERANCE_DOWN    200.0f
#define TOLERANCE_LATERAL 500.0f

// =======================================================================================
// Class variable initialization

class Species;

class PmodDesign
{
  friend Species; //XX temp hack - need to generalize validation function instead.
    
  public:
  
  // Instance variables - public - don't worry about alignment as only one of these
  rapidjson::Document   doc;
  int                   baseYear;
  Timeval               fileTime;
  FILE*                 writeFile;
  bool                  metricUnits;
  JSONStructureChecker* jCheck;
  Boundary              boundary;
  bool                  designBoxValid;
  BoundingBox           designBox;


private:
  
  static PmodDesign* design;
  
public:

  // Member functions - public
  PmodDesign(void);
  bool validateOLDF(void);
  void writeOLDFFile(LandSurface& land);
  ~PmodDesign(void);
  static PmodDesign& getDesign(void) // Get the singleton instance
   { return *design; }
  inline float spaceUnitsFromFeet(float f) {return metricUnits?f/3.28084:f;}
  void addBoxTolerances(void);
  
private:

  // Member functions - private
  
  // Functions associated with validating the OLDF file.
  bool validateJSONUnixTime(rapidjson::Value& object, char* objName);
  bool validateTreeGirth(rapidjson::Value& treeDiamArray, char* objName);
  bool validateIntroductoryData(void);
  bool validateSpaceUnits(rapidjson::Value& introductoryData);
  bool validateBaseYear(rapidjson::Value& introductoryData);
  bool validateFileTime(rapidjson::Value& introductoryData);
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

