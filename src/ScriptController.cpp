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

ScriptController::ScriptController(PmodDesign& D):
                                      design(D),
                                      timeSec(-1.0),
                                      timeLimit(0.0),
                                      index(0u),
                                      camActionMap(),
                                      interfaceActionMap(),
                                      currentCamAction(0u),
                                      currentInterfaceAction(IA_None)
{
  setupMaps();
  
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
}


// =======================================================================================
// Destructor

ScriptController::~ScriptController(void)
{
}

// =======================================================================================
// Initialize the camActionMap (a static mapping from cam action names to flag values).

void ScriptController::setupMaps(void)
{
  camActionMap["CAM_PITCH_UP"]          = CAM_PITCH_UP;
  camActionMap["CAM_PITCH_DOWN"]        = CAM_PITCH_DOWN;
  camActionMap["CAM_YAW_LEFT"]          = CAM_YAW_LEFT;
  camActionMap["CAM_YAW_RIGHT"]         = CAM_YAW_RIGHT;
  camActionMap["CAM_ROLL_LEFT"]         = CAM_ROLL_LEFT;
  camActionMap["CAM_ROLL_RIGHT"]        = CAM_ROLL_RIGHT;
  camActionMap["CAM_MOVE_FORWARD"]      = CAM_MOVE_FORWARD;
  camActionMap["CAM_MOVE_BACK"]         = CAM_MOVE_BACK;
  camActionMap["CAM_MOVE_UP"]           = CAM_MOVE_UP;
  camActionMap["CAM_MOVE_DOWN"]         = CAM_MOVE_DOWN;
  camActionMap["CAM_MOVE_LEFT"]         = CAM_MOVE_LEFT;
  camActionMap["CAM_MOVE_RIGHT"]        = CAM_MOVE_RIGHT;
  camActionMap["CAM_INTERFACE_ACTION"]  = CAM_INTERFACE_ACTION;
  
  interfaceActionMap["IA_None"]         = IA_None;
  interfaceActionMap["IA_HeightMarker"] = IA_HeightMarker;
  interfaceActionMap["IA_Unknown"]      = IA_Unknown;
}


// =======================================================================================
// Called when we are moving to the next (or first) script object in the array.

void ScriptController::processNewScriptControl(void)
{
  currentCamAction = 0u;
  
  if(index >= scriptArray.Size())
   {
    timeSec = -2.0;
    return;
   }
  
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
  else if(strncmp(type, "INTERFACE_ACTION", 16) == 0)
    processInterfaceAction(nextObject);
  else
    err(-1, "Bad script control at array position %d unsupported type.", index);
  
  index++;
}


// =======================================================================================
// Called when the next script object is an interface action.

void ScriptController::processInterfaceAction(Value& V)
{
  unless(V.HasMember("action"))
    err(-1, "Bad Interface action at array position %d has no action.", index);
  unless(V["action"].IsString())
    err(-1, "Bad Interface action at array position %d - action is not string.", index);
  const char* action = V["action"].GetString();
  unless(interfaceActionMap[action])
    err(-1, "Bad script control at array position %d unknown interface action %s.",
                                                                          index, action);
  currentInterfaceAction  = interfaceActionMap[action];
  currentCamAction        = CAM_INTERFACE_ACTION;
  
  if(currentInterfaceAction == IA_HeightMarker)
    setHeight(V);
}


// =======================================================================================
// Used when we have a IA_HeightMarker to extract and set the height

void ScriptController::setHeight(Value& V)
{
  unless(V.HasMember("height"))
    err(-1, "Bad Interface action at array position %d has no height.", index);
  unless(V["height"].IsNumber())
    err(-1, "Bad Interface action at array position %d - height is not a number.", index);

  args[0] = V["height"].GetFloat();
}


// =======================================================================================
// Called when the next script object is a camera movement.

void ScriptController::processCameraMovement(const char* type)
{
  unless(camActionMap[type])
    err(-1, "Bad script control at array position %d unknown camera movement %s.",
                                              index, type);
  unless(nextObject.HasMember("duration"))
    err(-1, "Bad script control at array position %d no duration.", index);
  unless(nextObject["duration"].IsNumber())
    err(-1, "Bad script control at array position %d duration not number.", index);
  double duration = nextObject["duration"].GetDouble();
  
  timeLimit += duration;
  currentCamAction = camActionMap[type];
  //printf("currentCamAction: %x\n", currentCamAction);
}


// =======================================================================================
// Interface to determine if the simulation script is calling for any keypresses
// Uses flags defined in Camera.h.  Arg delta is in microseconds.

unsigned ScriptController::simulatedKeys(float delta)
{
  if(timeSec < -1.5)  // There is no script, we'll never do anything.
    return 0u;
  
  if(timeSec < 0.0)  // First time we are called
    timeSec = 0.0;
  
  timeSec += delta/1000000.0;
  //printf("%.3f\n", timeSec);
  if(timeSec > timeLimit)
    processNewScriptControl();
  
  return currentCamAction;
}


// =======================================================================================
// Interface to determine if the simulation script is calling for any keypresses
// Uses flags defined in Camera.h.  Arg delta is in microseconds.

bool ScriptController::checkInterfaceAction(InterfaceAction I)
{
  if(currentCamAction == CAM_INTERFACE_ACTION && currentInterfaceAction == I)
   {
    currentInterfaceAction = IA_None;
    processNewScriptControl();
    return true;
   }
  else
    return false;
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
