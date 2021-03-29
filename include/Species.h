// Copyright Staniford Systems.  All Rights Reserved.  Jan 2021 -
// This class is a container for the factors governing the evolution of a particular
// species (mainly extracted from an OTDL JSON object).

#ifndef SPECIES_H
#define SPECIES_H

#include "HttpDebug.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/error/en.h"
#include "JSONStructureChecker.h"
#include "Version.h"
#include <map>
#include <unordered_map>
#include <string>

#define SPECIES_ARRAY_SIZE   2048 // max number of species in a design
#define MAX_SPECIES_PATH     256  // the following considers genus/species, then we allow
                                   // a fudge factor for variety names.
                                   //https://en.wikipedia.org/wiki/List_of_long_species_names
#ifndef SPECIES_IMPLEMENTATION
// These are basically global variables
extern char* seasonNames[4]; // 0 = spring, 1 = summer, etc.
extern unsigned displaySeason;
#endif

// =======================================================================================
// Class variable initialization

class Species;
class SpeciesList: public std::unordered_map<std::string, Species*>
{
  
};

class Species
{
 public:

  static unsigned OTDLVersion[];

  // Instance variables - public;
  const char*               genusName;
  const char*               speciesName;
  const char*               varName;
  const char*               sourceName;
  Species*                  parent;
  JSONStructureChecker*     jCheck;
  float                     stemRate;           // in spaceUnits/year
  float                     branchSpacing;      // in spaceUnits
  unsigned short            branchFactor;
  float                     branchAngle;        // in degrees
  float                     branchSpiralAngle;  // in degrees
  float                     initSapThickness;   // in spaceUnits
  float                     initBarkThickness;  // in spaceUnits
  float                     maxHeight;          // in spaceUnits
  float                     maxRadius;          // in spaceUnits
  float                     maxWidth;           // in spaceUnits
  float                     maxAge;             // in years
  Version                   version;
  std::map<float, unsigned> barkColorMap;
  unsigned                  leafColors[4];
  float                     photosynthesisMax;
  float                     quantumEfficiency;
  float                     darkRespiration;
  float                     NRHM_phi;

  // Member functions - public
  Species(rapidjson::Document& otdlDoc, char* source);
  void initializeOverViewData(rapidjson::Value& overviewData);
  void initializeWoodData(rapidjson::Document& otdlDoc);
  void initializeFoliageData(rapidjson::Document& otdlDoc);
  ~Species(void);
  void        logisticGrowthModel(float age, float& radius, float& height);
  void        extractBarkColors(rapidjson::Value& colorsArray);
  unsigned    getBarkColor(float age);
  const char* objectName(void);
  int         writeOTDL(char* buf, unsigned bufSize);
  bool        diagnosticHTML(HttpDebug* serv);
  bool        validateOverviewData(rapidjson::Document& doc);
  bool        validateOTDL(rapidjson::Document& doc);
  bool        validateCommonNames(rapidjson::Value& containObj);
  bool        validateFoliage(rapidjson::Document& doc);
  bool        validateWood(rapidjson::Document& doc);
  bool        validateBarkTextures(rapidjson::Value& obj);
  bool        validateBarkColors(rapidjson::Value& colorsArray);
  bool        validateLeafColors(rapidjson::Value& leafColorObject);
  bool        validateLeafColorSeason(rapidjson::Value& leafColorObject, int s);
  bool        validatePICurve(rapidjson::Value& pICurveObject);
  bool        checkMandatoryHeritableFloatValue(rapidjson::Value& jsonObject, char* name);
  bool        checkMandatoryHeritableUnsignedValue(rapidjson::Value& jsonObject,
                                                                              char* name);

  inline bool isValid(void) {return validOTDL;}
  
  // Static public functions
  static rapidjson::Document speciesIndex;
  static rapidjson::Document& readOTDLFromBuf(char* buf, char* sourceName);
  static Species* getSpeciesByPath(const char* speciesPath);
  static Species* loadLocalOTDLEntry(const char* speciesPath);
  static bool findSpeciesForHTTPDebug(HttpDebug* serv, char* path);

  // static public variables
  static Species** speciesPtrArray;
  static unsigned short speciesCount;
  static std::unordered_map<std::string, unsigned> genusList;
  static std::unordered_map<std::string, SpeciesList*> genusSpeciesList;

 private:
  
  // Instance variables - private
  bool                      validOTDL;

  // Member functions - private
  Species(const Species&);                 // Prevent copy-construction
  //Species& operator=(const Species&);      // Prevent assignment
};


// =======================================================================================
// Utility functions

// compute the season (0..4) from a floating point year.
inline unsigned season(float year)
{ return (int)((year - floorf(year)-1.0f/6.0f)*4.0f)%4;}


#endif
