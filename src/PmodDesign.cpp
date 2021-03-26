// Copyright Staniford Systems.  All Rights Reserved.  Apr 2020 -
// Main object that encapsulates the design of a given piece of land.  This is basically
// the contents of the main OLDF file given via -d.  Generally the rest of the command
// line is captured in PmodConfig, not in here.
// See docs/open-landscape-description-format.md for the file format.

#define PMOD_DESIGN_IMPLEMENTATION

#include "PmodDesign.h"
#include <stdexcept>
#include <err.h>
#include <math.h>
#include "loadFileToBuf.h"
#include "Logging.h"
#include "GlobalMacros.h"
#include "Tree.h"


using namespace rapidjson;

PmodDesign* PmodDesign::design = NULL;

// These unit things are used all over the place, so we have them in the global namespace
// for syntactic convenience (they are externed in PmodDesign.h)
float          mmPerSpaceUnit;
char*          spaceUnitName;
char           spaceUnitAbbr;


// =======================================================================================
// Constructor: parse the json file

PmodDesign::PmodDesign(void):
                  writeFile(NULL),
                  metricUnits(false),
                  jCheck(NULL)
{
  // Constructor should only be called once at startup.  Everyone else gets us via
  // getDesign()
  if(design)
    return;
  else
    design = this;
  unsigned bufSize;
  const PmodConfig& config = PmodConfig::getConfig();
  char* buf = loadFileToBuf(config.designFileName, &bufSize);
  
  ParseResult ok = doc.ParseInsitu<kParseCommentsFlag>(buf);
  if (!ok)
   {
    fprintf(stderr, "JSON parse error on OLDF file: %s (%u)\n",
            GetParseError_En(ok.Code()), (unsigned)(ok.Offset()));
    exit(1);
   }
  if(!doc.IsObject())
    err(-1, "Base of OLDF file %s is not JSON object.\n", config.designFileName);
  if(!validateOLDF())
    err(-1, "Invalid OLDF file %s - see log for details\n", config.designFileName);
  boundary.setFromOLDF(doc["boundaries"]);
}


// =======================================================================================
// Function to check the spatial units.

bool PmodDesign::validateSpaceUnits(Value& introductoryData)
{
  bool retVal = true;
#if defined LOG_OLDF_VALIDITY || defined LOG_OLDF_DETAILS
  const PmodConfig& config = PmodConfig::getConfig();
#endif
  
  if(introductoryData.HasMember("spaceUnits") && introductoryData["spaceUnits"].IsString())
   {
    const char* token = introductoryData["spaceUnits"].GetString();
    if(strcmp(token, "feet") == 0)
     {
      LogOLDFDetails("Using feet as spatial units in OLDF file %s\n", config.designFileName);
      metricUnits     = false;
      mmPerSpaceUnit  = MM_PER_FOOT;
      spaceUnitName   = (char*)"feet";
      spaceUnitAbbr   = '\'';
     }
    else if(strcmp(token, "meters") == 0 )
     {
      LogOLDFDetails("Using meters as spatial units in OLDF file %s\n", config.designFileName);
      metricUnits     = true;
      mmPerSpaceUnit  = 1000.0f;
      spaceUnitName   = (char*)"meters";
      spaceUnitAbbr   = 'm';
     }
    else
     {
      LogOLDFValidity("Bad introductoryData:spaceUnits %s, in OLDF file %s\n",
                                                            token, config.designFileName);
      retVal = false;
     }
   }
  else
   {
    LogOLDFValidity("No introductoryData:spaceUnits token in OLDF file %s\n",
                                                                    config.designFileName);
    retVal = false;
   }

 return retVal;
}


// =======================================================================================
// Function to check the base year.

bool PmodDesign::validateBaseYear(Value& introductoryData)
{
  bool retVal = true;
#if defined LOG_OLDF_VALIDITY || defined LOG_OLDF_DETAILS
  const PmodConfig& config = PmodConfig::getConfig();
#endif
  
  if(introductoryData.HasMember("baseYear") && introductoryData["baseYear"].IsInt())
   {
    baseYear = introductoryData["baseYear"].GetInt();
    if(!(baseYear >= 1500) && (baseYear <= 2500))
     {
      LogOLDFValidity("Out of range introductoryData:baseYear %d, in OLDF file %s\n",
                                                            baseYear, config.designFileName);
      retVal = false;
     }
    else
      LogOLDFDetails("baseYear is %d in OLDF file %s\n", baseYear, config.designFileName);
   }
  else
   {
    LogOLDFValidity("No valid introductoryData:baseYear in OLDF file %s\n",
                                                                    config.designFileName);
    retVal = false;
   }

 return retVal;
}


// =======================================================================================
// Function to check if a particular piece of JSON is a correct Unix timeval expressed
// as an array of [<sec>, <usec>].

bool PmodDesign::validateJSONUnixTime(Value& object, char* objName)
{
  bool retVal = true;
#if defined LOG_OLDF_VALIDITY //|| defined LOG_OLDF_DETAILS
  const PmodConfig& config = PmodConfig::getConfig();
#endif
  
  if(object.IsArray())
   {
    if(object.Size() == 2)
     {
      unless(object[0].IsInt())
       {
        LogOLDFValidity("%s time field is not integer seconds in OLDF file %s\n",
                                                    objName, config.designFileName);
        retVal = false;
       }
      unless(object[1].IsInt())
       {
        LogOLDFValidity("%s time field is not integer microseconds in OLDF file %s\n",
                                                    objName, config.designFileName);
        retVal = false;
       }
      int uSec = object[1].GetInt();
      unless(uSec >= 0 && uSec < 1000000)
       {
        LogOLDFValidity("%s time field out of range microseconds in OLDF file %s\n",
                                                    objName, config.designFileName);
        retVal = false;
       }
     }
    else
     {
      LogOLDFValidity("%s array is wrong size %d in OLDF file %s\n",
                                          objName, object.Size(), config.designFileName);
     }
   }
  else
   {
    LogOLDFValidity("%s is not array in OLDF file %s\n", objName, config.designFileName);
    retVal = false;
   }
  
 return retVal;
}


// =======================================================================================
// Function to check the structure of the OLDF introductoryData object.

bool PmodDesign::validateIntroductoryData(void)
{
  bool              retVal              = true;
  Value&            introductoryData    = doc["introductoryData"];

  retVal &= validateSpaceUnits(introductoryData);
  retVal &= validateBaseYear(introductoryData);
  retVal &= jCheck->validateVersion(introductoryData);
  retVal &= jCheck->validateFileTime(introductoryData);
  fileTime.set(introductoryData["fileTime"][0].GetInt(),
                                    introductoryData["fileTime"][1].GetInt());

  retVal &= jCheck->validateStringMemberExists(introductoryData,
                                    (char*)"introductoryData", (char*)"software");
  retVal &= jCheck->validateOptionalStringMember(introductoryData,
                                    (char*)"introductoryData", (char*)"softwareVersion");
  retVal &= jCheck->validateOptionalStringMember(introductoryData,
                                    (char*)"introductoryData", (char*)"author");
  return retVal;
}


// =======================================================================================
// Function to check the OLDF texture inside landSurface (note that it is optional).

bool PmodDesign::validateTexture(Value& landSurface)
{
  bool retVal = true;
#if defined LOG_OLDF_VALIDITY || defined LOG_OLDF_DETAILS
  const PmodConfig& config = PmodConfig::getConfig();
#endif
  
  if(!landSurface.HasMember("texture"))
   {
    LogOLDFDetails("No landSurface:texture object in OLDF file %s\n", config.designFileName);
    return true; // it is optional
   }
  
  if(!landSurface["texture"].IsObject())
   {
    LogOLDFValidity("landSurface:texture is not object in OLDF file %s\n",
                                                                config.designFileName);
    retVal = false;
   }
  else
   {
    Value& texture  = landSurface["texture"];
    bool widthOk  = true;
    bool urlOk    = true;
    bool offsetOk = true;
    
    float width;
    if(texture.HasMember("width") && texture["width"].IsNumber())
      width = texture["width"].GetFloat();
    else
     {
      LogOLDFValidity("Bad landSurface:texture:width in OLDF file %s\n",
                                                                config.designFileName);
      widthOk = false;
     }
    const char* url;
    if(texture.HasMember("url") && texture["url"].IsString())
      url = texture["url"].GetString();
    else
     {
      LogOLDFValidity("Bad landSurface:texture:url in OLDF file %s\n",
                                                                config.designFileName);
      urlOk = false;
     }
    float west, south;
    if(texture.HasMember("offset") && texture["offset"].IsArray())
     {
      Value& offset = texture["offset"];
      if(offset.Size() != 2)
       {
        LogOLDFValidity("landSurface:texture:offset has wrong size %d in OLDF file %s\n",
                                                    offset.Size(), config.designFileName);
        offsetOk = false;
       }
      else
       {
        if(offset[0].IsNumber() && offset[1].IsNumber())
         {
          west  = offset[0].GetFloat();
          south = offset[1].GetFloat();
         }
        else
         {
          LogOLDFValidity("landSurface:texture:offset: non-numerical vals in OLDF file %s\n",
                                                                  config.designFileName);
          offsetOk = false;
         }
       }
     }
    else
     {
      LogOLDFValidity("Bad landSurface:texture:offset in OLDF file %s\n",
                                                                config.designFileName);
      retVal = false;
     }
    if(widthOk && urlOk && offsetOk)
     {
      // the whole texture object is soft and sweet-smelling.
      LogOLDFDetails("Valid landSurface:texture object for %s in OLDF file %s\n",
                                                        url, config.designFileName);
     }
    else
      retVal = false; // specific problems will have been logged above.
   }
 return retVal;
}


// =======================================================================================
// Function to check the OLDF altitudes inside landSurface (note that it is optional).

bool PmodDesign::validateAltitudes(Value& landSurface)
{
  bool retVal = true;
#if defined LOG_OLDF_VALIDITY || defined LOG_OLDF_DETAILS
  const PmodConfig& config = PmodConfig::getConfig();
#endif
  
  if(!landSurface.HasMember("altitudes"))
   {
    LogOLDFDetails("No landSurface:altitudes object in OLDF file %s\n", config.designFileName);
    return true; // it is optional
   }

  if(landSurface["altitudes"].IsArray())
   {
    Value& altitudeArray = landSurface["altitudes"];
    for(int i = 0; i < altitudeArray.Size(); i++)
     {
      if(altitudeArray[i].IsArray())
       {
        int s = altitudeArray[i].Size();
        if(s == 3 || s == 4)
         {
          for(int j=0; j<3; j++)
            if(altitudeArray[i][j].IsNumber())
             {
              //XXX need to check fits in bounds?
             }
            else
             {
              LogOLDFValidity("landSurface:altitudes[%d][%d] is non numeric"
                      " in OLDF file %s\n", i, j, config.designFileName);
              retVal = false;
             }
         }
        else
         {
          LogOLDFValidity("landSurface:altitudes[%d] is wrong size (%d) array"
                              " in OLDF file %s\n", i, s, config.designFileName);
          retVal = false;
         }
       }
      else
       {
        LogOLDFValidity("landSurface:altitudes[%d] is not array in OLDF file %s\n",
                                                            i, config.designFileName);
        retVal = false;
       }
     }
   }
  else
   {
    LogOLDFValidity("landSurface:altitudes is not array in OLDF file %s\n",
                                                                config.designFileName);
    retVal = false;
   }


 return retVal;
}


// =======================================================================================
// Function to check the structure of the OLDF landSurface object.  Note the convention
// is that the syntactical correctness and anything that can be statically checked our
// checked here.  However dynamical issues (eg files not present, urls not loading) are
// checked in the LandSurface class.

bool PmodDesign::validateLandSurface(void)
{
  bool    retVal  = true;
  Value&  landSurface  = doc["landSurface"];

  retVal &= validateTexture(landSurface);
  retVal &= validateAltitudes(landSurface);

  return retVal;
}


// =======================================================================================
// Function to check the structure of a tree Diameter field in an OLDF plant object.

#define MAX_TREE_RADIUS 7000 //in mm.  See https://en.wikipedia.org/wiki/List_of_superlative_trees

bool PmodDesign::validateTreeGirth(rapidjson::Value& treeGirthArray, char* objName)
{
  bool retVal  = true;
#if defined LOG_OLDF_VALIDITY //|| defined LOG_OLDF_DETAILS
  const PmodConfig& config = PmodConfig::getConfig();
#endif
  
  unless(treeGirthArray.Size() == 2)
   {
    LogOLDFValidity("%s treeGirth array is wrong size %d in OLDF file %s\n",
                    objName,  treeGirthArray.Size(), config.designFileName);
    retVal = false;
   }

  unless(treeGirthArray[0].IsNumber() && treeGirthArray[1].IsNumber())
   {
    LogOLDFValidity("%s treeGirth array values non-numerical in OLDF file %s\n",
                                                          objName, config.designFileName);
    retVal = false;
   }

  float treeRadius = treeGirthArray[0].GetFloat()*mmPerSpaceUnit/2.0f/M_PI;
  unless(treeRadius > 0.0f && treeRadius < MAX_TREE_RADIUS)
   {
    LogOLDFValidity("%s treeGirth size out of range in OLDF file %s\n",
                                                          objName, config.designFileName);
    retVal = false;
   }

  if(retVal)
   {
    LogOLDFDetails("%s radius %.2f at breast height measured at %.2f\n",
                                    objName, treeRadius, treeDiamArray[1].GetFloat());
   }
  return retVal;
}


// =======================================================================================
// Function to check the structure of any OLDF plant objects present.

bool PmodDesign::validatePlants(void)
{
  bool    retVal  = true;
#if defined LOG_OLDF_VALIDITY //|| defined LOG_OLDF_DETAILS
  const PmodConfig& config = PmodConfig::getConfig();
#endif
  Value&  plants  = doc["plants"];
  
  unless(plants.IsArray())
   {
    LogOLDFValidity("Plants is not an array in OLDF file %s\n", config.designFileName);
    return false;
   }
  
  int N = plants.Size();
  for(int i=0; i<N; i++)
   {
    unless(plants[i].IsObject())
     {
      LogOLDFValidity("Plants[%d]is not an object in OLDF file %s\n",
                                                        i, config.designFileName);
      retVal = false;
     }
    char logObjectName[16];
    sprintf(logObjectName, "plants[%d]", i);
    
    // Location
    unless(plants[i].HasMember("location") && plants[i]["location"].IsArray())
     {
      LogOLDFValidity("Absent or malformed location for %s in OLDF file %s\n",
                                      logObjectName, config.designFileName);
      retVal = false;
     }
    unless(plants[i]["location"].Size() == 2)
     {
      LogOLDFValidity("Wrong size array for %s in OLDF file %s\n",
                                      logObjectName, config.designFileName);
      retVal = false;
     }
    unless(plants[i]["location"][0].IsNumber() && plants[i]["location"][1].IsNumber())
     {
      LogOLDFValidity("Location array values non-numerical for %s in OLDF file %s\n",
                                      logObjectName, config.designFileName);
      retVal = false;
     }

    // yearPlanted
    unless(plants[i].HasMember("yearPlanted") && plants[i]["yearPlanted"].IsInt())
     {
      LogOLDFValidity("Missing or invalid yearPlanted for %s in OLDF file %s\n",
                                      logObjectName, config.designFileName);
      retVal = false;
     }

    // timePlanted
    if(plants[i].HasMember("timePlanted"))
     {
      char fieldName[32];
      sprintf(fieldName, "plants[%d] timePlanted", i);
      retVal &= validateJSONUnixTime(plants[i]["timePlanted"], fieldName);
     }

    // Genus
    retVal &= jCheck->validateStringMemberExists(plants[i], logObjectName, (char*)"genus");
    retVal &= jCheck->validateGenusName(logObjectName, plants[i]["genus"].GetString());
    
    // Species
    retVal &= jCheck->validateStringMemberExists(plants[i], logObjectName, (char*)"species");
    retVal &= jCheck->validateSpeciesName(logObjectName, plants[i]["species"].GetString());

    // Variety
    retVal &= jCheck->validateOptionalStringMember(plants[i], logObjectName, (char*)"var");

    // Taxonomy link
    retVal &= jCheck->validateOptionalStringOrArrayString(plants[i], logObjectName,
                                                                    (char*)"taxonomyLink");

    // Common Name
    retVal &= jCheck->validateOptionalStringMember(plants[i], logObjectName,
                                                                        (char*)"commonName");

    // Tree Diameter
    unless(plants[i].HasMember("treeGirth") && plants[i]["treeGirth"].IsArray())
     {
      LogOLDFValidity("Missing or invalid treeGirth for %s in OLDF file %s\n",
                                      logObjectName, config.designFileName);
      retVal = false;
     }
    else
      retVal &= validateTreeGirth(plants[i]["treeGirth"], logObjectName);

    // Notes
    retVal &= jCheck->validateOptionalStringOrArrayString(plants[i], logObjectName,
                                                                    (char*)"notes");
   }
  
  return retVal;
}


// =======================================================================================
// Function to check the structure of the OLDF object after it has been parsed out of the
// file (ie it is at least known to be valid JSON at this point, now we want to know if
// it's valid OLDF.  Do a lot of logging of the structure if so configured.

bool PmodDesign::validateOLDF(void)
{
  bool retVal = true;
  const PmodConfig& config = PmodConfig::getConfig();
  char phrase[128];
  snprintf(phrase, 128, "OLDF file %s", config.designFileName);
  jCheck = new JSONStructureChecker(phrase, OLDF);

  if(!(doc.HasMember("introductoryData") && doc["introductoryData"].IsObject()))
   {
    LogOLDFValidity("No introductoryData in %s\n", phrase);
    retVal = false;
   }
  else
    retVal &= validateIntroductoryData();

  if(!(doc.HasMember("boundaries") && doc["boundaries"].IsObject()))
   {
    LogOLDFValidity("No boundaries in %s\n", phrase);
    retVal = false;
   }
  else
    retVal &= boundary.validateBoundaries(doc["boundaries"]);

  if(!(doc.HasMember("landSurface") && doc["landSurface"].IsObject()))
   {
    LogOLDFValidity("No land Surface in %s\n", phrase);
    retVal = false;
   }
  else
    retVal &= validateLandSurface();

  if(doc.HasMember("plants")) // plants are optional
    retVal &= validatePlants();
  else
    LogOLDFDetails("No plants present in %s\n", phrase);

  delete jCheck; jCheck = NULL;
  return retVal;
}


// =======================================================================================
// Write out the introductory data section of the OLDF file.

void PmodDesign::writeIntroductoryData(char* indent)
{
  // Open the object
  fprintf(writeFile, "%s\"introductoryData\":\n", indent);
  fprintf(writeFile, "%s {\n", indent);

  // spaceUnits
  fprintf(writeFile, "%s%s\"spaceUnits\": \"%s\",\n",
                                              indent, indent, spaceUnitsText(metricUnits));
  // version
  fprintf(writeFile, "%s%s\"version\": %s,\n", indent, indent,
                                                            currentOLDFVersion.printJSON());
  // fileTime
  Timeval T;
  T.now();
  fprintf(writeFile, "%s%s\"fileTime\": [%ld, %d],\n", indent, indent, T.tv_sec, T.tv_usec);

  // baseYear
  fprintf(writeFile, "%s%s\"baseYear\": %d,\n", indent, indent, baseYear);

  // software
  fprintf(writeFile, "%s%s\"software\": \"Permaplan\",\n", indent, indent);

  // software version
  fprintf(writeFile, "%s%s\"softwareVersion\": \"TBD\",\n", indent, indent);

  // author - this is the last one without the comma after
  fprintf(writeFile, "%s%s\"author\": \"TBD\"\n", indent, indent);

  fprintf(writeFile, "%s },\n", indent);
}


// =======================================================================================
// Function to write out an OLDF file from current in memory state.  Note, we write out
// our JSON with simple fprintf statements in fixed order, as this approach is cheaper
// and easier than using the rapidjson library.  On reading, we need to handle any valid
// JSON formatting, but on writing, we only need a single valid format.

void PmodDesign::writeOLDFFile(LandSurface& land)
{
  const PmodConfig& config = PmodConfig::getConfig();
  
  // Get the file, opening comment, etc
  writeFile = fopen(config.writeDesignFileName, "w");
  unless(writeFile)
    err(-1, "Couldn't open %s to write out OLDF file.\n", config.writeDesignFileName);
  fprintf(writeFile, "//OLDF Format - see https://github.com/stuartstaniford"
                "/permaplan/blob/master/docs/open-landscape-description-format.md"
                "\n{\n");
  
  // Write out the different sections.
  
  // Introductory Data
  char* indent = (char*)"  ";
  writeIntroductoryData(indent);
  
  // Landsurface
  land.writeOLDFSection(writeFile, indent);

  // Landsurface
  boundary.writeOLDFSection(writeFile, indent);

  // Plants
  Tree::writeTreesToOLDF(writeFile, indent);
  
  // Go home, we are done.
  fprintf(writeFile, "}\n");
  fclose(writeFile);
  writeFile = NULL;
}


// =======================================================================================
// Destructor

PmodDesign::~PmodDesign()
{

}


// =======================================================================================
