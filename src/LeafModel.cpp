// Copyright Staniford Systems.  All Rights Reserved.  Mar 2021 -
// This class is for managing the model of a leaf - both visual parts and it's
// photosynthetic behavior.

#define LEAF_MODEL_IMPLEMENTATION

#include "LeafModel.h"
#include "GlobalMacros.h"
#include "Logging.h"
#include <cstdio>
#include <err.h>

using namespace rapidjson;

// global variables to be used by parts of plants
char* seasonNames[4] = {(char*)"spring", (char*)"summer", (char*)"fall", (char*)"winter"};
unsigned displaySeason = 1u;

// =======================================================================================
// Constructor to extract data from OTDL doc.

LeafModel::LeafModel(Document& otdlDoc, LeafModel* leafParent, JSONStructureChecker* J,
                                                            char* phrase):
                                          parent(leafParent),
                                          OTDLDocPhrase(phrase),
                                          jCheck(J)
{
  if(validateFoliage(otdlDoc))
    validOTDL = true;
  else
   {
    validOTDL = false;
    return;
   }

  unless(otdlDoc.HasMember("foliage"))
   {
    // The foliage object is heritable in it's entirety, so if we don't have one, we have
    // to copy everything over from the parent.
    memcpy(leafColors, parent->leafColors, 4*sizeof(unsigned));
    photosynthesisMax = parent->photosynthesisMax;
    quantumEfficiency = parent->quantumEfficiency;
    darkRespiration   = parent->darkRespiration;
    NRHM_phi          = parent->NRHM_phi;
    return;
   }
  
  // We have a foliage object.  We might be getting some pieces from it, but
  // inheriting others.
  Value& foliageData = otdlDoc["foliage"];
    
  // leafColors - mandatory, heritable
  if(foliageData.HasMember("leafColors"))
   {
    for(int s=0; s<3; s++)  //loop over seasons
     {
      if(foliageData["leafColors"].HasMember(seasonNames[s]))
        leafColors[s] = colorFromRGBArray(foliageData["leafColors"][seasonNames[s]]);
      else
        leafColors[s] = parent->leafColors[s];
     }
   }
  else
    memcpy(leafColors, parent->leafColors, 4*sizeof(unsigned));

  // piCurve - mandatory, heritable
  if(foliageData.HasMember("pICurve"))
   {
    // photosynthesisMax - mandatory, heritable
    if(foliageData["pICurve"].HasMember("photosynthesisMax"))
      photosynthesisMax = foliageData["pICurve"]["photosynthesisMax"].GetFloat();
    else
      photosynthesisMax = parent->photosynthesisMax;

    // quantumEfficiency - mandatory, heritable
    if(foliageData["pICurve"].HasMember("quantumEfficiency"))
      quantumEfficiency = foliageData["pICurve"]["quantumEfficiency"].GetFloat();
    else
      quantumEfficiency = parent->quantumEfficiency;

    // darkRespiration - mandatory, heritable
    if(foliageData["pICurve"].HasMember("darkRespiration"))
      darkRespiration = foliageData["pICurve"]["darkRespiration"].GetFloat();
    else
      darkRespiration = parent->darkRespiration;

    // NRHM_Phi - mandatory, heritable
    // XX interchangeable models is not implemented yet, only handle NRHM
    if(foliageData["pICurve"].HasMember("NRHM")
                          && foliageData["pICurve"]["NRHM"].IsObject()
                          && foliageData["pICurve"]["NRHM"].HasMember("phi"))
      NRHM_phi = foliageData["pICurve"]["NRHM"]["phi"].GetFloat();
    else
      NRHM_phi = parent->NRHM_phi;
   }
  else
   {
    photosynthesisMax = parent->photosynthesisMax;
    quantumEfficiency = parent->quantumEfficiency;
    darkRespiration = parent->darkRespiration;
    NRHM_phi = parent->NRHM_phi;
   }
}


// =======================================================================================
// Destructor

LeafModel::~LeafModel(void)
{
}


// =======================================================================================
// Validate foliage:leafColors for one season.

bool LeafModel::validateLeafColorSeason(Value& leafColorObject, int s)
{
  bool   retVal       = true;
  
  if(leafColorObject[seasonNames[s]].IsString())
   {
    if(strcmp("none", leafColorObject[seasonNames[s]].GetString()) == 0)
     {
      LogOTDLDetails("leafColors no leaves in %s for %s\n",
                                                    seasonNames[s], jCheck->sourcePhrase);
     }
    else
     {
      LogOTDLValidity("leafColors has invalid value for %s for %s\n",
                                                    seasonNames[s], jCheck->sourcePhrase);
      retVal = false;
     }
   }
  else if(leafColorObject[seasonNames[s]].IsArray())
   {
    char objName[32];
    snprintf(objName, 32, "leafColor:%s", seasonNames[s]);
    retVal &= jCheck->validateRGBArray(leafColorObject[seasonNames[s]], objName);
   }
  else
   {
    LogOTDLValidity("leafColors has invalid type for %s for %s\n",
                                                    seasonNames[s], jCheck->sourcePhrase);
    retVal = false;
   }

  return retVal;
}


// =======================================================================================
// Validate the foliage:leafColors section of an OTDL object.

bool LeafModel::validateLeafColors(Value& leafColorObject)
{
  bool   retVal       = true;
  
  unless(leafColorObject.IsObject())
   {
    LogOTDLValidity("leafColors is not object in %s\n", jCheck->sourcePhrase);
    return false;
   }
  
  for(int s=0; s<4; s++) // loop over seasons
   {
    if(leafColorObject.HasMember(seasonNames[s]))
      retVal &= validateLeafColorSeason(leafColorObject, s);
    else if(parent)
     {
      LogOTDLDetails("Inheriting leafColors for %s from parent in %s\n",
                                                  seasonNames[s], jCheck->sourcePhrase);
     }
    else
     {
      LogOTDLValidity("No value for %s in leafColors object for %s\n",
                                                  seasonNames[s], jCheck->sourcePhrase);
      retVal = false;
     }
   }

  return retVal;
}
  

// =======================================================================================
// Validate the foliage:pICurve section of an OTDL object.

bool LeafModel::validatePICurve(Value& pICurveObject)
{
  bool   retVal       = true;
  
  unless(pICurveObject.IsObject())
   {
    LogOTDLValidity("pICurveObject is not object in %s\n", jCheck->sourcePhrase);
    return false;
   }
  
  // photosynthesisMax - mandatory, heritable
  retVal &= jCheck->checkMandatoryHeritableFloatValue(pICurveObject,
                                                      parent, (char*)"photosynthesisMax");

  // quantumEfficiency - mandatory, heritable
  retVal &= jCheck->checkMandatoryHeritableFloatValue(pICurveObject,
                                                      parent,  (char*)"quantumEfficiency");

  // darkRespiration - mandatory, heritable
  retVal &= jCheck->checkMandatoryHeritableFloatValue(pICurveObject,
                                                      parent,  (char*)"darkRespiration");

  // NRHM - mandatory, heritable - XX only because other models not supported yet
  if(pICurveObject.HasMember("NRHM"))
   {
    unless(pICurveObject["NRHM"].IsObject())
     {
      LogOTDLValidity("NRHM is not object in %s\n", jCheck->sourcePhrase);
      return false;
     }
    retVal &= jCheck->checkMandatoryHeritableFloatValue(pICurveObject["NRHM"],
                                                          parent,  (char*)"phi");
   }
  else if(parent)
   {
    LogOTDLDetails("Inheriting NRHM from parent in %s\n", jCheck->sourcePhrase);
   }
  else
   {
    LogOTDLValidity("No NRHM object for %s\n", jCheck->sourcePhrase);
    retVal = false;
   }

  return retVal;
}
  

// =======================================================================================
// Validate the foliage section of an OTDL object.

bool LeafModel::validateFoliage(Document& doc)
 {
  bool retVal = true;
  
  // foliage is mandatory but heritable, so we might be getting it from parent
  if(doc.HasMember("foliage"))
   {
    unless(doc["foliage"].IsObject())
     {
      LogOTDLValidity("Foliage is not object in %s\n", OTDLDocPhrase);
      return false;
     }
   }
  else if(parent)
   {
    // there is no foliage data in child, so get it from the parent
    LogOTDLDetails("Inheriting foliage data from parent in %s\n", OTDLDocPhrase);
    return true;
   }
  else
   {
    // we are just SOL.
    LogOTDLValidity("No foliage data in %s\n", OTDLDocPhrase);
    return false;
   }

  // if we get down here, there is a foliage object at this level to go through.
  Value& foliageObject = doc["foliage"];

  // leafColors - mandatory, heritable
  if(foliageObject.HasMember("leafColors"))
    retVal &= validateLeafColors(foliageObject["leafColors"]);
  else if(parent)
   {
    LogOTDLDetails("Inheriting leafColors from parent in %s\n", jCheck->sourcePhrase);
   }
  else
   {
    LogOTDLValidity("No leafColors object for %s\n", jCheck->sourcePhrase);
    retVal = false;
   }

  // pICurve - mandatory, heritable
  if(foliageObject.HasMember("pICurve"))
    retVal &= validatePICurve(foliageObject["pICurve"]);
  else if(parent)
   {
    LogOTDLDetails("Inheriting pICurve from parent in %s\n", jCheck->sourcePhrase);
   }
  else
   {
    LogOTDLValidity("No pICurve object for %s\n", jCheck->sourcePhrase);
    retVal = false;
   }

  return retVal;
}


// =======================================================================================
// Write out the OTDL object to a buffer (eg used by diagnosticHTML for Species page).

#define bufprintf(...) if((buf += snprintf(buf, end-buf,  __VA_ARGS__)) >= end) {return -1;}

int LeafModel::writeOTDL(char* buf, unsigned bufSize)
{
  char* end = buf + bufSize;

  // foliage
  bufprintf("  \"foliage\":\n   {\n");
  bufprintf("    \"leafColors\":\n     {\n");
  //XX iterate over seasons
  bufprintf("     },\n");
  bufprintf("    \"pICurve\":\n     {\n");
  bufprintf("      \"photosynthesisMax\": %f,\n", photosynthesisMax);
  bufprintf("      \"quantumEfficiency\": %f\n", quantumEfficiency);
  bufprintf("      \"darkRespiration\": %f\n", darkRespiration);
  bufprintf("      \"NRHM_phi\": %f\n", NRHM_phi);
  bufprintf("     }\n");

  bufprintf("   },\n");

  return (bufSize - (int)(end-buf));
}


// =======================================================================================
