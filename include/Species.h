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
#include <map>
#include <unordered_map>
#include <string>

#define SPECIES_ARRAY_SIZE   2048 // max number of species in a design
#define MAX_SPECIES_PATH  96   //https://en.wikipedia.org/wiki/List_of_long_species_names


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
  float                     stemRate;           // in mm/year
  float                     initSapThickness;   // in mm
  float                     initBarkThickness;  // in mm
  float                     maxHeight;          // in mm!
  std::map<float, unsigned> barkColorMap;

  // Member functions - public
  Species(rapidjson::Document& otdlDoc);
  ~Species(void);
  void        extractBarkColors(rapidjson::Value& colorsArray);
  const char* objectName(void);
  int         writeOTDL(char* buf, unsigned bufSize);
  bool        diagnosticHTML(HttpDebug* serv);

 public:
  // Static public functions
  static rapidjson::Document speciesIndex;
  static rapidjson::Document& readOTDLFromBuf(char* buf, char* sourceName);
  static bool validateOverviewData(rapidjson::Document& doc, JSONStructureChecker* jCheck);
  static bool validateOTDL(rapidjson::Document& doc, char* sourceName);
  static Species* getSpeciesByPath(char* speciesPath);
  static Species* loadLocalOTDLEntry(char* speciesPath);
  static Species** speciesPtrArray;
  static bool validateCommonNames(rapidjson::Value& containObj, JSONStructureChecker* jCheck);
  static bool validateFoliage(rapidjson::Document& doc, JSONStructureChecker* jCheck);
  static bool validateWood(rapidjson::Document& doc, JSONStructureChecker* jCheck);
  static bool validateBarkTextures(rapidjson::Value& obj, JSONStructureChecker* jCheck);
  static bool validateBarkColors(rapidjson::Value& colorsArray, JSONStructureChecker* jCheck);
  static bool findSpeciesForHTTPDebug(HttpDebug* serv, char* path);
  
  // statics public variables
  static unsigned short speciesCount;
  static std::unordered_map<std::string, unsigned> genusList;
  static std::unordered_map<std::string, SpeciesList*> genusSpeciesList;

 private:
  
  // Instance variables - private

  // Member functions - private
  Species(const Species&);                 // Prevent copy-construction
  //Species& operator=(const Species&);      // Prevent assignment
};

#endif

// =======================================================================================
// Utility functions




