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

// =======================================================================================
// Constructors.

Species::Species(Document& otdlDoc, Species* parent)
{
  // NOTE WELL.  Numeric values in OTDL are expressed in mm, but for uniformity in graphic
  // calculations, internally we must store them in spaceUnits.
  
  // Genus - mandatory, heritable
  if(otdlDoc["overviewData"].HasMember("genus"))
    genusName = otdlDoc["overviewData"]["genus"].GetString();
  else
    genusName = parent->genusName;
  if(genusSpeciesList.find(genusName) == genusSpeciesList.end())
    genusSpeciesList[genusName] = new SpeciesList;

  //Species - mandatory, heritable
  if(otdlDoc["overviewData"].HasMember("species"))
    speciesName = otdlDoc["overviewData"]["species"].GetString();
  else
    speciesName = parent->speciesName;
  unless(strcmp(speciesName, "nosuchspecies") == 0)
    genusList[genusName]++;

  //Var - optional, non-heritable
  if(otdlDoc["overviewData"].HasMember("var"))
   {
    varName = otdlDoc["overviewData"]["var"].GetString();
    char buf[MAX_SPECIES_PATH];
    snprintf(buf, MAX_SPECIES_PATH, "%s %s", speciesName, varName);
    (*genusSpeciesList[genusName])[buf] = this;
   }
  else // no variety
   {
    varName = NULL;
    (*genusSpeciesList[genusName])[speciesName] = this;
   }
  
  //version - mandatory, heritable
  if(otdlDoc["overviewData"].HasMember("version"))
    version.set(otdlDoc["overviewData"]["version"]);
  else
    version = parent->version;
   
  //maxHeight - mandatory, heritable
  if(otdlDoc["overviewData"].HasMember("maxHeight"))
    maxHeight  = otdlDoc["overviewData"]["maxHeight"].GetFloat()/mmPerSpaceUnit;
  else
    maxHeight = parent->maxHeight;

  //maxGirth - mandatory, heritable
  if(otdlDoc["overviewData"].HasMember("maxGirth"))
    maxRadius  = otdlDoc["overviewData"]["maxGirth"].GetFloat()/mmPerSpaceUnit/M_PI/2.0;
  else
    maxRadius = parent->maxRadius;

  //maxAge - mandatory, heritable
  if(otdlDoc["overviewData"].HasMember("maxAge"))
    maxAge  = otdlDoc["overviewData"]["maxAge"].GetFloat();
  else
    maxAge = parent->maxAge;

  // The wood object is heritable in it's entirety, so if we don't have one, we have
  // to copy everything over from the parent.
  
  unless(otdlDoc.HasMember("wood"))
   {
    stemRate          = parent->stemRate;
    initSapThickness  = parent->initSapThickness;
    initBarkThickness = parent->initBarkThickness;
    barkColorMap      = parent->barkColorMap;
   }
  else
   {
    // fill out the barkColorMap array (dedicated function for this)
    extractBarkColors(otdlDoc["wood"]["barkColors"]);

    // stemRate - mandatory, heritable
    if(otdlDoc["wood"].HasMember("stemRate"))
      stemRate  = otdlDoc["wood"]["stemRate"].GetFloat()/mmPerSpaceUnit;
    else
      stemRate  = parent->stemRate;
    
    // initSapThickness - mandatory, heritable
    if(otdlDoc["wood"].HasMember("initSapThickness"))
      initSapThickness  = otdlDoc["wood"]["initSapThickness"].GetFloat()/mmPerSpaceUnit;
    else
      initSapThickness  = parent->stemRate;

    // initBarkThickness - mandatory, heritable
    if(otdlDoc["wood"].HasMember("initBarkThickness"))
      initBarkThickness = otdlDoc["wood"]["initBarkThickness"].GetFloat()/mmPerSpaceUnit;
    else
      initBarkThickness  = parent->initBarkThickness;
   }
}


// =======================================================================================
// Destructor - currently should never be called as species stay around for the life of
// the program.

Species::~Species(void)
{
}


// =======================================================================================
// Braindead trunk growth model that assumes that growth follows a logistic function,
// specifically that a tree of age zero corresponds to -5, and that at maxAge/2 (for the
// species) the tree will have reached maxHeight/2 and maxRadius/2, which will correspond
// to +5 on the logistic curve x-axis.
// See https://en.wikipedia.org/wiki/Logistic_function
// for reference.

void Species::logisticGrowthModel(float age, float& radius, float& height)
{
  float xval = 16.0f*age/maxAge;
  float logisticVal = 1.0f/(1.0f + expf(-xval)) - 0.5f;
  radius = maxRadius*logisticVal;
  height = maxHeight*logisticVal;
  if(varName)
    LogGrowthModel("Growth Model for %s %s %s has x-val: %f, logisticVal: %f, "
                              "radius: %f, height %f\n", genusName, speciesName, varName,
                              xval, logisticVal, radius, height);
  else
    LogGrowthModel("Growth Model for %s %s has x-val: %f, logisticVal: %f, "
                              "radius: %f, height %f\n", genusName, speciesName,
                              xval, logisticVal, radius, height);
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
// Return a barkColor from the array based on the age of the bark in question.

unsigned Species::getBarkColor(float age)
{
  auto iter = barkColorMap.lower_bound(age);
  return iter->second;
}


// =======================================================================================
// Read a buffer containing an OTDL description of the type of tree we are.

Document& Species::readOTDLFromBuf(char* buf, char* sourceName, Species** parent)
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
  if(!validateOTDL(*doc, sourceName, parent))
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

bool Species::validateOverviewData(Document& doc, JSONStructureChecker* jCheck,
                                                                          Species* parent)
{
  bool   retVal       = true;
  Value& overviewData = doc["overviewData"];
  char* logObjectName = (char*)"overviewData";
  
  // Genus - mandatory, inheritable
  if(jCheck->validateStringMemberExists(overviewData, logObjectName, (char*)"genus"))
    retVal &= jCheck->validateGenusName(logObjectName, overviewData["genus"].GetString());
  else if(parent)
   {
    LogOTDLDetails("Inheriting genus from parent in %s\n", jCheck->sourcePhrase);
   }
  else
   {
    LogOTDLValidity("No genus available for %s\n", jCheck->sourcePhrase);
    retVal = false;
   }
   
  // Species - mandatory, inheritable
  if(jCheck->validateStringMemberExists(overviewData, logObjectName, (char*)"species"))
    retVal &= jCheck->validateSpeciesName(logObjectName, overviewData["species"].GetString());
  else if(parent)
   {
    LogOTDLDetails("Inheriting species from parent in %s\n", jCheck->sourcePhrase);
   }
  else
    retVal = false;
    
  // variety - optional, non-heritable
  retVal &= jCheck->validateOptionalStringMember(overviewData, logObjectName, (char*)"var");

  // fileTime - mandatory, non-heritable
  retVal &= jCheck->validateFileTime(overviewData);

  // maxHeight - mandatory, heritable
  if(overviewData.HasMember("maxHeight"))
   {
    unless(overviewData["maxHeight"].IsNumber())
     {
      LogOTDLValidity("maxHeight is not numeric in %s\n", jCheck->sourcePhrase);
      retVal = false;
     }
   }
  else if(parent)
   {
    LogOTDLDetails("Inheriting maxHeight from parent in %s\n", jCheck->sourcePhrase);
   }
  else
   {
    LogOTDLValidity("No maxHeight available for %s\n", jCheck->sourcePhrase);
    retVal = false;
   }
  
  // maxGirth - mandatory, heritable
  if(overviewData.HasMember("maxGirth"))
   {
    unless(overviewData["maxGirth"].IsNumber())
     {
      LogOTDLValidity("maxGirth is not numeric in %s\n", jCheck->sourcePhrase);
      retVal = false;
     }
   }
  else if(parent)
   {
    LogOTDLDetails("Inheriting maxGirth from parent in %s\n", jCheck->sourcePhrase);
   }
  else
   {
    LogOTDLValidity("No maxGirth available for %s\n", jCheck->sourcePhrase);
    retVal = false;
   }

  // maxAge - mandatory, heritable
  if(overviewData.HasMember("maxAge"))
   {
    unless(overviewData["maxAge"].IsNumber())
     {
      LogOTDLValidity("maxGirth is not numeric in %s\n", jCheck->sourcePhrase);
      retVal = false;
     }
   }
  else if(parent)
   {
    LogOTDLDetails("Inheriting maxGirth from parent in %s\n", jCheck->sourcePhrase);
   }
  else
   {
    LogOTDLValidity("No maxGirth available for %s\n", jCheck->sourcePhrase);
    retVal = false;
   }

  // version - mandatory, heritable
  if(overviewData.HasMember("version"))
    retVal &= jCheck->validateVersion(overviewData);
  else if(parent)
   {
    LogOTDLDetails("Inheriting version from parent in %s\n", jCheck->sourcePhrase);
   }
  else
   {
    LogOTDLValidity("No version available for %s\n", jCheck->sourcePhrase);
    retVal = false;
   }

  // authors - optional, heritable
  // XX no internal representation of authors yet
  retVal &= jCheck->validateOptionalStringOrArrayString(overviewData, logObjectName,
                                                                  (char*)"authors");
  // common names object - optional, hereditable
  // XX no internal representation of commonNames yet
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
// Validate the wood section of an OTDL object.  This is the version for a standalone
// object with no parent.

bool Species::validateWood(Document& doc, JSONStructureChecker* jCheck, Species* parent)
{
  bool   retVal       = true;
  Value& woodObject = doc["wood"];
  
  // if we get in here, there is likely at least some wood data in the child,
  // but there also may be a parent

  // stemRate - mandatory, heritable
  if(woodObject.HasMember("stemRate"))
   {
    unless(woodObject["stemRate"].IsNumber())
     {
      LogOTDLValidity("stemRate non-numeric in %s\n", jCheck->sourcePhrase);
      retVal = false;
     }
   }
  else if(parent)
   {
    LogOTDLDetails("Inheriting stemRate from parent in %s\n", jCheck->sourcePhrase);
   }
  else
   {
    LogOTDLValidity("No stemRate available for %s\n", jCheck->sourcePhrase);
    retVal = false;
   }

  // initSapThickness - mandatory, heritable
  if(woodObject.HasMember("initSapThickness"))
   {
    unless(woodObject["initSapThickness"].IsNumber())
     {
      LogOTDLValidity("initSapThickness is not numeric in %s\n", jCheck->sourcePhrase);
      retVal = false;
     }
   }
  else if(parent)
   {
    LogOTDLDetails("Inheriting initSapThickness from parent in %s\n", jCheck->sourcePhrase);
   }
  else
   {
    LogOTDLValidity("No initSapThickness available for %s\n", jCheck->sourcePhrase);
    retVal = false;
   }

  // initBarkThickness - mandatory, heritable
  if(woodObject.HasMember("initBarkThickness"))
   {
    unless(woodObject["initBarkThickness"].IsNumber())
     {
      LogOTDLValidity("initBarkThickness is not numeric in %s\n", jCheck->sourcePhrase);
      retVal = false;
     }
   }
  else if(parent)
   {
    LogOTDLDetails("Inheriting initBarkThickness from parent in %s\n", jCheck->sourcePhrase);
   }
  else
   {
    LogOTDLValidity("No initBarkThickness available for %s\n", jCheck->sourcePhrase);
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
// Validate the parent section of an OTDL object.

Species* Species::getParent(Document& doc, JSONStructureChecker* jCheck)
{
  Species* parent = Species::getSpeciesByPath(doc["parent"].GetString());
  return parent;
}


// =======================================================================================
// Validate OTDL/JSON structure of the type of tree we are.

bool Species::validateOTDL(Document& doc, char* sourceName, Species** parentBack)
{
  bool retVal = true;
  char phrase[128];
  snprintf(phrase, 128, "OTDL object %s", sourceName);
  JSONStructureChecker* jCheck = new JSONStructureChecker(phrase, OTDL);
  Species* parent = NULL;
  
  if(doc.HasMember("parent"))
   {
    unless(doc["parent"].IsString())
     {
      LogOTDLValidity("Invalid parent path in %s\n", phrase);
      retVal = false;
     }
    else
     {
      parent = getParent(doc, jCheck);
      if(!parent)
        return false;
     }
   }
  else
     LogOTDLDetails("No inheritance from parent in %s\n", phrase);
  *parentBack = parent;
  
  // overviewData is mandatory even if an object has a parent, as it needs to
  // be at least somewhat different in naming.
  unless(doc.HasMember("overviewData") && doc["overviewData"].IsObject())
   {
    LogOTDLValidity("No overviewData in %s\n", phrase);
    retVal = false;
   }
  else
    retVal &= validateOverviewData(doc, jCheck, parent);

  // wood is mandatory but heritable, so we might be getting it from parent
  if(doc.HasMember("wood"))
   {
    unless(doc["wood"].IsObject())
     {
      LogOTDLValidity("Wood is not object in %s\n", phrase);
      retVal = false;
     }
    else // hopefully there is at least some wood data here.
      retVal &= validateWood(doc, jCheck, parent);
   }
  else if(parent)
   {
    // there is no wood data in child, so get it from the parent
    LogOTDLDetails("Inheriting wood data from parent in %s\n", phrase);
   }
  else
   {
    // we are just SOL.
    LogOTDLValidity("No wood data in %s\n", phrase);
    retVal = false;
   }
     
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

Species* Species::getSpeciesByPath(const char* speciesPath)
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

Species* Species::loadLocalOTDLEntry(const char* speciesPath)
{
  const PmodConfig& config = PmodConfig::getConfig();
  char path[256];
  snprintf(path, 256, "%s/%s/default.otdl", config.speciesDirectory, speciesPath);
  unless(regularFileExists(path))
    return NULL;
  unsigned bufSize;
  char* buf = loadFileToBuf(path, &bufSize);
  Species* parent;
  Document& doc = readOTDLFromBuf(buf, path, &parent);
  
  // At this point we are confident we have valid OTDL, so create the new species
  Species* newSpecies = new Species(doc, parent);
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
  bufprintf("   \"version\":  %s,\n", currentOTDLVersion.printJSON());
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
  if(varName)
    snprintf(title, MAX_SPECIES_PATH+16, "Species: %s %s %s", genusName,
                                                                speciesName, varName);
  else
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
