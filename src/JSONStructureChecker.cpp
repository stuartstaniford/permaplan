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
#include "JSONStructureChecker.h"

using namespace rapidjson;

// =======================================================================================
// Constructor: parse the json file

JSONStructureChecker::JSONStructureChecker(char* sPhrase, JSONType jType):
                                          sourcePhrase(sPhrase),
                                          type(jType)
{
  if(type == OLDF)
   {
    expectedVersion[0] = 0;
    expectedVersion[1] = 0;
    expectedVersion[2] = 2;
   }
  else if(type == OTDL)
   {
    expectedVersion[0] = 0;
    expectedVersion[1] = 0;
    expectedVersion[2] = 1;
   }
  else
    err(-1, "Unknown type in JSONStructureChecker::JSONStructureChecker");
}


// =======================================================================================
// Function to do the right kind of logging based on the kind of JSON this is.

void JSONStructureChecker::makeLog(bool isGood)
{
  if(type == OLDF)
   {
    if(isGood)
      LogOLDFDetails("%s", logBuf);
    else
      LogOLDFValidity("%s", logBuf);
   }
  else if(type == OTDL)
   {
    if(isGood)
      LogOTDLDetails("%s", logBuf);
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
    makeLog(false);
    retVal = false;
   }

 return retVal;
}


// =======================================================================================
// Function to check the OLDF spec version array.

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
        sprintBuf("version is [%d,%d,%d] in %s\n", expectedVersion[0],
                                  expectedVersion[1], expectedVersion[2], sourcePhrase);
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
// Destructor

JSONStructureChecker::~JSONStructureChecker()
{

}


// =======================================================================================
