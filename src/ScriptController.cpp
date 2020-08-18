// Copyright Staniford Systems.  All Rights Reserved.  Aug 2020 -
// Class for reading a set of scripted UI actions from the JSON file, and then
// spoon feeding them to the Window3D class as it runs, to simulate some particular
// sequence of user actions.  Useful for creating test cases, canned demos, and the
// like.

#include <err.h>
#include <cstring>
#include "VertexBufferObject.h"
#include "ScriptController.h"

using namespace rapidjson;

// =======================================================================================
// Constructor which sets up the surface as specified in the design file

ScriptController::ScriptController(PmodDesign& D): design(D)
{

  unless(design.doc.HasMember("scriptControl") && design.doc["scriptControl"].IsObject())
   {
    //XX somehow flag that we will be doing nothing
    return;
   }
  
  Value& scriptArray = design.doc["scriptControl"];
  unless(scriptArray.IsArray())
    err(-1, "Misformatted scriptControl is not array in JSON file.");

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
