// Copyright Staniford Systems.  All Rights Reserved.  Jan 2021 -
// This class is a container for the factors governing the evolution of a particular
// species (mainly extracted from an OTDL JSON object).

// NOTE WELL.  Numeric values in OTDL are expressed in mm, but for uniformity in graphic
// calculations, internally we must store them in spaceUnits.

#include "Species.h"
#include "loadFileToBuf.h"
#include "Global.h"
#include "PmodConfig.h"
#include "PmodDesign.h"
#include "JSONStructureChecker.h"
#include "Logging.h"
#include "LeafModel.h"
#include "HttpDebug.h"
#include <dirent.h>
#include <err.h>


// =======================================================================================
// Static variables etc

using namespace rapidjson;

unsigned short Species::speciesCount = 0u;
SpeciesList* Species::cachedSpecies = new SpeciesList;
std::unordered_map<std::string, unsigned> Species::genusList;
std::unordered_map<std::string, SpeciesList*> Species::genusSpeciesList;


// =======================================================================================
/// @brief Constructor.
/// @param otdlDoc A reference to the rapidson::Document for the OTDL file describing
/// this species.  See docs/open-tree-description-language.md for detailes on the format.
/// @param source A pointer to a C-string with the name of the OTDL document.

Species::Species(Document& otdlDoc, char* source):
                                sourceName(source),
                                parent(NULL),
                                jCheck(NULL)
{
  if(validateOTDL(otdlDoc))
    validOTDL = true;
  else
   {
    validOTDL = false;
    return;
   }
    
  initializeOverViewData(otdlDoc["overviewData"]);
  initializeWoodData(otdlDoc);
  
  // foliage LeafModel
  LeafModel* leafParent = NULL;
  char phrase[128];
  snprintf(phrase, 128, "OTDL object %s", sourceName);
  if(parent)
    leafParent = parent->foliage;
  foliage = new LeafModel(otdlDoc, leafParent, jCheck, phrase);
  unless(foliage->isValid())
    validOTDL = false;
}


// =======================================================================================
/// @brief Subroutine for the constructor to handle the overviewData in the OTDL doc.
/// @param overviewData The rapidjson::Value for the overviewData JSON object inside the
/// OTDL doc.

void Species::initializeOverViewData(Value& overviewData)
{
  // Genus - mandatory, heritable
  if(overviewData.HasMember("genus"))
    genusName = overviewData["genus"].GetString();
  else
    genusName = parent->genusName;
  if(genusSpeciesList.find(genusName) == genusSpeciesList.end())
    genusSpeciesList[genusName] = new SpeciesList;

  //Species - mandatory, heritable
  if(overviewData.HasMember("species"))
    speciesName = overviewData["species"].GetString();
  else
    speciesName = parent->speciesName;
  unless(strcmp(speciesName, "nosuchspecies") == 0)
    genusList[genusName]++;

  //Var - optional, non-heritable
  if(overviewData.HasMember("var"))
   {
    varName = overviewData["var"].GetString();
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
  if(overviewData.HasMember("version"))
    version.set(overviewData["version"]);
  else
    version = parent->version;
   
  //maxHeight - mandatory, heritable
  if(overviewData.HasMember("maxHeight"))
    maxHeight  = overviewData["maxHeight"].GetFloat()/mmPerSpaceUnit;
  else
    maxHeight = parent->maxHeight;

  //maxWidth - mandatory, heritable
  if(overviewData.HasMember("maxWidth"))
    maxWidth  = overviewData["maxWidth"].GetFloat()*maxHeight;
  else
    maxWidth = parent->maxWidth;

  //maxGirth - mandatory, heritable
  if(overviewData.HasMember("maxGirth"))
    maxRadius  = overviewData["maxGirth"].GetFloat()/mmPerSpaceUnit/M_PI/2.0;
  else
    maxRadius = parent->maxRadius;

  //maxAge - mandatory, heritable
  if(overviewData.HasMember("maxAge"))
    maxAge  = overviewData["maxAge"].GetFloat();
  else
    maxAge = parent->maxAge;
}


// =======================================================================================
/// @brief Subroutine for the constructor to handle the wood data in the OTDL doc.
/// @param otdlDoc A reference to the rapidson::Document for the OTDL file describing
/// this species.

void Species::initializeWoodData(Document& otdlDoc)
{
  unless(otdlDoc.HasMember("wood"))
   {
    // The wood object is heritable in it's entirety, so if we don't have one, we have
    // to copy everything over from the parent.
    stemRate          = parent->stemRate;
    branchSpacing     = parent->branchSpacing;
    branchFactor      = parent->branchFactor;
    branchAngle       = parent->branchAngle;
    branchSpiralAngle = parent->branchSpiralAngle;
    initSapThickness  = parent->initSapThickness;
    initBarkThickness = parent->initBarkThickness;
    barkColorMap      = parent->barkColorMap;
   }
  else
   {
    // We have a wood object.  We might be getting some pieces from it, but
    // inheriting others.
    Value& woodData = otdlDoc["wood"];
    
    // barkColors - mandatory, heritable
    if(woodData.HasMember("barkColors"))
      extractBarkColors(woodData["barkColors"]);
    else
      barkColorMap      = parent->barkColorMap;

    // stemRate - mandatory, heritable
    if(woodData.HasMember("stemRate"))
      stemRate  = woodData["stemRate"].GetFloat()/mmPerSpaceUnit;
    else
      stemRate  = parent->stemRate;

    // branchSpacing - mandatory, heritable
    if(woodData.HasMember("branchSpacing"))
      branchSpacing  = woodData["branchSpacing"].GetFloat()/mmPerSpaceUnit;
    else
      branchSpacing  = parent->branchSpacing;

    // branchFactor - mandatory, heritable
    if(woodData.HasMember("branchFactor"))
      branchFactor  = woodData["branchFactor"].GetInt();
    else
      branchFactor  = parent->branchFactor;
    
    // branchAngle - mandatory, heritable
    if(woodData.HasMember("branchAngle"))
      branchAngle  = woodData["branchAngle"].GetFloat();
    else
      branchAngle  = parent->branchAngle;

    // branchSpiralAngle - mandatory, heritable
    if(woodData.HasMember("branchSpiralAngle"))
      branchSpiralAngle  = woodData["branchSpiralAngle"].GetFloat();
    else
      branchSpiralAngle  = parent->branchSpiralAngle;

    // initSapThickness - mandatory, heritable
    if(woodData.HasMember("initSapThickness"))
      initSapThickness  = woodData["initSapThickness"].GetFloat()/mmPerSpaceUnit;
    else
      initSapThickness  = parent->initSapThickness;

    // initBarkThickness - mandatory, heritable
    if(woodData.HasMember("initBarkThickness"))
      initBarkThickness = woodData["initBarkThickness"].GetFloat()/mmPerSpaceUnit;
    else
      initBarkThickness  = parent->initBarkThickness;
   }
}


// =======================================================================================
/// @brief Destructor currently should never be called as species stay around for the 
/// life of the program.

Species::~Species(void)
{
}


// =======================================================================================
/// @brief Simple tree growth model following a logistic curve.
///
/// Braindead trunk growth model that assumes that growth follows a logistic 
/// function, specifically that a tree of age zero corresponds to -5, and that at 
/// maxAge/2 (for the species) the tree will have reached maxHeight/2 and maxRadius/2, 
/// which will correspond to +5 on the logistic curve x-axis. See
/// https://en.wikipedia.org/wiki/Logistic_function for reference.
/// @param age The float age of the tree in years
/// @param radius A reference to a float to store the new radius.
/// @param height A reference to a float to store the new height

void Species::logisticGrowthModel(float age, float& radius, float& height)
{
  float xval = 16.0f*age/maxAge;
  float logisticVal = 1.0f/(1.0f + expf(-xval)) - 0.5f;
  radius = maxRadius*logisticVal;
  height = maxHeight*logisticVal;
  if(varName)
   {
    LogGrowthModel("Growth Model for %s %s %s has x-val: %f, logisticVal: %f, "
                              "radius: %f, height %f\n", genusName, speciesName, varName,
                              xval, logisticVal, radius, height);
   }
  else
   {
    LogGrowthModel("Growth Model for %s %s has x-val: %f, logisticVal: %f, "
                              "radius: %f, height %f\n", genusName, speciesName,
                              xval, logisticVal, radius, height);
   }
}


// =======================================================================================
/// @brief Extract and store the bark colors for later use in visualization.
/// @param colorsArray The section of OTDL/JSON with the array associated with the 
/// barkColors tag.

void Species::extractBarkColors(Value& colorsArray)
{
  int barkColorSize = colorsArray.Size();

  for(int i=0; i<barkColorSize; i++)
    barkColorMap[colorsArray[i]["ages"][1].GetFloat()]
                                                  = colorFromRGBArray(colorsArray[i]["rgb"]);
}


// =======================================================================================
/// @brief Get a barkColor from the array based on the age of the bark in question.
/// @returns An unsigned represenation of the RGBA color appropriate to the supplied age.
/// @param age A floating point age in years.

unsigned Species::getBarkColor(float age)
{
  auto iter = barkColorMap.lower_bound(age);
  return iter->second;
}


// =======================================================================================
/// @brief Read a buffer containing an OTDL description of the type of tree we are.
/// @returns A reference to the rapidjson::document for the whole OTDL document after
/// the JSON has been parsed.
/// @param buf A pointer to a character array containing the complete OTDL document text.
/// @param sourceName The name of the file for error reporting and logging.

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
 
  return *doc;
}


// =======================================================================================
/// @brief Function to check the syntax of the mandatory OTDL commonNames object 
/// inside overviewData.
/// @returns True if the syntax is correct, false otherwise.
/// @param containObj Should be a reference to the rapidjson::Value for the overviewData
/// object.

bool Species::validateCommonNames(Value& containObj)
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
/// @brief Validate the overviewData section of an OTDL object.
/// @returns True if the syntax is correct, false otherwise.
/// @param doc A reference to the rapidson::Document for the OTDL file describing
/// this species.

bool Species::validateOverviewData(Document& doc)
{
  bool   retVal       = true;
  Value& overviewData = doc["overviewData"];
  char* logObjectName = (char*)"overviewData";
  
  // Genus - mandatory, heritable
  retVal &= jCheck->checkMandatoryHeritableStringValue(overviewData, parent, (char*)"genus");
  if(overviewData.HasMember("genus"))
    retVal &= jCheck->validateGenusName(logObjectName, overviewData["genus"].GetString());
    
  // Species - mandatory, heritable
  retVal &= jCheck->checkMandatoryHeritableStringValue(overviewData, parent, (char*)"species");
  if(overviewData.HasMember("species"))
    retVal &= jCheck->validateSpeciesName(logObjectName, overviewData["species"].GetString());
    
  // variety - optional, non-heritable
  retVal &= jCheck->validateOptionalStringMember(overviewData, logObjectName, (char*)"var");

  // fileTime - mandatory, non-heritable
  retVal &= jCheck->validateFileTime(overviewData);

  // maxHeight - mandatory, heritable
  retVal &= jCheck->checkMandatoryHeritableFloatValue(overviewData,
                                                      parent, (char*)"maxHeight");

  // maxWidth - mandatory, heritable
  retVal &= jCheck->checkMandatoryHeritableFloatValue(overviewData,
                                                      parent,  (char*)"maxWidth");

  // maxGirth - mandatory, heritable
  retVal &= jCheck->checkMandatoryHeritableFloatValue(overviewData,
                                                      parent,  (char*)"maxGirth");
  
  // maxAge - mandatory, heritable
  retVal &= jCheck->checkMandatoryHeritableFloatValue(overviewData,
                                                      parent,  (char*)"maxAge");
  
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
  retVal &= validateCommonNames(overviewData);

  return retVal;
}


// =======================================================================================
/// @brief Validate the bark textures section of an OTDL object.
/// @todo Stub implementation that always returns true.

bool Species::validateBarkTextures(Value& obj)
{
  bool   retVal       = true;
  
  return retVal;
}


// =======================================================================================
/// @brief Validate the bark colors section of an OTDL object.
/// @returns True if the syntax is correct, false otherwise.
/// @param colorsArray The section of OTDL/JSON with the array associated with the 
/// barkColors tag.

bool Species::validateBarkColors(Value& colorsArray)
{
  bool   retVal       = true;
  
  unless(colorsArray.IsArray())
   {
    LogOTDLValidity("barkColors is not array in %s\n", jCheck->sourcePhrase);
    return false;
   }

  int N = colorsArray.Size();
  unless(N > 0)
   {
    LogOTDLValidity("barkColors array is empty in %s\n", jCheck->sourcePhrase);
    return false;
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
/// @brief Validate the wood section of an OTDL object.  
/// 
/// @returns True if the syntax is correct, false otherwise.
/// @param doc A reference to the rapidson::Document for the OTDL file describing
/// this species.

bool Species::validateWood(Document& doc)
{
  bool   retVal       = true;
  Value& woodObject = doc["wood"];
  
  // if we get in here, there is likely at least some wood data in the child,
  // but there also may be a parent

  // stemRate - mandatory, heritable
  retVal &= jCheck->checkMandatoryHeritableFloatValue(woodObject,
                                                      parent,  (char*)"stemRate");

  // branchSpacing - mandatory, heritable
  retVal &= jCheck->checkMandatoryHeritableFloatValue(woodObject,
                                                      parent,  (char*)"branchSpacing");

  // branchFactor - mandatory, heritable
  retVal &= jCheck->checkMandatoryHeritableUnsignedValue(woodObject,
                                                         parent,  (char*)"branchFactor");

  // branchAngle - mandatory, heritable
  retVal &= jCheck->checkMandatoryHeritableFloatValue(woodObject,
                                                      parent,  (char*)"branchAngle");

  // branchSpiralAngle - mandatory, heritable
  retVal &= jCheck->checkMandatoryHeritableFloatValue(woodObject,
                                                      parent,  (char*)"branchSpiralAngle");

  // initSapThickness - mandatory, heritable
  retVal &= jCheck->checkMandatoryHeritableFloatValue(woodObject,
                                                      parent,  (char*)"initSapThickness");
  
  // initBarkThickness - mandatory, heritable
  retVal &= jCheck->checkMandatoryHeritableFloatValue(woodObject,
                                                      parent,  (char*)"initBarkThickness");
  
  // barkColors - mandatory, heritable
  if(woodObject.HasMember("barkColors"))
    retVal &= validateBarkColors(woodObject["barkColors"]);
  else if(parent)
   {
    LogOTDLDetails("Inheriting barkColors from parent in %s\n", jCheck->sourcePhrase);
   }
  else
   {
    LogOTDLValidity("No barkColors array for %s\n", jCheck->sourcePhrase);
    retVal = false;
   }

  // barkTextures - optional
  if(woodObject.HasMember("barkTextures"))
   {
    unless(woodObject["barkTextures"].IsObject())
     {
      LogOTDLValidity("barkTextures is not object in %s\n", jCheck->sourcePhrase);
      retVal = false;
     }
    else
      retVal &= validateBarkTextures(woodObject["barkTextures"]);
  }

  return retVal;
}


// =======================================================================================
/// @brief Validate OTDL/JSON structure of the type of tree we are.
///
/// This coordinates the overall process of validating OTDL.  It relies on different
/// subroutines above to check the individual sections.
/// @returns True if the syntax is correct, false otherwise.
/// @param doc A reference to the rapidson::Document for the OTDL file describing
/// this species.

bool Species::validateOTDL(Document& doc)
{
  bool retVal = true;
  char phrase[128];
  snprintf(phrase, 128, "OTDL object %s", sourceName);
  jCheck = new JSONStructureChecker(phrase, OTDL);
  
  if(doc.HasMember("parent"))
   {
    unless(doc["parent"].IsString())
     {
      LogOTDLValidity("Invalid parent path in %s\n", phrase);
      retVal = false;
     }
    else
     {
      parent = Species::getSpeciesByPath(doc["parent"].GetString());
      if(!parent)
        return false;
     }
   }
  else
    LogOTDLDetails("No inheritance from parent in %s\n", phrase);
  
  // overviewData is mandatory even if an object has a parent, as it needs to
  // be at least somewhat different in naming.
  unless(doc.HasMember("overviewData") && doc["overviewData"].IsObject())
   {
    LogOTDLValidity("No overviewData in %s\n", phrase);
    retVal = false;
   }
  else
    retVal &= validateOverviewData(doc);

  // wood is mandatory but heritable, so we might be getting it from parent
  if(doc.HasMember("wood"))
   {
    unless(doc["wood"].IsObject())
     {
      LogOTDLValidity("Wood is not object in %s\n", phrase);
      retVal = false;
     }
    else // hopefully there is at least some wood data here.
      retVal &= validateWood(doc);
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
     
  delete jCheck; jCheck = NULL;
  return retVal;
}


// =======================================================================================
/// @brief Static function to find the entry for a particular "genus/species".
/// 
/// Function will first check our static data structures, loading it from elsewhere if 
/// it's not already in memory.
/// @returns A pointer to the Species entry.
/// @param speciesPath A C-string with the genus/species etc path of the species.

Species* Species::getSpeciesByPath(const char* speciesPath)
{
  if(cachedSpecies->count(speciesPath))
   {
    LogOTDLFileSearch("Species %s is cached in memory.\n", speciesPath);
    return (*cachedSpecies)[speciesPath];
   }
  
  // Handle the case where the species has not been seen before.
  // First Check local OTDL directories
  Species* species;
  if( (species = loadLocalOTDLEntry(speciesPath)) )
   {
    (*cachedSpecies)[speciesPath] = species;
    return species;
   }
  
  // XX should go out to central web repository
  err(-1, "Couldn't find species %s\n", speciesPath);
  
  return NULL;
}


// =======================================================================================
/// @brief Static function to find an OTDL file in a directory.
/// @param path A C-string containing the directory path to search, to which we will 
/// append the name of the OTDL file we find.
/// @param pathSize The maximum allowable storage in the path variable.
/// @todo We currently just return the first one found.

void Species::findOTDLFileName(char* path, unsigned pathSize)
{
  bool found = false;
  
  // Open the directory
  DIR* dir = opendir(path);
  if(!dir)
    err(-1, "Couldn't open directory %s in findOTDLFileName", path);

  LogOTDLFileSearch("Checking %s for OTDL files.\n", path);

  // Traverse it looking for the file
  struct dirent* dirEntry;
  while( (dirEntry = readdir(dir)) )
   {
    unless(dirEntry->d_type == DT_REG)   // ignore everything but regular files
     {
      LogOTDLFileSearch("%s is not a regular file.\n", dirEntry->d_name);
      continue;
     }
    if(dirEntry->d_name[0]  == '.')  // ignore hidden files
     {
      LogOTDLFileSearch("%s is a hidden file.\n", dirEntry->d_name);
      continue;
     }
    char* dot = rindex(dirEntry->d_name, '.');
    unless(dot)
     {
      LogOTDLFileSearch("%s has no extension.\n", dirEntry->d_name);
      continue;
     }
    if(*(dot+1) == '\0')
     {
      LogOTDLFileSearch("%s ends in '.'.\n", dirEntry->d_name);
      continue;
     }
    unless(strcmp(dot+1, "otdl") == 0)
     {
      LogOTDLFileSearch("%s has wrong extension.\n", dirEntry->d_name);
      continue;
     }
    // if we get this far, we've found an OTDL file.
    if(strlen(path) + strlen(dirEntry->d_name) > pathSize-1)
      err(-1, "Path too long for %s in findOTDLFileName.", dirEntry->d_name);
    strcat(path, dirEntry->d_name);
    LogOTDLFileSearch("Found OTDL file %s\n", path);
    found = true;
    break;
   }
  closedir(dir);
  if(!found)
    err(-1, "Couldn't find OTDL file in %s\n", path);
}


// =======================================================================================
/// @brief Static function to try and load a particular "genus/species" from local disk
/// @returns A pointer to the Species entry, if found, else NULL.
/// @param speciesPath A C-string with the genus/species etc path of the species.

Species* Species::loadLocalOTDLEntry(const char* speciesPath)
{
  const PmodConfig& config = PmodConfig::getConfig();
  char path[256];
  snprintf(path, 256, "%s/%s/", config.speciesDirectory, speciesPath);
  Species::findOTDLFileName(path, 256);
  unless(regularFileExists(path))
    return NULL;
  unsigned bufSize;
  char* buf = loadFileToBuf(path, &bufSize);
  Document& doc = readOTDLFromBuf(buf, path);
  
  // At this point we are confident we have valid OTDL, so create the new species
  Species* newSpecies = new Species(doc, path);
  
  if(newSpecies->isValid())
    return newSpecies;
  else
    return NULL;
}


// =======================================================================================
/// @brief Write out the OTDL object to a buffer (eg used by diagnosticHTML for Species 
/// page).
///
/// NOTE WELL.  Numeric values in OTDL are expressed in mm, but for uniformity in graphic
/// calculations, internally we must store them in spaceUnits.
/// @returns The number of bytes written into the buffer
/// @param buf A char array where the OTDL text should be written.
/// @param bufSize The maximum size of the buffer.

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
  bufprintf("   \"maxWidth\":     \"%.2f\",\n", maxWidth/maxHeight);
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
  bufprintf("   \"branchSpacing\":           \"%f\",\n", branchSpacing*mmPerSpaceUnit);
  bufprintf("   \"branchFactor\":           \"%u\",\n", branchFactor);
  bufprintf("   \"branchAngle\":           \"%.0f\",\n", branchAngle);
  bufprintf("   \"branchSpiralAngle\":           \"%.0f\",\n", branchSpiralAngle);
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

  buf += foliage->writeOTDL(buf, end-buf);

  // closing
  bufprintf("}\n");
  LogHTTPDetails("Wrote %ld bytes of OTDL in HTTP response.\n", bufSize - (end-buf));
  return (bufSize - (int)(end-buf));
}


// =======================================================================================
/// @brief Tell callers our name at runtime.
/// @returns A const pointer to a C-string with the name.

const char* Species::objectName(void)
{
  static char* name = (char*)"Species";
  return name;
}


// =======================================================================================
/// @brief Provide a section about this particular species in a diagnostic page
/// @returns True if the desired HTML was written correctly, false if we ran out of space.
/// @param serv The HTTP Debug server

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
/// @brief Simple static function just to find the right species, and then return the 
/// diagnosticHTML for it.
/// @returns True if the desired HTML was written correctly, false if we ran out of space.
/// @param serv The HTTP Debug server
/// @param path The genus/species/var for the species of interest

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
