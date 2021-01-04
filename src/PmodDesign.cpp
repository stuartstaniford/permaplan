// Copyright Staniford Systems.  All Rights Reserved.  Apr 2020 -
// Main object that encapsulates the design of a given piece of land.  This is basically
// the contents of the main OLDF file given via -d.  Generally the rest of the command
// line is captured in PmodConfig, not in here.
// See docs/open-landscape-description-format.md for the file format.


#include "PmodDesign.h"
#include <stdexcept>
#include <err.h>
#include <math.h>
#include "loadFileToBuf.h"
#include "Logging.h"
#include "GlobalMacros.h"


using namespace rapidjson;

PmodDesign* PmodDesign::design = NULL;

// =======================================================================================
// Constructor: parse the json file

PmodDesign::PmodDesign(void):
                  writeFile(NULL),
                  metricUnits(false)
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
}


// =======================================================================================
// Function to check the spatial units.

bool PmodDesign::validateSpaceUnits(Value& introductoryData)
{
  bool retVal = true;
  const PmodConfig& config = PmodConfig::getConfig();
  
  if(introductoryData.HasMember("spaceUnits") && introductoryData["spaceUnits"].IsString())
   {
    const char* token = introductoryData["spaceUnits"].GetString();
    if(strcmp(token, "feet") == 0)
     {
      LogOLDFDetails("Using feet as spatial units in OLDF file %s\n", config.designFileName);
      metricUnits = false;
     }
    else if(strcmp(token, "meters") == 0 )
     {
      LogOLDFDetails("Using meters as spatial units in OLDF file %s\n", config.designFileName);
      metricUnits = true;
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
  const PmodConfig& config = PmodConfig::getConfig();
  
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
// Function to check the OLDF spec version array.

bool PmodDesign::validateVersion(Value& introductoryData)
{
  bool retVal = true;
  int expectedVersion[3] = {0,0,2};
  const PmodConfig& config = PmodConfig::getConfig();
  
  if(introductoryData.HasMember("version") && introductoryData["version"].IsArray())
   {
    Value& versionArray = introductoryData["version"];
    if(versionArray.Size() == 3)
     {
      bool versionGood = true;
      for (int i = 0; i < versionArray.Size(); i++)
       {
        if(!(versionArray[i].IsInt() && versionArray[i].GetInt() == expectedVersion[i]))
         {
          versionGood = false;
          LogOLDFValidity("introductoryData:version array is not %d at pos %d in OLDF file %s\n",
                                                    expectedVersion[i], i, config.designFileName);
         }
       }
      if(versionGood)
        LogOLDFDetails("version is [%d,%d,%d] in OLDF file %s\n", expectedVersion[0],
                                  expectedVersion[1], expectedVersion[2], config.designFileName);
      else
        retVal = false;
     }
    else
     {
      LogOLDFValidity("introductoryData:version array is wrong size %d in OLDF file %s\n",
                                                    versionArray.Size(), config.designFileName);
     }
   }
  else
   {
    LogOLDFValidity("No introductoryData:version array in OLDF file %s\n", config.designFileName);
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
  const PmodConfig& config = PmodConfig::getConfig();
  
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
// Function to check the OLDF spec file time.

bool PmodDesign::validateFileTime(Value& introductoryData)
{
  bool retVal = true;
  const PmodConfig& config = PmodConfig::getConfig();
  
  if(introductoryData.HasMember("fileTime") && introductoryData["fileTime"].IsArray())
   {
    Value& fileTimeArray = introductoryData["fileTime"];
    if(fileTimeArray.Size() == 2)
     {
      bool fileTimeGood = true;
      for (int i = 0; i < fileTimeArray.Size(); i++)
       {
        if(!(fileTimeArray[i].IsInt()))
         {
          fileTimeGood = false;
          LogOLDFValidity("introductoryData:fileTime array is not int at pos %d in OLDF file %s\n",
                                                    i, config.designFileName);
         }
       }
      fileTime.set(fileTimeArray[0].GetInt(), fileTimeArray[1].GetInt());
      if(fileTimeGood)
        LogOLDFDetails("file time of OLDF file %s is %s", config.designFileName,
                            fileTime.ctimeString());
      else
        retVal = false;
     }
    else
     {
      LogOLDFValidity("introductoryData:fileTime array is wrong size %d in OLDF file %s\n",
                                                    fileTimeArray.Size(), config.designFileName);
     }
   }
  else
   {
    LogOLDFValidity("No introductoryData:fileTime array in OLDF file %s\n", config.designFileName);
    retVal = false;
   }
  
 return retVal;
}


// =======================================================================================
// Function to check that a particular member exists and is a JSON string.

bool PmodDesign::validateStringMemberExists(Value& thisObject, char* objName, char* member)
{
  bool retVal = true;
  const PmodConfig& config = PmodConfig::getConfig();
  
  if(thisObject.HasMember(member) && thisObject[member].IsString())
   {
    const char* token = thisObject[member].GetString();
    LogOLDFDetails("\"%s\" is \"%s\" in %s object in OLDF file %s\n", member, token,
                                                      objName, config.designFileName);
   }
  else
   {
    LogOLDFValidity("No %s:%s token in OLDF file %s\n", objName, member,
                                                            config.designFileName);
    retVal = false;
   }

 return retVal;
}


// =======================================================================================
// Function to check that if a particular member exists, it is a JSON string.

bool PmodDesign::validateOptionalStringMember(Value& thisObject, char* objName, char* member)
{
  bool retVal = true;
  const PmodConfig& config = PmodConfig::getConfig();
  
  if(thisObject.HasMember(member))
   {
    if(thisObject[member].IsString())
     {
      const char* token = thisObject[member].GetString();
      LogOLDFDetails("\"%s\" is \"%s\" in %s object in OLDF file %s\n", member, token,
                                                      objName, config.designFileName);
     }
    else
     {
      retVal = false;
      LogOLDFValidity("%s:%s token is not string in OLDF file %s\n", objName, member,
                                                            config.designFileName);
     }
   }
  else
   {
    LogOLDFDetails("Non-required %s:%s token not present in OLDF file %s\n",
                                      objName, member, config.designFileName);
   }
 return retVal;
}


// =======================================================================================
// Function to check that if a particular member exists, it is either a JSON string, or
// an array of JSON strings (an idiom used in several places in OLDF to allow multi-valued
// things.

bool PmodDesign::validateOptionalStringOrArrayString(Value& thisObject,
                                                            char* objName, char* member)
{
  bool retVal = true;
  const PmodConfig& config = PmodConfig::getConfig();
  
  if(thisObject.HasMember(member))
   {
    if(thisObject[member].IsString())
     {
      const char* token = thisObject[member].GetString();
      LogOLDFDetails("\"%s\" is \"%s\" in %s object in OLDF file %s\n", member, token,
                                                      objName, config.designFileName);
     }
    else if(thisObject[member].IsArray())
     {
      int N = thisObject[member].Size();
      for(int i=0; i<N; i++)
       {
        unless(thisObject[member][i].IsString())
         {
          LogOLDFValidity("%s:%s token is not string at array position %d in OLDF file %s\n",
                          objName, member, i, config.designFileName);
          retVal = false;
         }
       }
     }
    else
     {
      retVal = false;
      LogOLDFValidity("%s:%s token is not string or array of strings in OLDF file %s\n", objName, member,
                                                            config.designFileName);
     }
   }
  else
   {
    LogOLDFDetails("Non-required %s:%s token not present in OLDF file %s\n",
                                      objName, member, config.designFileName);
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
  retVal &= validateVersion(introductoryData);
  retVal &= validateFileTime(introductoryData);
  retVal &= validateStringMemberExists(introductoryData,
                                    (char*)"introductoryData", (char*)"software");
  retVal &= validateOptionalStringMember(introductoryData,
                                    (char*)"introductoryData", (char*)"softwareVersion");
  retVal &= validateOptionalStringMember(introductoryData,
                                    (char*)"introductoryData", (char*)"author");
  return retVal;
}


// =======================================================================================
// Function to check the OLDF boundary reference point.

bool PmodDesign::validateReferencePoint(Value& boundaries)
{
  bool retVal = true;
  const PmodConfig& config = PmodConfig::getConfig();
  
  if(boundaries.HasMember("referencePoint") && boundaries["referencePoint"].IsArray())
   {
    Value& refPointArray = boundaries["referencePoint"];
    if(refPointArray.Size() == 2)
     {
      bool bothGood = true;
      
      // Latitude
      if(!(refPointArray[0].IsNumber()))
       {
        bothGood = false;
        LogOLDFValidity("referencePoint latitude is not numerical in OLDF file %s\n",
                                                                      config.designFileName);
       }
      else
       {
        if(refPointArray[0].GetDouble() < -90.0 || refPointArray[0].GetDouble() > 90.0)
         {
          bothGood = false;
          LogOLDFValidity("referencePoint latitude is out of range in OLDF file %s\n",
                                                                      config.designFileName);
         }
      }

      // Longtitude
      if(!(refPointArray[1].IsNumber()))
       {
        bothGood = false;
        LogOLDFValidity("referencePoint longtitude is not numerical in OLDF file %s\n",
                                                                      config.designFileName);
       }
      else
       {
        if(refPointArray[0].GetDouble() < -180.0 || refPointArray[0].GetDouble() > 180.0)
         {
          bothGood = false;
          LogOLDFValidity("referencePoint longtitude is out of range in OLDF file %s\n",
                                                                      config.designFileName);
         }
      }

      if(bothGood)
        LogOLDFDetails("Reference Point of OLDF file %s is lat %.3f, long %.3f\n",
            config.designFileName, refPointArray[0].GetDouble(), refPointArray[1].GetDouble());
      else
        retVal = false;
     }
    else
     {
      LogOLDFValidity("boundaries:referencePoint array is wrong size %d in OLDF file %s\n",
                                                    refPointArray.Size(), config.designFileName);
     }
   }
  else
   {
    LogOLDFValidity("No boundaries:referencePoint array in OLDF file %s\n", config.designFileName);
    retVal = false;
   }
  
 return retVal;
}


// =======================================================================================
// Function to check the OLDF arcs.

bool PmodDesign::validateArcs(Value& boundaries)
{
  bool retVal = true;
  const PmodConfig& config = PmodConfig::getConfig();
  float sumDist[2] = {0.0f, 0.0f};
  
  if(boundaries.HasMember("arcs") && boundaries["arcs"].IsArray())
   {
    Value& arcArray = boundaries["arcs"];
    if(arcArray.Size() >= 3)
     {
      bool allGood = true;
      
      for(int i=0; i<arcArray.Size(); i++)
       {
        if(!(arcArray[i].IsArray()))
         {
          allGood = false;
          LogOLDFValidity("arcs[%d] is not array in OLDF file %s\n", i, config.designFileName);
         }
        else
         {
          if(arcArray[i].Size() == 2)
            for(int j=0; j<2; j++)
             {
              if(arcArray[i][j].IsNumber())
                sumDist[j] += arcArray[i][j].GetFloat();
              else
               {
                allGood = false;
                LogOLDFValidity("arcs[%d][%d] is not numerical in OLDF file %s\n", i, j,
                                                                      config.designFileName);
               }
             }
          else
           {
            LogOLDFValidity("arcs[%d] has wrong size %d in OLDF file %s\n", i,
                                                    arcArray[i].Size(), config.designFileName);
           }
         }
       }
 
      if(allGood)
       {
        if( (fabs(sumDist[0]) < EPSILON) && (fabs(sumDist[1]) < EPSILON) )
          LogOLDFDetails("Correct arcs array of size %d in OLDF file %s\n",
                                                      arcArray.Size(), config.designFileName);
        else
         {
          retVal = false;
          LogOLDFValidity("arcs do not close (sum %f, %f) in OLDF file %s\n", sumDist[0],
                                                        sumDist[1], config.designFileName);
         }
       }
      else
        retVal = false;
     }
    else
     {
      LogOLDFValidity("boundaries:arcs array is too small (size %d) in OLDF file %s\n",
                                                    arcArray.Size(), config.designFileName);
     }
   }
  else
   {
    LogOLDFValidity("No boundaries:arcs array in OLDF file %s\n", config.designFileName);
    retVal = false;
   }
  
 return retVal;
}


// =======================================================================================
// Function to check the structure of the OLDF introductoryData object.

bool PmodDesign::validateBoundaries(void)
{
  bool    retVal      = true;
  Value&  boundaries  = doc["boundaries"];

  retVal &= validateReferencePoint(boundaries);
  retVal &= validateArcs(boundaries);

  return retVal;
}


// =======================================================================================
// Function to check the OLDF texture inside landSurface (note that it is optional).

bool PmodDesign::validateTexture(Value& landSurface)
{
  bool retVal = true;
  const PmodConfig& config = PmodConfig::getConfig();
  
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
  const PmodConfig& config = PmodConfig::getConfig();

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
// Utility function to test that a genus name starts with an upper case letter, and
// otherwise only has lower case letters.

bool validateGenusName(char* objName, const char* genus)
{
  bool retVal  = true;
  const PmodConfig& config = PmodConfig::getConfig();

  int N = strlen(genus);
  
  unless(N > 1)
   {
    LogOLDFValidity("Genus name %s too short in %s in OLDF file %s\n",
                                                    genus, objName, config.designFileName);
    return false;
   }

  unless(genus[0] >= 'A' && genus[0] <= 'Z')
   {
    LogOLDFValidity("Genus name %s not capitalized in %s in OLDF file %s\n",
                                                    genus, objName, config.designFileName);
    retVal = false;
   }
  
  bool allLower = true;
  for(int i=1; i<N; i++)
    unless(genus[i] >= 'a' && genus[i] <= 'z')
      allLower = false;
  unless(allLower)
   {
    LogOLDFValidity("Genus name %s not has non-alpha chars or upper case in %s "
                            "in OLDF file %s\n", genus, objName, config.designFileName);
    retVal = false;
   }

  return retVal;
}


// =======================================================================================
// Utility function to test that a species name starts only has lower case letters.

bool validateSpeciesName(char* objName, const char* species)
{
  bool retVal  = true;
  const PmodConfig& config = PmodConfig::getConfig();

  int N = strlen(species);
  
  unless(N > 1)
   {
    LogOLDFValidity("Species name %s too short in %s in OLDF file %s\n",
                                                    species, objName, config.designFileName);
    return false;
   }
  
  bool allLower = true;
  for(int i=0; i<N; i++)
    unless(species[i] >= 'a' && species[i] <= 'z')
      allLower = false;
  unless(allLower)
   {
    LogOLDFValidity("Species name %s not has non-alpha chars or upper case in %s "
                            "in OLDF file %s\n", species, objName, config.designFileName);
    retVal = false;
   }

  return retVal;
}


// =======================================================================================
// Function to check the structure of any OLDF plant objects present.

bool PmodDesign::validatePlants(void)
{
  bool    retVal  = true;
  const PmodConfig& config = PmodConfig::getConfig();
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
    retVal &= validateStringMemberExists(plants[i], logObjectName, (char*)"genus");
    retVal &= validateGenusName(logObjectName, plants[i]["genus"].GetString());
    
    // Species
    retVal &= validateStringMemberExists(plants[i], logObjectName, (char*)"species");
    retVal &= validateSpeciesName(logObjectName, plants[i]["species"].GetString());

    // Taxonomy link
    retVal &= validateOptionalStringOrArrayString(plants[i], logObjectName,
                                                                    (char*)"taxonomyLink");

    // Common Name
    retVal &= validateOptionalStringMember(plants[i], logObjectName, (char*)"commonName");
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

  if(!(doc.HasMember("introductoryData") && doc["introductoryData"].IsObject()))
   {
    LogOLDFValidity("No introductoryData in OLDF file %s\n", config.designFileName);
    retVal = false;
   }
  else
    retVal &= validateIntroductoryData();

  if(!(doc.HasMember("boundaries") && doc["boundaries"].IsObject()))
   {
    LogOLDFValidity("No boundaries in OLDF file %s\n", config.designFileName);
    retVal = false;
   }
  else
    retVal &= validateBoundaries();

  if(!(doc.HasMember("landSurface") && doc["landSurface"].IsObject()))
   {
    LogOLDFValidity("No land Surface in OLDF file %s\n", config.designFileName);
    retVal = false;
   }
  else
    retVal &= validateLandSurface();

  if(doc.HasMember("plants")) // plants are optional
    retVal &= validatePlants();
  else
    LogOLDFDetails("No plants present in OLDF file %s\n", config.designFileName);

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
  fprintf(writeFile, "%s%s\"version\": [0, 0, 2],\n", indent, indent);

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
  char* indent = (char*)"  ";
  writeIntroductoryData(indent);
  land.writeOLDFSection(writeFile, indent);
  
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
