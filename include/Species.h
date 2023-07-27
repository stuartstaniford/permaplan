// Copyright Staniford Systems.  All Rights Reserved.  Jan 2021 -

#ifndef SPECIES_H
#define SPECIES_H

#include "DynamicallyTypable.h"
#include "Version.h"
#include "Taxonomy.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/error/en.h"


// =======================================================================================
// Forward declarations

class HttpDebug;
class LeafModel;
class JSONStructureChecker;


// =======================================================================================
/// @brief Manage information about a species
///
/// This class is a container for the factors governing the evolution of a particular
/// species (mainly extracted from an OTDL JSON object).

class Species: public DynamicallyTypable
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
  LeafModel*                foliage;
  
  // Member functions - public
  Species(rapidjson::Document& otdlDoc, char* source);
  Species(char* gName, char* sName);
  void initializeOverViewData(rapidjson::Value& overviewData);
  void initializeWoodData(rapidjson::Document& otdlDoc);
  ~Species(void);
  void        logisticGrowthModel(float age, float& radius, float& height);
  void        extractBarkColors(rapidjson::Value& colorsArray);
  unsigned    getBarkColor(float age);
  const char* objectName(void);
  int         writeOTDL(char* buf, unsigned bufSize);
  bool        diagnosticHTML(HttpServThread* serv);
  bool        validateOverviewData(rapidjson::Document& doc);
  bool        validateOTDL(rapidjson::Document& doc);
  bool        validateCommonNames(rapidjson::Value& containObj);
  bool        validateWood(rapidjson::Document& doc);
  bool        validateBarkTextures(rapidjson::Value& obj);
  bool        validateBarkColors(rapidjson::Value& colorsArray);
  virtual DynamicType getDynamicType(void) {return TypeSpecies;}

  inline bool isValid(void) {return validOTDL;}
  
  // Static public functions
  static rapidjson::Document speciesIndex;
  static rapidjson::Document& readOTDLFromBuf(char* buf, char* sourceName);
  static Species* getSpeciesByPath(const char* speciesPath);
  static Species* loadLocalOTDLEntry(const char* speciesPath);
  static void findOTDLFileName(char* path, unsigned pathSize);
  static bool findSpeciesForHTTPDebug(HttpDebug* serv, char* path);

  // static public variables
  static SpeciesList* cachedSpecies;
  static unsigned short speciesCount;
  static std::unordered_map<std::string, unsigned> genusList;
  static std::unordered_map<std::string, SpeciesList*> genusSpeciesList;
  static Taxonomy taxonomy;
  static char* speciesDirectory;

 private:
  
  // Instance variables - private
  bool                      validOTDL;

  // Member functions - private
  Species(const Species&);                 // Prevent copy-construction
  //Species& operator=(const Species&);      // Prevent assignment
};


// =======================================================================================
/// @brief A placeholder for a species that has not been loaded into memory.
/// 
/// We store its path

class SpeciesPath: public DynamicallyTypable
{
  private:
    const char* path;
  
  public:
  inline SpeciesPath(const char* srcPath): path(srcPath){};
  virtual inline ~SpeciesPath(void){};
  inline DynamicType getDynamicType(void) {return TypeSpeciesPath;}
  inline const char* getPath(void) {return path;}
};


// =======================================================================================
// Utility functions

// compute the season (0..4) from a floating point year.
inline unsigned season(float year)
{ return (int)((year - floorf(year)-1.0f/6.0f)*4.0f)%4;}


// =======================================================================================

#endif
