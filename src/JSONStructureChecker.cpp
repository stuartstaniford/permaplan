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
// Function to check that a mandatory but heritable thing is available.

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
// Function to check that a mandatory but heritable thing is available.

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
// Function to check that an object exists and is of the form of a series of RFC 5646
// language tags, followed by values associated with those languages (eg commonNames).
// Caller is responsible for checking the object exists.

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
// Function to do the right kind of logging based on the kind of JSON this is.

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
// Utility function to test that a genus name starts with an upper case letter, and
// otherwise only has lower case letters.

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
// Utility function to test that a species name starts only has lower case letters.

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
// Function to check that a particular member exists and is a JSON string.

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
// Function to check that a value is an array of numbers of a desired size.

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
// Function to check that a value is an array of integers of a desired size.

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
        sprintBuf("%s array[%d] is not numerical in %s\n", objName, i, sourcePhrase);
        makeLog(false);
        retVal = false;
       }
     }
   }

  return retVal;
}


// =======================================================================================
// Function to check that a value is a valid array of RGB values.

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
// Utility function to produce a color as a 4 byte quantity from an RGB json array.
// RGB array is assumed valid, no checking is done in here (see validateRGBArray for that).

unsigned colorFromRGBArray(Value& array)
{
  unsigned color = 0x000000ff;
  char* colorBytes = (char*)&color;

  for(int i = 0; i<3; i++)
   colorBytes[i] = array[i].GetInt();

  return color;
}


// =======================================================================================
// Utility function to produce an RGB json array from a color as a 4 byte quantity.

void RGBArrayFromColor(unsigned color, char buf[])
{
  sprintf(buf, "[%u, %u, %u]", (color>>24)&0x000000ff, (color>>16)&0x000000ff,
          (color>>8)&0x000000ff);
}


// =======================================================================================
// Function to check the a three member spec version array.

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
// Function to check an object has a valid file time.

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
// Function to check that if a particular member exists, it is a JSON string.

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
// Function to check that if a particular member exists, it is either a JSON string, or
// an array of JSON strings (an idiom used in several places in OLDF/OTDL to allow
// multi-valued things.

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
// Destructor

JSONStructureChecker::~JSONStructureChecker()
{

}


// =======================================================================================
