// Copyright Staniford Systems.  All Rights Reserved.  Mar 2021 -
// This class is for managing the model of a leaf - both visual parts and it's
// photosynthetic behavior.


#ifndef LEAF_MODEL_H
#define LEAF_MODEL_H

#include "JSONStructureChecker.h"
#include <cglm/cglm.h>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/error/en.h"

#ifndef LEAF_MODEL_IMPLEMENTATION
// These are basically global variables
extern char* seasonNames[4]; // 0 = spring, 1 = summer, etc.
extern unsigned displaySeason;
#endif

// =======================================================================================
// Class variable initialization

class WoodySegment;

class LeafModel
{
  friend WoodySegment;
public:
  
  // Instance variables - public

  // Member functions - public
  LeafModel(rapidjson::Document& otdlDoc, LeafModel* leafParent,
                                            JSONStructureChecker* J, char* phrase);
  ~LeafModel(void);
  inline bool isValid(void) {return validOTDL;}
  int writeOTDL(char* buf, unsigned bufSize);

  
private:
  
  // Instance variables - private
  LeafModel*                parent;
  unsigned                  leafColors[4];
  float                     photosynthesisMax;
  float                     quantumEfficiency;
  float                     darkRespiration;
  float                     NRHM_phi;
  bool                      validOTDL;
  char*                     OTDLDocPhrase;
  JSONStructureChecker*     jCheck;

  // Member functions - private
  bool  validateFoliage(rapidjson::Document& doc);
  bool  validateLeafColors(rapidjson::Value& leafColorObject);
  bool  validateLeafColorSeason(rapidjson::Value& leafColorObject, int s);
  bool  validatePICurve(rapidjson::Value& pICurveObject);
  LeafModel(const LeafModel&);                 // Prevent copy-construction
  LeafModel& operator=(const LeafModel&);      // Prevent assignment

};

#endif




