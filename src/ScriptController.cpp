// Copyright Staniford Systems.  All Rights Reserved.  Aug 2020 -
// Class for reading a set of scripted UI actions from the JSON file, and then
// spoon feeding them to the Window3D class as it runs, to simulate some particular
// sequence of user actions.  Useful for creating test cases, canned demos, and the
// like.

#include <err.h>
#include <cstring>
#include "VertexBufferObject.h"
#include "ScriptController.h"
#include "Camera.h"

using namespace rapidjson;

// =======================================================================================
// Constructor which sets up the surface as specified in the design file

ScriptController::ScriptController(PmodDesign& D): design(D), timeSec(-1.0), timeLimit(0.0),
                                index(0u), camActionMap(), currentCamAction(0u)
{

  unless(design.doc.HasMember("scriptControl"))
   {
    timeSec = -2.0; // this means ScriptController will never actually do anything.
    return;
   }
  
  scriptArray = design.doc["scriptControl"];
  unless(scriptArray.IsArray())
    err(-1, "Misformatted scriptControl is not array in JSON file.");
  unless(scriptArray.Size() > 0)
   {
    timeSec = -2.0;
    warn("Empty scriptControl array in JSON file.");
   }
  
  processNewScriptControl();
  
  camActionMap[(char*)"CAM_PITCH_UP"]     = CAM_PITCH_UP;
  camActionMap[(char*)"CAM_PITCH_DOWN"]   = CAM_PITCH_DOWN;
  camActionMap[(char*)"CAM_YAW_LEFT"]     = CAM_YAW_LEFT;
  camActionMap[(char*)"CAM_YAW_RIGHT"]    = CAM_YAW_RIGHT;
  camActionMap[(char*)"CAM_ROLL_LEFT"]    = CAM_ROLL_LEFT;
  camActionMap[(char*)"CAM_ROLL_RIGHT"]   = CAM_ROLL_RIGHT;
  camActionMap[(char*)"CAM_MOVE_FORWARD"] = CAM_MOVE_FORWARD;
  camActionMap[(char*)"CAM_MOVE_BACK"]    = CAM_MOVE_BACK;
  camActionMap[(char*)"CAM_MOVE_UP"]      = CAM_MOVE_UP;
  camActionMap[(char*)"CAM_MOVE_DOWN"]    = CAM_MOVE_DOWN;
  camActionMap[(char*)"CAM_MOVE_LEFT"]    = CAM_MOVE_LEFT;
  camActionMap[(char*)"CAM_MOVE_RIGHT"]   = CAM_MOVE_RIGHT;

/*
  float width;
  if(LsJson.HasMember("width") && LsJson["width"].IsNumber())
    width = LsJson["width"].GetFloat();
  else
    err(-1, "Bad landSurface width in file %s\n", design.config.designFileName);

  if(LsJson.HasMember("textureFile") && LsJson["textureFile"].IsString())
    rect = new TexturedRect(shader, LsJson["textureFile"].GetString(), width, 0.0f);
  else
    err(-1, "Bad landSurface texturefile in file %s\n", design.config.designFileName);

*/
 }


// =======================================================================================
// Destructor

ScriptController::~ScriptController(void)
{
}


// =======================================================================================
// Called when we are moving to the next (or first) script object in the array.

void ScriptController::processNewScriptControl(void)
{
  currentCamAction = 0u;
  
  if(index >= scriptArray.Size())
    timeSec = -2.0;
 
  nextObject = scriptArray[index];
  
  unless(nextObject.IsObject())
    err(-1, "Bad script control at array position %d not object.", index);

  unless(nextObject.HasMember("type"))
    err(-1, "Bad script control at array position %d no type.", index);

  unless(nextObject["type"].IsString())
    err(-1, "Bad script control at array position %d type is not string.", index);

  const char* type = nextObject["type"].GetString();
  
  if(strncmp(type, "CAM_", 4) == 0)
    processCameraMovement(type);
  else
    err(-1, "Bad script control at array position %d unsupported type.", index);
  
  index++;
}


// =======================================================================================
// Called when we are moving to the next (or first) script object in the array.

void ScriptController::processCameraMovement(const char* type)
{
  unless(camActionMap[(char*)type] > 0u)
    err(-1, "Bad script control at array position %d unknown camera movement.", index);
  unless(nextObject.HasMember("duration"))
    err(-1, "Bad script control at array position %d no duration.", index);
  unless(nextObject["duration"].IsNumber())
    err(-1, "Bad script control at array position %d duration not number.", index);
  double duration = nextObject["duration"].GetDouble();
  
  timeLimit += duration;
  currentCamAction = camActionMap[(char*)type];
}


// =======================================================================================
// Interface to determine if the simulation script is calling for any keypresses
// Uses flags defined in Camera.h

unsigned ScriptController::simulatedKeys(float delta)
{
  unsigned retVal = 0u;
  
  if(timeSec < 1.5)  // There is no script, we'll never do anything.
    return 0u;
  
  if(timeSec < 0.0)  // First time we are called
    timeSec = 0.0;
  
  timeSec += delta;
  
  if(timeSec <= timeLimit)
    retVal = currentCamAction;
  else
    processNewScriptControl();
  
  return retVal;
}


// =======================================================================================
// Provide a diagnostic page about the land surface

bool ScriptController::diagnosticHTML(HttpDebug* serv)
{
  serv->startResponsePage("Script Controller");

  /*
  serv->addResponseData("<center>\n");
  serv->startTable();
  serv->addResponseData("<tr><th>Heightmarker index</th><th>X</th>");
  serv->addResponseData("<th>Y</th><th>Z</th></tr>\n");
  
  for(int i=0; i< locationCount; i++)
   {
    serv->respPtr += sprintf(serv->respPtr, "<tr><th>%d</td>", i);
    for(int j=0; j<3; j++)
      serv->respPtr += sprintf(serv->respPtr, "<td>%.1f</td>", heightLocations[i][j]);
    serv->addResponseData("</tr>\n");
   }
  
  serv->addResponseData("</table></center><hr>\n");
*/
  serv->endResponsePage();
  return true;
}


// =======================================================================================
