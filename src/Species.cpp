// Copyright Staniford Systems.  All Rights Reserved.  Jan 2021 -
// This class is a container for the factors governing the evolution of a particular
// species (mainly extracted from an OTDL JSON object).

#include "Species.h"
#include <err.h>
#include "loadFileToBuf.h"
#include "GlobalMacros.h"
#include "PmodConfig.h"
#include "PmodDesign.h"

using namespace rapidjson;

rapidjson::Document Species::speciesIndex;
unsigned short Species::speciesCount = 0u;
Species** Species::speciesPtrArray = new Species*[SPECIES_ARRAY_SIZE];


// =======================================================================================
// Constructors.

Species::Species(Document& otdlDoc)
{
}


// =======================================================================================
// Destructor

Species::~Species(void)
{
}


// =======================================================================================
// Read a buffer containing an OTDL description of the type of tree we are.

Document& Species::readOTDLFromBuf(char* buf, char* sourceName)
{
  Document* doc = new Document;

  ParseResult ok = doc->ParseInsitu<kParseCommentsFlag>(buf);
  if (!ok)
   {
    fprintf(stderr, "JSON parse error on OTDL file %s: %s (%u)\n", sourceName,
          GetParseError_En(ok.Code()), (unsigned)(ok.Offset()));
    exit(1);
   }
  if(!doc->IsObject())
    err(-1, "Base of OTDL file %s is not JSON object.\n", sourceName);
  if(!validateOTDL(*doc, sourceName))
    err(-1, "Invalid OTDL file %s - see log for details\n", sourceName);
 
  return *doc;
}


// =======================================================================================
// Function to check the mandatory OTDL commonNames object inside overviewData.

bool Species::validateCommonNames(Value& containObj, JSONStructureChecker* jCheck)
{
  unless(containObj.HasMember("commonNames"))
   {
    LogOTDLValidity("No commonNames object in %s\n", jCheck->sourcePhrase);
    return false;
   }
  unless(containObj["commonNames"].IsObject())
   {
    LogOTDLValidity("commonNames is not object in %s\n", jCheck->sourcePhrase);
    return false;
   }
  
  Value& cNames = containObj["commonNames"];
  return jCheck->checkLanguageObject(cNames, (char*)"commonNames");
}


// =======================================================================================
// Validate the overviewData section of an OTDL object.

bool Species::validateOverviewData(Document& doc, JSONStructureChecker* jCheck)
{
  bool   retVal       = true;
  Value& overviewData = doc["overviewData"];
  char* logObjectName = (char*)"overviewData";
  
  // fileTime
  retVal &= jCheck->validateFileTime(overviewData);

  // Genus
  retVal &= jCheck->validateStringMemberExists(overviewData, logObjectName, (char*)"genus");
  retVal &= jCheck->validateGenusName(logObjectName, overviewData["genus"].GetString());
  
  // Species
  retVal &= jCheck->validateStringMemberExists(overviewData, logObjectName, (char*)"species");
  retVal &= jCheck->validateSpeciesName(logObjectName, overviewData["species"].GetString());

  // variety
  retVal &= jCheck->validateOptionalStringMember(overviewData, logObjectName, (char*)"var");

  // version
  retVal &= jCheck->validateVersion(overviewData);

  // authors
  retVal &= jCheck->validateOptionalStringOrArrayString(overviewData, logObjectName,
                                                                  (char*)"authors");
  // common names object.
  retVal &= validateCommonNames(overviewData, jCheck);

  return retVal;
}


// =======================================================================================
// Validate the bark textures section of an OTDL object.

bool Species::validateBarkTextures(Value& obj, JSONStructureChecker* jCheck)
{
  bool   retVal       = true;
  
  return retVal;
}


// =======================================================================================
// Validate the bark colors section of an OTDL object.

bool Species::validateBarkColors(Value& colorsArray, JSONStructureChecker* jCheck)
{
  bool   retVal       = true;
  
  int N = colorsArray.Size();

  unless(N > 0)
   {
    LogOTDLValidity("barkColors array is empty in %s\n", jCheck->sourcePhrase);
    retVal = false;
   }

  for(int i=0; i<N; i++)
   {
    Value& entry = colorsArray[i];
    char objName[32];
    snprintf(objName, 32, "barkColors[%d]", i);
    unless(entry.IsObject())
     {
      LogOTDLValidity("%s is not object in %s\n", objName, jCheck->sourcePhrase);
      retVal = false;
     }
    
    // Deal with ages
    unless(entry.HasMember("ages"))
     {
      LogOTDLValidity("%s has no ages in %s\n", objName, jCheck->sourcePhrase);
      retVal = false;
     }
    snprintf(objName, 32, "barkColors[%d]:ages", i);
    retVal &= jCheck->validateNumberArray(entry["ages"], 2, objName);

    if(i==0 && 0.0f != entry["ages"][0].GetFloat())
     {
      LogOTDLValidity("%s ages don't start at 0 (%.2f) in %s\n",
                        objName, entry["ages"][0].GetFloat(), jCheck->sourcePhrase);
      retVal = false;
     }
    if(i==N-1 && 10000.0f != entry["ages"][1].GetFloat())
     {
      LogOTDLValidity("%s ages don't end at 10000.0 (%.2f) in %s\n",
                        objName, entry["ages"][1].GetFloat(), jCheck->sourcePhrase);
      retVal = false;
     }

    // Deal with rgb array
    unless(entry.HasMember("rgb"))
     {
      LogOTDLValidity("%s has no rgb in %s\n", objName, jCheck->sourcePhrase);
      retVal = false;
     }
    snprintf(objName, 32, "barkColors[%d]:rgb", i);
    retVal &= jCheck->validateRGBArray(entry["rgb"], objName);
   }
  
  return retVal;
}


// =======================================================================================
// Validate the wood section of an OTDL object.

bool Species::validateWood(Document& doc, JSONStructureChecker* jCheck)
{
  bool   retVal       = true;
  Value& woodObject = doc["wood"];

  if(woodObject.HasMember("barkTextures")) // this is optional
   {
    unless(woodObject["barkTextures"].IsObject())
     {
      LogOTDLValidity("barkTextures is not object in %s\n", jCheck->sourcePhrase);
      retVal = false;
     }
    else
      retVal &= validateBarkTextures(woodObject["barkTextures"], jCheck);
  }

  unless(woodObject.HasMember("barkColors") && woodObject["barkColors"].IsArray())
   {
    LogOTDLValidity("No barkColors data in %s\n", jCheck->sourcePhrase);
    retVal = false;
   }
  else
    retVal &= validateBarkColors(woodObject["barkColors"], jCheck);

  return retVal;
}


// =======================================================================================
// Validate the foliage section of an OTDL object.

bool Species::validateFoliage(Document& doc, JSONStructureChecker* jCheck)
{
  bool   retVal       = true;
  //Value& foliageObject = doc["foliage"];
  //char* logObjectName = (char*)"foliage";
  
  return retVal;
}


// =======================================================================================
// Validate OTDL/JSON structure of the type of tree we are.

bool Species::validateOTDL(Document& doc, char* sourceName)
{
  bool retVal = true;
  char phrase[128];
  snprintf(phrase, 128, "OTDL object %s", sourceName);
  JSONStructureChecker* jCheck = new JSONStructureChecker(phrase, OTDL);
  
  unless(doc.HasMember("overviewData") && doc["overviewData"].IsObject())
   {
    LogOTDLValidity("No overviewData in %s\n", phrase);
    retVal = false;
   }
  else
    retVal &= validateOverviewData(doc, jCheck);

  unless(doc.HasMember("wood") && doc["wood"].IsObject())
   {
    LogOTDLValidity("No wood data in %s\n", phrase);
    retVal = false;
   }
  else
    retVal &= validateWood(doc, jCheck);

  unless(doc.HasMember("foliage") && doc["foliage"].IsObject())
   {
    LogOTDLValidity("No foliage data in %s\n", phrase);
    retVal = false;
   }
  else
    retVal &= validateFoliage(doc, jCheck);

  delete jCheck; jCheck = NULL;
  return retVal;
}


// =======================================================================================
// Static function to find the entry for a particular "genus/species".

Species* Species::getSpeciesByPath(char* speciesPath)
{
  unless(speciesIndex.IsObject())
    speciesIndex.SetObject();

  if(speciesIndex.HasMember(speciesPath))
    return speciesPtrArray[speciesIndex[speciesPath].GetInt()];
  
  // Handle the case where the species has not been seen before.
  // First Check local OTDL directories
  Species* species;
  if( (species = loadLocalOTDLEntry(speciesPath)) )
    return species;
  
  // XX should go out to central web repository
  err(-1, "Couldn't find species %s\n", speciesPath);
  
  return NULL;
}


// =======================================================================================
// Static function to try and load a particular "genus/species" from local disk

Species* Species::loadLocalOTDLEntry(char* speciesPath)
{
  const PmodConfig& config = PmodConfig::getConfig();
  char path[256];
  snprintf(path, 256, "%s/%s/default.otdl", config.speciesDirectory, speciesPath);
  unless(regularFileExists(path))
    return NULL;
  unsigned bufSize;
  char* buf = loadFileToBuf(path, &bufSize);
  Document& doc = readOTDLFromBuf(buf, path);
  
  // At this point we are confident we have valid OTDL, so create the new species
  Species* newSpecies = new Species(doc);
  return newSpecies;
}


// =======================================================================================
// Tell callers our name at runtime.

const char* Species::objectName(void)
{
  static char* name = (char*)"Species";
  return name;
}


// =======================================================================================
// We assume we are part of a table of visual objects and we just contribute one row
// about this particular object.

bool Species::diagnosticHTML(HttpDebug* serv)
{
  serv->addResponseData("<tr><td>Species</td>");
  return true;
}


// =======================================================================================
