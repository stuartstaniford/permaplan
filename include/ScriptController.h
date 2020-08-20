// Copyright Staniford Systems.  All Rights Reserved.  Aug 2020 -
// Class for reading a set of scripted UI actions from the JSON file, and then
// spoon feeding them to the Window3D class as it runs, to simulate some particular
// sequence of user actions.  Useful for creating test cases, canned demos, and the
// like.


#ifndef SCRIPTCONTROLLER_H
#define SCRIPTCONTROLLER_H


#include <unordered_map>
#include <string>
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
  void processNewScriptControl(void);
  void setupCamActionMap(void);
  void processCameraMovement(const char* type);
  unsigned simulatedKeys(float delta);
  bool diagnosticHTML(HttpDebug* serv);

 private:
  
  // Instance variables - private
  PmodDesign&                         design;
  double                              timeSec;
  double                              timeLimit;
  unsigned                            index;
  rapidjson::Value                    scriptArray;
  rapidjson::Value                    nextObject;
  std::unordered_map<std::string, unsigned> camActionMap;
  unsigned                            currentCamAction;
  
  // Member functions - private

};

#endif



