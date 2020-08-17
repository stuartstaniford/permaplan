// Copyright Staniford Systems.  All Rights Reserved.  Aug 2020 -
// Class for reading a set of scripted UI actions from the JSON file, and then
// spoon feeding them to the Window3D class as it runs, to simulate some particular
// sequence of user actions.  Useful for creating test cases, canned demos, and the
// like.


#ifndef SCRIPTCONTROLLER_H
#define SCRIPTCONTROLLER_H

/*
#include <cglm/cglm.h>
#include <vector>
#include "Shader.h"
#include "VertexArrayObject.h"
#include "VertexBufferObject.h"
#include "TexturedRect.h"
#include "Quadtree.h"
#include "Camera.h"
#include "HeightMarker.h"
*/

#include "HttpDebug.h"
#include "PmodDesign.h"


// =======================================================================================
// Class variable initialization

class ScriptController
{
 public:
  
  // Instance variables - public
  
  // Member functions - public
  ScriptController(PmodDesign& D);
  ~ScriptController(void);
  bool diagnosticHTML(HttpDebug* serv);

 private:
  
  // Instance variables - private
  PmodDesign&           design;
  
  // Member functions - private

};

#endif



