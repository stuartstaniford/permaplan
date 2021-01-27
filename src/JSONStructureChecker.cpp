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
}


// =======================================================================================
// Function to do the right kind of logging based on the kind of JSON this is.

void JSONStructureChecker::makeLog(bool isErr)
{
  if(type == OLDF)
   {
    if(isErr)
      LogOLDFValidity("%s", logBuf);
    else
      LogOLDFDetails("%s", logBuf);
   }
  else if(type == OTDL)
   {
    if(isErr)
      LogOTDLValidity("%s", logBuf);
    else
      LogOTDLDetails("%s", logBuf);
   }
  else
    err(-1, "Unsupported type in JSONStructureChecker::makeLog\n");
}


// =======================================================================================
// Function to check and object has a valid file time.

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
          sprintf(logBuf, "fileTime array is not int at pos %d in %s\n", i, sourcePhrase);
          makeLog(true);
         }
       }
      if(fileTimeGood)
       {
        fileT.set(fileTimeArray[0].GetInt(), fileTimeArray[1].GetInt());
        sprintf(logBuf, "file time of %s is %s", sourcePhrase, fileT.ctimeString());
        makeLog(false);
       }
      else
        retVal = false;
     }
    else
     {
      sprintf(logBuf, "fileTime array is wrong size %d in %s\n",
                                                    fileTimeArray.Size(), sourcePhrase);
      makeLog(true);
     }
   }
  else
   {
    sprintf(logBuf, "No fileTime array in %s\n", sourcePhrase);
    makeLog(true);
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
