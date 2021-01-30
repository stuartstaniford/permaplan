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

#define SPECIES_ARRAY_SIZE   2048 // max number of species in a design


// =======================================================================================
// Class variable initialization

class Species
{
 public:
  
  // Instance variables - public;

  // Member functions - public
  Species(rapidjson::Document& otdlDoc);
  ~Species(void);
  void extractBarkColors(rapidjson::Value& colorsArray);
  const char* objectName(void);
  bool        diagnosticHTML(HttpDebug* serv);

 public:
  // Static functions and variables
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
  static unsigned short speciesCount;

 private:
  
  // Instance variables - private
  unsigned  barkColorSize;
  unsigned* barkColors;
  float*    barkDividers;

  // Member functions - private
  Species(const Species&);                 // Prevent copy-construction
  //Species& operator=(const Species&);      // Prevent assignment
};

#endif

// =======================================================================================
// Utility functions




