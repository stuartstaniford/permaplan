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
std::unordered_map<std::string, unsigned> Species::genusList;
std::unordered_map<std::string, SpeciesList*> Species::genusSpeciesList;
unsigned Species::OTDLVersion[] = {0, 0, 1};

// =======================================================================================
// Constructors.

Species::Species(Document& otdlDoc)
{
  // NOTE WELL.  Numeric values in OTDL are expressed in mm, but for uniformity in graphic
  // calculations, internally we must store them in spaceUnits.
  
  // store this species/genus in the static arrays for looking us up.
  genusName = otdlDoc["overviewData"]["genus"].GetString();
  speciesName = otdlDoc["overviewData"]["species"].GetString();
  genusList[genusName]++;
  if(genusSpeciesList.find(genusName) == genusSpeciesList.end())
    genusSpeciesList[genusName] = new SpeciesList;
  (*genusSpeciesList[genusName])[speciesName] = this;

  // other overviewdata
  maxHeight  = otdlDoc["overviewData"]["maxHeight"].GetFloat()/mmPerSpaceUnit;
  maxRadius  = otdlDoc["overviewData"]["maxGirth"].GetFloat()/mmPerSpaceUnit/M_PI/2.0;
  maxAge  = otdlDoc["overviewData"]["maxAge"].GetFloat();

  // fill out the barkColorMap array (dedicated function for this)
  extractBarkColors(otdlDoc["wood"]["barkColors"]);

  // stemRate, initSapThickness, initBarkThickness
  stemRate          = otdlDoc["wood"]["stemRate"].GetFloat()/mmPerSpaceUnit;
  initSapThickness  = otdlDoc["wood"]["initSapThickness"].GetFloat()/mmPerSpaceUnit;
  initBarkThickness = otdlDoc["wood"]["initBarkThickness"].GetFloat()/mmPerSpaceUnit;
}


// =======================================================================================
// Destructor - currently should never be called as species stay around for the life of
// the program.

Species::~Species(void)
{
}


// =======================================================================================
// Braindead trunk growth model that assumes that growth follows a logistic function, specifically
// that a tree of age zero corresponds to -5, and that at maxAge/2 (for the species) the
// tree will have reached maxHeight/2 and maxRadius/2, which will correspond to +5 on the
// logistic curve.
// See https://en.wikipedia.org/wiki/Logistic_function
// for further detail.

void Species::logisticGrowthModel(float age, float& radius, float& height)
{
  float logisticVal = 0.5f/(1.0f + expf(10.f*(maxAge/4.0f-age))/(maxAge/2.0f));
  radius = maxRadius*logisticVal;
  height = maxHeight*logisticVal;
}


// =======================================================================================
// Extract and store the bark colors for later use in visualization.

void Species::extractBarkColors(Value& colorsArray)
{
  int barkColorSize = colorsArray.Size();

  for(int i=0; i<barkColorSize; i++)
    barkColorMap[colorsArray[i]["ages"][1].GetFloat()]
                                                  = colorFromRGBArray(colorsArray[i]["rgb"]);
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

  // maxHeight
  unless(overviewData.HasMember("maxHeight") && overviewData["maxHeight"].IsNumber())
   {
    LogOTDLValidity("No maxHeight or invalid maxHeight in %s\n", jCheck->sourcePhrase);
    retVal = false;
   }

  // maxGirth
  unless(overviewData.HasMember("maxGirth") && overviewData["maxGirth"].IsNumber())
   {
    LogOTDLValidity("No maxGirth or invalid maxGirth in %s\n", jCheck->sourcePhrase);
    retVal = false;
   }

  // maxAge
  unless(overviewData.HasMember("maxAge") && overviewData["maxAge"].IsNumber())
   {
    LogOTDLValidity("No maxAge or invalid maxAge in %s\n", jCheck->sourcePhrase);
    retVal = false;
   }

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

  // stemRate - mandatory
  unless(woodObject.HasMember("stemRate") && woodObject["stemRate"].IsNumber())
   {
    LogOTDLValidity("No stemRate or invalid stemRate in %s\n", jCheck->sourcePhrase);
    retVal = false;
   }

  // initSapThickness - mandatory
  unless(woodObject.HasMember("initSapThickness")
                                            && woodObject["initSapThickness"].IsNumber())
   {
    LogOTDLValidity("No initSapThickness or invalid initSapThickness in %s\n",
                                                                      jCheck->sourcePhrase);
    retVal = false;
   }

  // initBarkThickness - mandatory
  unless(woodObject.HasMember("initBarkThickness") &&
                                                  woodObject["initBarkThickness"].IsNumber())
   {
    LogOTDLValidity("No initBarkThickness or invalid initBarkThickness in %s\n",
                                                                      jCheck->sourcePhrase);
    retVal = false;
   }

  // barkColors - mandatory
  unless(woodObject.HasMember("barkColors") && woodObject["barkColors"].IsArray())
   {
    LogOTDLValidity("No barkColors data in %s\n", jCheck->sourcePhrase);
    retVal = false;
   }
  else
    retVal &= validateBarkColors(woodObject["barkColors"], jCheck);

  // barkTextures - optional
  if(woodObject.HasMember("barkTextures"))
   {
    unless(woodObject["barkTextures"].IsObject())
     {
      LogOTDLValidity("barkTextures is not object in %s\n", jCheck->sourcePhrase);
      retVal = false;
     }
    else
      retVal &= validateBarkTextures(woodObject["barkTextures"], jCheck);
  }

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
// Write out the OTDL object to a buffer (eg used by diagnosticHTML for Species page).

#define bufprintf(...) if((buf += snprintf(buf, end-buf,  __VA_ARGS__)) >= end) {return -1;}

// NOTE WELL.  Numeric values in OTDL are expressed in mm, but for uniformity in graphic
// calculations, internally we must store them in spaceUnits.

int Species::writeOTDL(char* buf, unsigned bufSize)
{
  char* end = buf + bufSize;

  // initial comment, opening brace
  bufprintf("// OTDL for %s %s (Common Name Needed Here)\n{\n", genusName, speciesName);
  
  // overviewData
  bufprintf("  \"overviewData\":\n   {\n");
  bufprintf("   \"genus\":       \"%s\",\n", genusName);
  bufprintf("   \"species\":     \"%s\",\n", speciesName);
  bufprintf("   \"maxHeight\":     \"%.0f\",\n", maxHeight*mmPerSpaceUnit);
  bufprintf("   \"maxGirth\":     \"%.0f\",\n", maxRadius*mmPerSpaceUnit*M_PI*2.0);
  bufprintf("   \"maxAge\":     \"%.0f\",\n", maxAge);
  bufprintf("   \"commonName\":\n");
  bufprintf("    {\n");
  bufprintf("    },\n");
  Timeval T;
  T.now();
  bufprintf("   \"fileTime\": [%lu, %u],\n", T.tv_sec, T.tv_usec);
  bufprintf("   \"version\":  [%u, %u, %u],\n", OTDLVersion[0], OTDLVersion[1], OTDLVersion[2]);
  bufprintf("   \"authors\":  []\n");
  bufprintf("   },\n");

  // wood
  bufprintf("  \"wood\":\n   {\n");
  bufprintf("   \"stemRate\":           \"%f\",\n", stemRate*mmPerSpaceUnit);
  bufprintf("   \"initSapThickness\":   \"%f\",\n", initSapThickness*mmPerSpaceUnit);
  bufprintf("   \"initBarkThickness\":  \"%f\",\n", initBarkThickness*mmPerSpaceUnit);
  bufprintf("   \"barkColors\":\n");
  bufprintf("    [\n");
  //XX need to iterate over barkcolors
  bufprintf("    ],\n");
  bufprintf("   \"barkTextures\":\n");
  bufprintf("    {\n");
  bufprintf("    }\n");
  
  bufprintf("   },\n");

  // foliage
  bufprintf("  \"foliage\":\n   {\n");
  bufprintf("   },\n");

  // closing
  bufprintf("}\n");
  LogHTTPDetails("Wrote %ld bytes of OTDL in HTTP response.\n", bufSize - (end-buf));
  return (bufSize - (int)(end-buf));
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
  char title[MAX_SPECIES_PATH+16];
  snprintf(title, MAX_SPECIES_PATH+16, "Species: %s %s", genusName, speciesName);
  serv->startResponsePage(title);
  
  httPrintf("<h2>OTDL spec</h2>\n");
  httPrintf("<pre>\n");
  int size;
  if((size = writeOTDL(serv->respPtr, serv->respEnd - serv->respPtr)) >= 0)
    serv->respPtr += size;
  else
    return false;
  httPrintf("</pre>\n");
  serv->endResponsePage();
  return true;
}


// =======================================================================================
// Simple static function just to find the right species, and then return the
// diagnosticHTML for it.

bool Species::findSpeciesForHTTPDebug(HttpDebug* serv, char* path)
{
  char* endGenus   = index(path, '/');
  *endGenus = '\0';
  char* endSpecies = index(endGenus+1, '/');
  if(endSpecies)
    *endSpecies = '\0';
  
  if(genusList.find(path) == genusList.end())
    return false;

  if(genusSpeciesList[path]->find(endGenus+1) == genusSpeciesList[path]->end())
    return false;

  Species* species = (*genusSpeciesList[path])[endGenus+1];
  return species->diagnosticHTML(serv);
}


// =======================================================================================
