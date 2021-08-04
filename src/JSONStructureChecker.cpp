// Copyright Staniford Systems.  All Rights Reserved.  Apr 2020 -
// This class provides support for checking various generic patterns that show up 
// OTDL and OLDF a lot.


#include "JSONStructureChecker.h"
#include "PmodDesign.h"
#include "loadFileToBuf.h"
#include "Logging.h"
#include "Global.h"
#include <stdexcept>
#include <err.h>
#include <math.h>

using namespace rapidjson;


// =======================================================================================
/// @brief Constructor
/// @param sPhrase A C-string describing the source of the JSON for use in error messages.
/// @param jType A JSONType to tell us whether this particular helper object will be 
/// dealing with OTDL or OLDF.

JSONStructureChecker::JSONStructureChecker(char* sPhrase, JSONType jType):
                                          sourcePhrase(sPhrase),
                                          type(jType),
                                          languageTags(NULL)
{
  if(type == OLDF)
    expectedVersion = currentOLDFVersion;
  else if(type == OTDL)
    expectedVersion = currentOTDLVersion;
  else
    err(-1, "Unknown type in JSONStructureChecker::JSONStructureChecker");
}


// =======================================================================================
/// @brief Load language tags
/// 
/// Function to load the known language tags (per RFC 5646 into a JSON object for use
/// in parsing stuff that needs to know about different languages (eg commonNames).
/// @returns True (will exit on error).

bool JSONStructureChecker::loadRFC5646LanguageTags(void)
{
  bool retVal  = true;
  char* langFile = (char*)"rfc5646-language-tags.json";
  unsigned bufSize;
  char* buf = loadFileToBuf(langFile, &bufSize);
  languageTags = new Document;
  
  ParseResult ok = languageTags->ParseInsitu<kParseCommentsFlag>(buf);
  if (!ok)
   {
    fprintf(stderr, "JSON parse error on RFC 5646 language file %s: %s (%u)\n",
            langFile, GetParseError_En(ok.Code()), (unsigned)(ok.Offset()));
    exit(1);
   }
  if(!languageTags->IsObject())
    err(-1, "Base of %s is not JSON object.\n", langFile);

  return retVal;
}


// =======================================================================================
/// @brief Check that a mandatory but heritable value is available and numeric.
/// @returns True if value is present and correct, false otherwise.
/// @param jsonObject The containing object
/// @param name The name of the value we wish to check is present and numerical
/// @param pointer to a parent object, if inheritance is operating here.

bool JSONStructureChecker::checkMandatoryHeritableFloatValue(Value& jsonObject,
                                                                    void* parent, char* name)
{
  bool retVal = true;
  
  if(jsonObject.HasMember(name))
   {
    unless(jsonObject[name].IsNumber())
     {
      LogOTDLValidity("%s is not numeric in %s\n", name, sourcePhrase);
      retVal = false;
     }
   }
  else if(parent)
   {
    LogOTDLDetails("Inheriting %s from parent in %s\n", name, sourcePhrase);
   }
  else
   {
    LogOTDLValidity("No %s available for %s\n", name, sourcePhrase);
    retVal = false;
   }
  
  return retVal;
}


// =======================================================================================
/// @brief Check that a mandatory but heritable value is available and a non-negative
/// integer.
/// @returns True if value is present and correct, false otherwise.
/// @param jsonObject The containing object
/// @param name The name of the value we wish to check is present and correct
/// @param parent A pointer to a parent object, if inheritance is operating here.

bool JSONStructureChecker::checkMandatoryHeritableUnsignedValue(Value& jsonObject,
                                                                    void* parent, char* name)
{
  bool retVal = true;
  
  if(jsonObject.HasMember(name))
   {
    unless(jsonObject[name].IsInt() && jsonObject[name].GetInt() >= 0)
     {
      LogOTDLValidity("%s is not non-negative integer in %s\n", name, sourcePhrase);
      retVal = false;
     }
   }
  else if(parent)
   {
    LogOTDLDetails("Inheriting %s from parent in %s\n", name, sourcePhrase);
   }
  else
   {
    LogOTDLValidity("No %s available for %s\n", name, sourcePhrase);
    retVal = false;
   }
  
  return retVal;
}


// =======================================================================================
/// @brief Check syntax of an object with multiple languages.
/// 
/// Function to check that an object exists and is of the form of a series of RFC 5646
/// language tags, followed by values associated with those languages (eg commonNames).
/// Caller is responsible for checking the object exists.
/// @returns True if the object has the correct structure, false otherwise.
/// @param thisObject A rapidjson::Value for the object to be checked.
/// @param objName A C-string name of the object for use in logging.
/// @todo We do not check that the string following a language tag is actually in that
/// language.

bool JSONStructureChecker::checkLanguageObject(Value& thisObject, char* objName)
{
  bool retVal  = true;

  unless(thisObject.IsObject())
   {
    sprintBuf("\"%s\" is not object in %s\n", objName, sourcePhrase);
    makeLog(false);
    return false;
   }

  if(!languageTags)
    loadRFC5646LanguageTags();
  Value& langTags = *languageTags;
  
  Value::ConstMemberIterator iter;
  for (iter = thisObject.MemberBegin(); iter != thisObject.MemberEnd(); ++iter)
   {
    const char* lang = iter->name.GetString();
   
    unless(langTags.HasMember(lang))
     {
      sprintBuf("\"%s\" in %s is not a valid language in %s\n", lang, objName,
                                                                        sourcePhrase);
      makeLog(false);
      retVal = false;
     }

    unless(iter->value.IsString())
     {
      sprintBuf("\"%s\" value in %s is not string in %s\n", lang, objName, sourcePhrase);
      makeLog(false);
      retVal = false;
     }
    
    //XX ideally we would here check that iter->value is actually in the right language.
    
    if(retVal)
     {
      sprintBuf("\"%s\" value in %s is %s in %s\n", langTags[lang].GetString(), objName,
                                        iter->value.GetString(), sourcePhrase);
      makeLog(true);
     }
   }

  return retVal;
}


// =======================================================================================
/// @brief Do the right kind of logging based on the flavor of JSON this is.
/// 
/// The log message should have already been stored in our state variable logBuf.
/// @param isGood.  A bool which is true for detail messages that are not errors, and
/// false if the message is about an actual validity problem.

void JSONStructureChecker::makeLog(bool isGood)
{
  if(type == OLDF)
   {
    if(isGood)
     {
      LogOLDFDetails("%s", logBuf);
     }
    else
      LogOLDFValidity("%s", logBuf);
   }
  else if(type == OTDL)
   {
    if(isGood)
     {
      LogOTDLDetails("%s", logBuf);
     }
    else
      LogOTDLValidity("%s", logBuf);
   }
  else
    err(-1, "Unsupported type in JSONStructureChecker::makeLog\n");
}


// =======================================================================================
/// @brief Test that a genus name starts with an upper case letter, and otherwise only 
/// has lower case letters.
/// @returns True if the genus name is in the valid form, false if it is not.
/// @param objName A C-string for the name of the containing object for log purposes.
/// @param genus A C-string with the putative genus name to check.

bool JSONStructureChecker::validateGenusName(char* objName, const char* genus)
{
  bool retVal  = true;

  int N = strlen(genus);
  
  unless(N > 1)
   {
    sprintBuf("Genus name %s too short in %s in %s\n", genus, objName, sourcePhrase);
    makeLog(false);
    return false;
   }

  unless(genus[0] >= 'A' && genus[0] <= 'Z')
   {
    sprintBuf("Genus name %s not capitalized in %s in %s\n", genus, objName, sourcePhrase);
    makeLog(false);
    retVal = false;
   }
  
  bool allLower = true;
  for(int i=1; i<N; i++)
    unless(genus[i] >= 'a' && genus[i] <= 'z')
      allLower = false;
  unless(allLower)
   {
    sprintBuf("Genus name %s not has non-alpha chars or upper case in %s "
                            "in %s\n", genus, objName, sourcePhrase);
    makeLog(false);
    retVal = false;
   }

  return retVal;
}


// =======================================================================================
/// @brief Test that a species name starts with and only has lower case letters.
/// @returns True if the species name is in the valid form, false if it is not.
/// @param objName A C-string for the name of the containing object for log purposes.
/// @param species A C-string with the putative species name to check.

bool JSONStructureChecker::validateSpeciesName(char* objName, const char* species)
{
  bool retVal  = true;

  int N = strlen(species);
  
  unless(N > 1)
   {
    sprintBuf("Species name %s too short in %s in %s\n", species, objName, sourcePhrase);
    makeLog(false);
    return false;
   }
  
  bool allLower = true;
  for(int i=0; i<N; i++)
    unless(species[i] >= 'a' && species[i] <= 'z')
      allLower = false;
  unless(allLower)
   {
    sprintBuf("Species name %s not has non-alpha chars or upper case in %s "
                            "in %s\n", species, objName, sourcePhrase);
    makeLog(false);
    retVal = false;
   }

  return retVal;
}


// =======================================================================================
/// @brief Check that a particular member exists and is a JSON string.
/// @returns True if value is present and correct, false otherwise.
/// @param thisObject The containing object
/// @param member The name of the value we wish to check is present and correct
/// @param objName C-string name of the containing object for logging descriptions.

bool JSONStructureChecker::validateStringMemberExists(Value& thisObject,
                                                              char* objName, char* member)
{
  bool retVal = true;
  
  if(thisObject.HasMember(member) && thisObject[member].IsString())
   {
    const char* token = thisObject[member].GetString();
    sprintBuf("\"%s\" is \"%s\" in %s object in %s\n", member, token, objName, sourcePhrase);
    makeLog(true);
   }
  else
   {
    sprintBuf("No %s:%s token in %s\n", objName, member, sourcePhrase);
    makeLog(true);
    retVal = false;
   }

 return retVal;
}


// =======================================================================================
/// @brief Check that a value is an array of numbers of a desired size.
/// @returns True if value is of the correct form, false otherwise.
/// @param array rapidjson::Value containing the putative array object
/// @param desiredSize The required array size (eg 3 for a vec3)
/// @param objName A C-string name of the containing object for logging descriptions.

bool JSONStructureChecker::validateNumberArray(Value& array, int desiredSize, char* objName)
{
  bool retVal = true;

  unless(array.IsArray())
   {
    sprintBuf("%s is not array in  %s\n", objName, sourcePhrase);
    makeLog(false);
    return false;
   }
  
  int N = array.Size();
  unless(N == desiredSize)
   {
    sprintBuf("%s is wrong size (%d instead of %d in  %s\n",
                                                objName, N, desiredSize, sourcePhrase);
    makeLog(false);
    retVal = false;
   }
  
  for(int i = 0; i<N; i++)
   {
    unless(array[i].IsNumber())
     {
      sprintBuf("%s array[%d] is not numerical in %s\n", objName, i, sourcePhrase);
      makeLog(false);
      retVal = false;
     }
   }

  return retVal;
}


// =======================================================================================
/// @brief Check that a value is an array of integers of a desired size.
/// @returns True if value is of the correct form, false otherwise.
/// @param array rapidjson::Value containing the putative array object
/// @param desiredSize The required array size (eg 3 for a vec3)
/// @param objName A C-string name of the containing object for logging descriptions.

bool JSONStructureChecker::validateIntegerArray(Value& array, int desiredSize, char* objName)
{
  bool retVal = validateNumberArray(array, desiredSize, objName);

  if(retVal)
   {
    int N = array.Size();
    for(int i = 0; i<N; i++)
     {
      unless(array[i].IsInt())
       {
        sprintBuf("%s array[%d] is not an integer in %s\n", objName, i, sourcePhrase);
        makeLog(false);
        retVal = false;
       }
     }
   }

  return retVal;
}


// =======================================================================================
/// @brief Check that a value is is a valid array of RGB values.
/// @returns True if value is of the correct form, false otherwise.
/// @param array rapidjson::Value containing the putative array object
/// @param objName A C-string name of the containing object for logging descriptions.

bool JSONStructureChecker::validateRGBArray(Value& array, char* objName)
{
  bool retVal = validateIntegerArray(array, 3, objName);

  if(retVal)
   {
    int N = array.Size();
    for(int i = 0; i<N; i++)
     {
      int value = array[i].GetInt();
      unless(value >= 0 && value < 256)
       {
        sprintBuf("%s array[%d] is %d, out of range [0,255] in %s\n",
                                              objName, i, value, sourcePhrase);
        makeLog(false);
        retVal = false;
       }
     }
   }

  return retVal;
}


// =======================================================================================
/// @brief Produce a color as a 4 byte quantity from an RGB json array.
///
/// RGB array is assumed valid, no checking is done in here (see validateRGBArray for that).
/// @returns An unsigned holding the 4 byte RGBA value.
/// @param array A rapidjson::Value reference to the JSON RGBA array. 

unsigned colorFromRGBArray(Value& array)
{
  unsigned color = 0x000000ff;
  char* colorBytes = (char*)&color;

  for(int i = 0; i<3; i++)
   colorBytes[i] = array[i].GetInt();

  return color;
}


// =======================================================================================
/// @brief Produc an RGB json array from a color as a 4 byte quantity.
/// @param color An unsigned holding the 4 byte RGBA value.
/// @param buf  A C-string buffer to write the JSON into.  Caller must make sure it is
/// big enough.

void RGBArrayFromColor(unsigned color, char buf[])
{
  sprintf(buf, "[%u, %u, %u]", (color>>24)&0x000000ff, (color>>16)&0x000000ff,
          (color>>8)&0x000000ff);
}


// =======================================================================================
/// @brief Check the a three member spec version array matches the expected value.
/// 
/// Our sense of the expected value is in the expectedVersion array.
/// @returns True if value is present and correct, false otherwise.
/// @param containObj Containing object rapidjson::Value (we check to see there is a 
/// "version" field).

bool JSONStructureChecker::validateVersion(Value& containObj)
{
  bool retVal = true;
  
  if(containObj.HasMember("version") && containObj["version"].IsArray())
   {
    Value& versionArray = containObj["version"];
    if(versionArray.Size() == 3)
     {
      bool versionGood = true;
      for (int i = 0; i < versionArray.Size(); i++)
       {
        if(!(versionArray[i].IsInt() && versionArray[i].GetInt() == expectedVersion[i]))
         {
          versionGood = false;
          sprintBuf("version array is not %d at pos %d in %s\n",
                                                    expectedVersion[i], i, sourcePhrase);
          makeLog(false);
         }
       }
      if(versionGood)
       {
        sprintBuf("version is %s in %s\n", expectedVersion.printJSON(), sourcePhrase);
        makeLog(true);
       }
      else
        retVal = false;
     }
    else
     {
      sprintBuf("version array is wrong size %d in  %s\n",
                                                      versionArray.Size(), sourcePhrase);
      makeLog(false);
     }
   }
  else
   {
    sprintBuf("No version array in %s\n", sourcePhrase);
    makeLog(false);
    retVal = false;
   }
  
  return retVal;
}


// =======================================================================================
/// @brief Check an object has a valid file time.
/// @returns True if value is present and correct, false otherwise.
/// @param containObj Containing object rapidjson::Value (we check to see there is a 
/// "fileTime" field).

bool JSONStructureChecker::validateFileTime(Value& containObj)
{
  bool retVal = true;
  Timeval fileT;
  
  if(containObj.HasMember("fileTime") && containObj["fileTime"].IsArray())
   {
    Value& fileTimeArray = containObj["fileTime"];
    if(fileTimeArray.Size() == 2)
     {
      bool fileTimeGood = true;
      for (int i = 0; i < fileTimeArray.Size(); i++)
       {
        if(!(fileTimeArray[i].IsInt()))
         {
          fileTimeGood = false;
          sprintBuf("fileTime array is not int at pos %d in %s\n", i, sourcePhrase);
          makeLog(false);
         }
       }
      if(fileTimeGood)
       {
        fileT.set(fileTimeArray[0].GetInt(), fileTimeArray[1].GetInt());
        sprintBuf("file time of %s is %s", sourcePhrase, fileT.ctimeString());
        makeLog(true);
       }
      else
        retVal = false;
     }
    else
     {
      sprintBuf("fileTime array is wrong size %d in %s\n",
                                                    fileTimeArray.Size(), sourcePhrase);
      makeLog(false);
     }
   }
  else
   {
    sprintBuf("No fileTime array in %s\n", sourcePhrase);
    makeLog(false);
    retVal = false;
   }
  
 return retVal;
}


// =======================================================================================
/// @brief Check that if a particular member exists, it is a JSON string.
/// @returns True if value is missing, or if it is present is correct. False otherwise.
/// @param thisObject Containing object rapidjson::Value.
/// @param objName C-string name of the containing object for logging descriptions.
/// @param member The name of the value we wish to check is correct if present.

bool JSONStructureChecker::validateOptionalStringMember(Value& thisObject, char* objName, char* member)
{
  bool retVal = true;
  
  if(thisObject.HasMember(member))
   {
    if(thisObject[member].IsString())
     {
      const char* token = thisObject[member].GetString();
      sprintBuf("\"%s\" is \"%s\" in %s object in %s\n", member, token, objName, sourcePhrase);
      makeLog(true);
     }
    else
     {
      retVal = false;
      sprintBuf("%s:%s token is not string in %s\n", objName, member, sourcePhrase);
      makeLog(false);
     }
   }
  else
   {
    sprintBuf("Non-required %s:%s token not present in %s\n", objName, member, sourcePhrase);
    makeLog(true);
  }
 return retVal;
}


// =======================================================================================
/// @brief Check that if a particular member exists, it is either a JSON string, or
/// an array of JSON strings.
/// 
/// This is an idiom used in several places in OLDF/OTDL to allow multi-valued things.
/// @returns True if value is missing, or if it is present is correct. False otherwise.
/// @param thisObject Containing object rapidjson::Value.
/// @param objName C-string name of the containing object for logging descriptions.
/// @param member The name of the value we wish to check is correct if present.

bool JSONStructureChecker::validateOptionalStringOrArrayString(Value& thisObject,
                                                            char* objName, char* member)
{
  bool retVal = true;
  
  if(thisObject.HasMember(member))
   {
    if(thisObject[member].IsString())
     {
      const char* token = thisObject[member].GetString();
      sprintBuf("\"%s\" is \"%s\" in %s object in %s\n", member, token,
                                                      objName, sourcePhrase);
      makeLog(true);
     }
    else if(thisObject[member].IsArray())
     {
      int N = thisObject[member].Size();
      for(int i=0; i<N; i++)
       {
        unless(thisObject[member][i].IsString())
         {
          sprintBuf("%s:%s token is not string at array position %d in %s\n",
                          objName, member, i, sourcePhrase);
          makeLog(false);
          retVal = false;
         }
       }
      if(retVal) // Multiple correct authors present.
       {
        sprintBuf("\"%s\" has %d values in %s object in %s\n", member, N,
                                                        objName, sourcePhrase);
        makeLog(true);
       }
     }
    else
     {
      retVal = false;
      sprintBuf("%s:%s token is not string or array of strings in %s\n",
                                                        objName, member, sourcePhrase);
      makeLog(false);
     }
   }
  else
   {
    sprintBuf("Non-required %s:%s token not present in %s\n",
                                                          objName, member, sourcePhrase);
    makeLog(true);
   }
 return retVal;
}


// =======================================================================================
/// @brief Destructor

JSONStructureChecker::~JSONStructureChecker()
{

}


// =======================================================================================
